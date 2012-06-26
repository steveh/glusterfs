/*
  Copyright (c) 2012 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

#define ALLOC(ptr) (ptr = calloc (1, sizeof (*ptr)), ptr)

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#include "glusterfs.h"
#include "logging.h"
#include "stack.h"
#include "event.h"
#include "libglusterfsclient-mem-types.h"
#include "common-utils.h"
#include "syncop.h"
#include "call-stub.h"


struct glfs_ctx {
	glusterfs_ctx_t  *gctx;
	pthread_t         poller;
};


#define DEFAULT_EVENT_POOL_SIZE            16384
#define GF_MEMPOOL_COUNT_OF_DICT_T        4096
#define GF_MEMPOOL_COUNT_OF_DATA_T        (GF_MEMPOOL_COUNT_OF_DICT_T * 4)
#define GF_MEMPOOL_COUNT_OF_DATA_PAIR_T   (GF_MEMPOOL_COUNT_OF_DICT_T * 4)

int glusterfs_mgmt_init (glusterfs_ctx_t *ctx);

static char *
generate_uuid ()
{
        char           tmp_str[1024] = {0,};
        char           hostname[256] = {0,};
        struct timeval tv = {0,};
        char           now_str[32];

        if (gettimeofday (&tv, NULL) == -1) {
                gf_log ("glusterfsd", GF_LOG_ERROR,
                        "gettimeofday: failed %s",
                        strerror (errno));
        }

        if (gethostname (hostname, sizeof hostname) == -1) {
                gf_log ("glusterfsd", GF_LOG_ERROR,
                        "gethostname: failed %s",
                        strerror (errno));
        }

        gf_time_fmt (now_str, sizeof now_str, tv.tv_sec, gf_timefmt_Ymd_T);
        snprintf (tmp_str, sizeof tmp_str, "%s-%d-%s:%" GF_PRI_SUSECONDS,
                  hostname, getpid(), now_str, tv.tv_usec);

        return gf_strdup (tmp_str);
}


static int
glusterfs_ctx_defaults_init (glusterfs_ctx_t *ctx)
{
        call_pool_t   *pool = NULL;
        int            ret = -1;

        xlator_mem_acct_init (THIS, glfs_mt_end);

        ctx->process_uuid = generate_uuid ();
        if (!ctx->process_uuid) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs uuid generation failed");
                goto out;
        }

        ctx->page_size  = 128 * GF_UNIT_KB;

        ctx->iobuf_pool = iobuf_pool_new ();
        if (!ctx->iobuf_pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs iobuf pool creation failed");
                goto out;
        }

        ctx->event_pool = event_pool_new (DEFAULT_EVENT_POOL_SIZE);
        if (!ctx->event_pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs event pool creation failed");
                goto out;
        }

	ctx->env = syncenv_new (0);
        if (!ctx->env) {
                gf_log ("glfs", GF_LOG_ERROR,
                        "Could not create new sync-environment");
                goto out;
        }

        pool = GF_CALLOC (1, sizeof (call_pool_t),
                          glfs_mt_call_pool_t);
        if (!pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs call pool creation failed");
                goto out;
        }

        /* frame_mem_pool size 112 * 4k */
        pool->frame_mem_pool = mem_pool_new (call_frame_t, 4096);
        if (!pool->frame_mem_pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs frame pool creation failed");
                goto out;
        }
        /* stack_mem_pool size 256 * 1024 */
        pool->stack_mem_pool = mem_pool_new (call_stack_t, 1024);
        if (!pool->stack_mem_pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs stack pool creation failed");
                goto out;
        }

        ctx->stub_mem_pool = mem_pool_new (call_stub_t, 1024);
        if (!ctx->stub_mem_pool) {
                gf_log ("glfs", GF_LOG_CRITICAL,
                        "ERROR: glusterfs stub pool creation failed");
                goto out;
        }

        ctx->dict_pool = mem_pool_new (dict_t, GF_MEMPOOL_COUNT_OF_DICT_T);
        if (!ctx->dict_pool)
                return -1;

        ctx->dict_pair_pool = mem_pool_new (data_pair_t,
                                            GF_MEMPOOL_COUNT_OF_DATA_PAIR_T);
        if (!ctx->dict_pair_pool)
                return -1;

        ctx->dict_data_pool = mem_pool_new (data_t, GF_MEMPOOL_COUNT_OF_DATA_T);
        if (!ctx->dict_data_pool)
                return -1;

        INIT_LIST_HEAD (&pool->all_frames);
	INIT_LIST_HEAD (&ctx->cmd_args.xlator_options);
        LOCK_INIT (&pool->lock);
        ctx->pool = pool;

        pthread_mutex_init (&(ctx->lock), NULL);

        ret = 0;
out:

        if (ret && pool) {

                if (pool->frame_mem_pool)
                        mem_pool_destroy (pool->frame_mem_pool);

                if (pool->stack_mem_pool)
                        mem_pool_destroy (pool->stack_mem_pool);

                GF_FREE (pool);
        }

        if (ret && ctx) {
                if (ctx->stub_mem_pool)
                        mem_pool_destroy (ctx->stub_mem_pool);

                if (ctx->dict_pool)
                        mem_pool_destroy (ctx->dict_pool);

                if (ctx->dict_data_pool)
                        mem_pool_destroy (ctx->dict_data_pool);

                if (ctx->dict_pair_pool)
                        mem_pool_destroy (ctx->dict_pair_pool);
        }

        return ret;
}

