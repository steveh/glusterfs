/*
  Copyright (c) 2012 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/


#ifndef _LIBGLUSTERFSCLIENT_H
#define _LIBGLUSTERFSCLIENT_H

struct glfs_ctx;
typedef struct glfs_ctx glfs_ctx_t;

glfs_ctx_t *glfs_ctx_new (const char *volname);
int glfs_ctx_set_volfile (glfs_ctx_t *ctx, const char *volfile);
int glfs_ctx_set_volfile_server (glfs_ctx_t *ctx, const char *transport,
				 const char *host, int port);
int glfs_ctx_set_logging (glfs_ctx_t *ctx, const char *logfile, int loglevel);

int glfs_ctx_init (glfs_ctx_t *ctx);


#endif /* !_LIBGLUSTERFSCLIENT_H */