static FILE *
get_volfp (glusterfs_ctx_t *ctx)
{
        int          ret = 0;
        cmd_args_t  *cmd_args = NULL;
        FILE        *specfp = NULL;
        struct stat  statbuf;

        cmd_args = &ctx->cmd_args;

        ret = lstat (cmd_args->volfile, &statbuf);
        if (ret == -1) {
                gf_log ("glusterfsd", GF_LOG_ERROR,
                        "%s: %s", cmd_args->volfile, strerror (errno));
                return NULL;
        }

        if ((specfp = fopen (cmd_args->volfile, "r")) == NULL) {
                gf_log ("glusterfsd", GF_LOG_ERROR,
                        "volume file %s: %s",
                        cmd_args->volfile,
                        strerror (errno));
                return NULL;
        }

        gf_log ("glusterfsd", GF_LOG_DEBUG,
                "loading volume file %s", cmd_args->volfile);

        return specfp;
}


int
glusterfs_process_volfp (glusterfs_ctx_t *ctx, FILE *fp)
{
        glusterfs_graph_t  *graph = NULL;
        int                 ret = -1;
        xlator_t           *trav = NULL;

        graph = glusterfs_graph_construct (fp);
        if (!graph) {
                gf_log ("glfs", GF_LOG_ERROR, "failed to construct the graph");
                goto out;
        }

        for (trav = graph->first; trav; trav = trav->next) {
                if (strcmp (trav->type, "mount/fuse") == 0) {
                        gf_log ("glusterfsd", GF_LOG_ERROR,
                                "fuse xlator cannot be specified "
                                "in volume file");
                        goto out;
                }
        }

        ret = glusterfs_graph_prepare (graph, ctx);
        if (ret) {
                glusterfs_graph_destroy (graph);
                goto out;
        }

        ret = glusterfs_graph_activate (graph, ctx);

        if (ret) {
                glusterfs_graph_destroy (graph);
                goto out;
        }

        gf_log_volume_file (fp);

        ret = 0;
out:
        if (fp)
                fclose (fp);

        if (!ctx->active) {
		ret = -1;
        }

        return ret;
}


int
glusterfs_volumes_init (glusterfs_ctx_t *ctx)
{
        FILE               *fp = NULL;
        cmd_args_t         *cmd_args = NULL;
        int                 ret = 0;

        cmd_args = &ctx->cmd_args;

        if (cmd_args->volfile_server) {
                ret = glusterfs_mgmt_init (ctx);
                goto out;
        }

        fp = get_volfp (ctx);

        if (!fp) {
                gf_log ("glusterfsd", GF_LOG_ERROR,
                        "Cannot reach volume specification file");
                ret = -1;
                goto out;
        }

        ret = glusterfs_process_volfp (ctx, fp);
        if (ret)
                goto out;

out:
        return ret;
}

static int
logging_init (glusterfs_ctx_t *ctx)
{
        cmd_args_t *cmd_args = NULL;
        int         ret = 0;

        cmd_args = &ctx->cmd_args;

        if (cmd_args->log_file == NULL) {
                ret = gf_asprintf (&cmd_args->log_file, "/dev/stderr");
                if (ret == -1) {
                        fprintf (stderr, "ERROR: failed to set the log file path\n");
                        return -1;
                }
        }

        if (gf_log_init (cmd_args->log_file) == -1) {
                fprintf (stderr, "ERROR: failed to open logfile %s\n",
                         cmd_args->log_file);
                return -1;
        }

	cmd_args->log_level = 10;

        gf_log_set_loglevel (cmd_args->log_level);

        return 0;
}


////////////////////////////////////////////////////////////////////////////


static void *
glfs_poller (void *data)
{
	struct glfs_ctx  *ctx = NULL;

	ctx = data;

	event_dispatch (ctx->gctx->event_pool);

	return NULL;
}


struct glfs_ctx *
glfs_ctx_new (const char *volname)
{
	struct glfs_ctx   *ctx = NULL;
	int                ret = -1;

        ret = glusterfs_globals_init ();
        if (ret)
                return NULL;

	if (!ALLOC(ctx))
		return NULL;

        ctx->gctx = glusterfs_ctx_get ();
        if (!ctx->gctx) {
                return NULL;
	}

        ret = glusterfs_ctx_defaults_init (ctx->gctx);
        if (ret)
                return NULL;

        ret = logging_init (ctx->gctx);
        if (ret)
                return NULL;

	ctx->gctx->cmd_args.volfile_id = gf_strdup (volname);

	ret = pthread_create (&ctx->poller, NULL, glfs_poller, ctx);

	return ctx;
}


int
glfs_ctx_set_volfile (struct glfs_ctx *ctx, const char *volfile)
{
        cmd_args_t  *cmd_args = NULL;

	cmd_args = &ctx->gctx->cmd_args;

	cmd_args->volfile = gf_strdup (volfile);

	return 0;
}


int
glfs_ctx_set_volfile_server (struct glfs_ctx *ctx, const char *transport,
			     const char *host, int port)
{
        cmd_args_t  *cmd_args = NULL;

	cmd_args = &ctx->gctx->cmd_args;

	cmd_args->volfile_server = gf_strdup (host);

	return 0;
}


int
glfs_ctx_set_logging (struct glfs_ctx *ctx, const char *logfile, int loglevel)
{
	return 0;
}


int
glfs_ctx_init (struct glfs_ctx *ctx)
{
	int  ret = -1;

        ret = glusterfs_volumes_init (ctx->gctx);
        if (ret)
                return ret;

	return ret;
}

