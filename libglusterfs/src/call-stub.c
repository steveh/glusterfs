/*
  Copyright (c) 2008-2012 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#include <openssl/md5.h>
#include <inttypes.h>

#include "call-stub.h"
#include "mem-types.h"


static call_stub_t *
stub_new (call_frame_t *frame,
          char wind,
          glusterfs_fop_t fop)
{
        call_stub_t *new = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        new = mem_get0 (frame->this->ctx->stub_mem_pool);
        GF_VALIDATE_OR_GOTO ("call-stub", new, out);

        new->frame = frame;
        new->wind = wind;
        new->fop = fop;
        new->stub_mem_pool = frame->this->ctx->stub_mem_pool;
        INIT_LIST_HEAD (&new->list);
out:
        return new;
}


call_stub_t *
fop_lookup_stub (call_frame_t *frame,
                 fop_lookup_t fn,
                 loc_t *loc,
                 dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_LOOKUP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.lookup.fn = fn;

        loc_copy (&stub->args.lookup.loc, loc);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_lookup_cbk_stub (call_frame_t *frame,
                     fop_lookup_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     inode_t *inode,
                     struct iatt *buf,
                     dict_t *xdata,
                     struct iatt *postparent)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_LOOKUP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.lookup_cbk.fn = fn;
        stub->args.lookup_cbk.op_ret = op_ret;
        stub->args.lookup_cbk.op_errno = op_errno;
        if (inode)
                stub->args.lookup_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.lookup_cbk.buf = *buf;
        if (postparent)
                stub->args.lookup_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}



call_stub_t *
fop_stat_stub (call_frame_t *frame,
               fop_stat_t fn,
               loc_t *loc, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_STAT);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.stat.fn = fn;
        loc_copy (&stub->args.stat.loc, loc);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_stat_cbk_stub (call_frame_t *frame,
                   fop_stat_cbk_t fn,
                   int32_t op_ret,
                   int32_t op_errno,
                   struct iatt *buf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_STAT);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.stat_cbk.fn = fn;
        stub->args.stat_cbk.op_ret = op_ret;
        stub->args.stat_cbk.op_errno = op_errno;
        if (op_ret == 0)
                stub->args.stat_cbk.buf = *buf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fstat_stub (call_frame_t *frame,
                fop_fstat_t fn,
                fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FSTAT);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fstat.fn = fn;

        if (fd)
                stub->args.fstat.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fstat_cbk_stub (call_frame_t *frame,
                    fop_fstat_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    struct iatt *buf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FSTAT);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fstat_cbk.fn = fn;
        stub->args.fstat_cbk.op_ret = op_ret;
        stub->args.fstat_cbk.op_errno = op_errno;
        if (buf)
                stub->args.fstat_cbk.buf = *buf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


/* truncate */

call_stub_t *
fop_truncate_stub (call_frame_t *frame,
                   fop_truncate_t fn,
                   loc_t *loc,
                   off_t off, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_TRUNCATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.truncate.fn = fn;
        loc_copy (&stub->args.truncate.loc, loc);
        stub->args.truncate.off = off;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_truncate_cbk_stub (call_frame_t *frame,
                       fop_truncate_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       struct iatt *prebuf,
                       struct iatt *postbuf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_TRUNCATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.truncate_cbk.fn = fn;
        stub->args.truncate_cbk.op_ret = op_ret;
        stub->args.truncate_cbk.op_errno = op_errno;
        if (prebuf)
                stub->args.truncate_cbk.prebuf = *prebuf;
        if (postbuf)
                stub->args.truncate_cbk.postbuf = *postbuf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_ftruncate_stub (call_frame_t *frame,
                    fop_ftruncate_t fn,
                    fd_t *fd,
                    off_t off, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FTRUNCATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.ftruncate.fn = fn;
        if (fd)
                stub->args.ftruncate.fd = fd_ref (fd);

        stub->args.ftruncate.off = off;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_ftruncate_cbk_stub (call_frame_t *frame,
                        fop_ftruncate_cbk_t fn,
                        int32_t op_ret,
                        int32_t op_errno,
                        struct iatt *prebuf,
                        struct iatt *postbuf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FTRUNCATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.ftruncate_cbk.fn = fn;
        stub->args.ftruncate_cbk.op_ret = op_ret;
        stub->args.ftruncate_cbk.op_errno = op_errno;
        if (prebuf)
                stub->args.ftruncate_cbk.prebuf = *prebuf;
        if (postbuf)
                stub->args.ftruncate_cbk.postbuf = *postbuf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_access_stub (call_frame_t *frame,
                 fop_access_t fn,
                 loc_t *loc,
                 int32_t mask, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_ACCESS);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.access.fn = fn;
        loc_copy (&stub->args.access.loc, loc);
        stub->args.access.mask = mask;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_access_cbk_stub (call_frame_t *frame,
                     fop_access_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_ACCESS);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.access_cbk.fn = fn;
        stub->args.access_cbk.op_ret = op_ret;
        stub->args.access_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readlink_stub (call_frame_t *frame,
                   fop_readlink_t fn,
                   loc_t *loc,
                   size_t size, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_READLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readlink.fn = fn;
        loc_copy (&stub->args.readlink.loc, loc);
        stub->args.readlink.size = size;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readlink_cbk_stub (call_frame_t *frame,
                       fop_readlink_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       const char *path,
                       struct iatt *sbuf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_READLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readlink_cbk.fn = fn;
        stub->args.readlink_cbk.op_ret = op_ret;
        stub->args.readlink_cbk.op_errno = op_errno;
        if (path)
                stub->args.readlink_cbk.buf = gf_strdup (path);
        if (sbuf)
                stub->args.readlink_cbk.sbuf = *sbuf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_mknod_stub (call_frame_t *frame, fop_mknod_t fn, loc_t *loc, mode_t mode,
                dev_t rdev, mode_t umask, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_MKNOD);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.mknod.fn = fn;
        loc_copy (&stub->args.mknod.loc, loc);
        stub->args.mknod.mode = mode;
        stub->args.mknod.rdev = rdev;
        stub->args.mknod.umask = umask;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_mknod_cbk_stub (call_frame_t *frame,
                    fop_mknod_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    inode_t *inode,
                    struct iatt *buf,
                    struct iatt *preparent,
                    struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_MKNOD);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.mknod_cbk.fn = fn;
        stub->args.mknod_cbk.op_ret = op_ret;
        stub->args.mknod_cbk.op_errno = op_errno;
        if (inode)
                stub->args.mknod_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.mknod_cbk.buf = *buf;
        if (preparent)
                stub->args.mknod_cbk.preparent = *preparent;
        if (postparent)
                stub->args.mknod_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_mkdir_stub (call_frame_t *frame, fop_mkdir_t fn,
                loc_t *loc, mode_t mode, mode_t umask, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_MKDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.mkdir.fn = fn;
        loc_copy (&stub->args.mkdir.loc, loc);
        stub->args.mkdir.mode  = mode;
        stub->args.mkdir.umask = umask;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_mkdir_cbk_stub (call_frame_t *frame,
                    fop_mkdir_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    inode_t *inode,
                    struct iatt *buf,
                    struct iatt *preparent,
                    struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_MKDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.mkdir_cbk.fn = fn;
        stub->args.mkdir_cbk.op_ret = op_ret;
        stub->args.mkdir_cbk.op_errno = op_errno;
        if (inode)
                stub->args.mkdir_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.mkdir_cbk.buf = *buf;
        if (preparent)
                stub->args.mkdir_cbk.preparent = *preparent;
        if (postparent)
                stub->args.mkdir_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_unlink_stub (call_frame_t *frame,
                 fop_unlink_t fn,
                 loc_t *loc, int xflag, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_UNLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.unlink.fn = fn;
        loc_copy (&stub->args.unlink.loc, loc);
        stub->args.unlink.xflag = xflag;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_unlink_cbk_stub (call_frame_t *frame,
                     fop_unlink_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     struct iatt *preparent,
                     struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_UNLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.unlink_cbk.fn = fn;
        stub->args.unlink_cbk.op_ret = op_ret;
        stub->args.unlink_cbk.op_errno = op_errno;
        if (preparent)
                stub->args.unlink_cbk.preparent = *preparent;
        if (postparent)
                stub->args.unlink_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}



call_stub_t *
fop_rmdir_stub (call_frame_t *frame, fop_rmdir_t fn,
                loc_t *loc, int flags, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_RMDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rmdir.fn = fn;
        loc_copy (&stub->args.rmdir.loc, loc);
        stub->args.rmdir.flags = flags;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_rmdir_cbk_stub (call_frame_t *frame,
                    fop_rmdir_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    struct iatt *preparent,
                    struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_RMDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rmdir_cbk.fn = fn;
        stub->args.rmdir_cbk.op_ret = op_ret;
        stub->args.rmdir_cbk.op_errno = op_errno;
        if (preparent)
                stub->args.rmdir_cbk.preparent = *preparent;
        if (postparent)
                stub->args.rmdir_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_symlink_stub (call_frame_t *frame, fop_symlink_t fn,
                  const char *linkname, loc_t *loc, mode_t umask, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);
        GF_VALIDATE_OR_GOTO ("call-stub", linkname, out);

        stub = stub_new (frame, 1, GF_FOP_SYMLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.symlink.fn = fn;
        stub->args.symlink.linkname = gf_strdup (linkname);
        stub->args.symlink.umask = umask;
        loc_copy (&stub->args.symlink.loc, loc);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_symlink_cbk_stub (call_frame_t *frame,
                      fop_symlink_cbk_t fn,
                      int32_t op_ret,
                      int32_t op_errno,
                      inode_t *inode,
                      struct iatt *buf,
                      struct iatt *preparent,
                      struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_SYMLINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.symlink_cbk.fn = fn;
        stub->args.symlink_cbk.op_ret = op_ret;
        stub->args.symlink_cbk.op_errno = op_errno;
        if (inode)
                stub->args.symlink_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.symlink_cbk.buf = *buf;
        if (preparent)
                stub->args.symlink_cbk.preparent = *preparent;
        if (postparent)
                stub->args.symlink_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_rename_stub (call_frame_t *frame,
                 fop_rename_t fn,
                 loc_t *oldloc,
                 loc_t *newloc, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", oldloc, out);
        GF_VALIDATE_OR_GOTO ("call-stub", newloc, out);

        stub = stub_new (frame, 1, GF_FOP_RENAME);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rename.fn = fn;
        loc_copy (&stub->args.rename.old, oldloc);
        loc_copy (&stub->args.rename.new, newloc);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_rename_cbk_stub (call_frame_t *frame,
                     fop_rename_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     struct iatt *buf,
                     struct iatt *preoldparent,
                     struct iatt *postoldparent,
                     struct iatt *prenewparent,
                     struct iatt *postnewparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_RENAME);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rename_cbk.fn = fn;
        stub->args.rename_cbk.op_ret = op_ret;
        stub->args.rename_cbk.op_errno = op_errno;
        if (buf)
                stub->args.rename_cbk.buf = *buf;
        if (preoldparent)
                stub->args.rename_cbk.preoldparent = *preoldparent;
        if (postoldparent)
                stub->args.rename_cbk.postoldparent = *postoldparent;
        if (prenewparent)
                stub->args.rename_cbk.prenewparent = *prenewparent;
        if (postnewparent)
                stub->args.rename_cbk.postnewparent = *postnewparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_link_stub (call_frame_t *frame,
               fop_link_t fn,
               loc_t *oldloc,
               loc_t *newloc, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", oldloc, out);
        GF_VALIDATE_OR_GOTO ("call-stub", newloc, out);

        stub = stub_new (frame, 1, GF_FOP_LINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.link.fn = fn;
        loc_copy (&stub->args.link.oldloc, oldloc);
        loc_copy (&stub->args.link.newloc, newloc);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_link_cbk_stub (call_frame_t *frame,
                   fop_link_cbk_t fn,
                   int32_t op_ret,
                   int32_t op_errno,
                   inode_t *inode,
                   struct iatt *buf,
                   struct iatt *preparent,
                   struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_LINK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.link_cbk.fn = fn;
        stub->args.link_cbk.op_ret = op_ret;
        stub->args.link_cbk.op_errno = op_errno;
        if (inode)
                stub->args.link_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.link_cbk.buf = *buf;
        if (preparent)
                stub->args.link_cbk.preparent = *preparent;
        if (postparent)
                stub->args.link_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_create_stub (call_frame_t *frame, fop_create_t fn,
                 loc_t *loc, int32_t flags, mode_t mode,
                 mode_t umask, fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_CREATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.create.fn = fn;
        loc_copy (&stub->args.create.loc, loc);
        stub->args.create.flags = flags;
        stub->args.create.mode = mode;
        stub->args.create.umask = umask;
        if (fd)
                stub->args.create.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_create_cbk_stub (call_frame_t *frame,
                     fop_create_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     fd_t *fd,
                     inode_t *inode,
                     struct iatt *buf,
                     struct iatt *preparent,
                     struct iatt *postparent, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_CREATE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.create_cbk.fn = fn;
        stub->args.create_cbk.op_ret = op_ret;
        stub->args.create_cbk.op_errno = op_errno;
        if (fd)
                stub->args.create_cbk.fd = fd_ref (fd);
        if (inode)
                stub->args.create_cbk.inode = inode_ref (inode);
        if (buf)
                stub->args.create_cbk.buf = *buf;
        if (preparent)
                stub->args.create_cbk.preparent = *preparent;
        if (postparent)
                stub->args.create_cbk.postparent = *postparent;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_open_stub (call_frame_t *frame,
               fop_open_t fn,
               loc_t *loc,
               int32_t flags, fd_t *fd,
               dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_OPEN);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.open.fn = fn;
        loc_copy (&stub->args.open.loc, loc);
        stub->args.open.flags = flags;
        if (fd)
                stub->args.open.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_open_cbk_stub (call_frame_t *frame,
                   fop_open_cbk_t fn,
                   int32_t op_ret,
                   int32_t op_errno,
                   fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_OPEN);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.open_cbk.fn = fn;
        stub->args.open_cbk.op_ret = op_ret;
        stub->args.open_cbk.op_errno = op_errno;
        if (fd)
                stub->args.open_cbk.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readv_stub (call_frame_t *frame,
                fop_readv_t fn,
                fd_t *fd,
                size_t size,
                off_t off, uint32_t flags, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_READ);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readv.fn = fn;
        if (fd)
                stub->args.readv.fd = fd_ref (fd);
        stub->args.readv.size  = size;
        stub->args.readv.off   = off;
        stub->args.readv.flags = flags;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readv_cbk_stub (call_frame_t *frame,
                    fop_readv_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    struct iovec *vector,
                    int32_t count,
                    struct iatt *stbuf,
                    struct iobref *iobref, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_READ);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readv_cbk.fn = fn;
        stub->args.readv_cbk.op_ret = op_ret;
        stub->args.readv_cbk.op_errno = op_errno;
        if (op_ret >= 0) {
                stub->args.readv_cbk.vector = iov_dup (vector, count);
                stub->args.readv_cbk.count = count;
                stub->args.readv_cbk.stbuf = *stbuf;
                stub->args.readv_cbk.iobref = iobref_ref (iobref);
        }
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_writev_stub (call_frame_t *frame,
                 fop_writev_t fn,
                 fd_t *fd,
                 struct iovec *vector,
                 int32_t count,
                 off_t off, uint32_t flags,
                 struct iobref *iobref, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", vector, out);

        stub = stub_new (frame, 1, GF_FOP_WRITE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.writev.fn = fn;
        if (fd)
                stub->args.writev.fd = fd_ref (fd);
        stub->args.writev.vector = iov_dup (vector, count);
        stub->args.writev.count  = count;
        stub->args.writev.off    = off;
        stub->args.writev.flags  = flags;
        stub->args.writev.iobref = iobref_ref (iobref);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_writev_cbk_stub (call_frame_t *frame,
                     fop_writev_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     struct iatt *prebuf,
                     struct iatt *postbuf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_WRITE);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.writev_cbk.fn = fn;
        stub->args.writev_cbk.op_ret = op_ret;
        stub->args.writev_cbk.op_errno = op_errno;
        if (op_ret >= 0)
                stub->args.writev_cbk.postbuf = *postbuf;
        if (prebuf)
                stub->args.writev_cbk.prebuf = *prebuf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}



call_stub_t *
fop_flush_stub (call_frame_t *frame,
                fop_flush_t fn,
                fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FLUSH);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.flush.fn = fn;
        if (fd)
                stub->args.flush.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_flush_cbk_stub (call_frame_t *frame,
                    fop_flush_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FLUSH);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.flush_cbk.fn = fn;
        stub->args.flush_cbk.op_ret = op_ret;
        stub->args.flush_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}




call_stub_t *
fop_fsync_stub (call_frame_t *frame,
                fop_fsync_t fn,
                fd_t *fd,
                int32_t datasync, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FSYNC);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsync.fn = fn;
        if (fd)
                stub->args.fsync.fd = fd_ref (fd);
        stub->args.fsync.datasync = datasync;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fsync_cbk_stub (call_frame_t *frame,
                    fop_fsync_cbk_t fn,
                    int32_t op_ret,
                    int32_t op_errno,
                    struct iatt *prebuf,
                    struct iatt *postbuf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FSYNC);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsync_cbk.fn = fn;
        stub->args.fsync_cbk.op_ret = op_ret;
        stub->args.fsync_cbk.op_errno = op_errno;
        if (prebuf)
                stub->args.fsync_cbk.prebuf = *prebuf;
        if (postbuf)
                stub->args.fsync_cbk.postbuf = *postbuf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_opendir_stub (call_frame_t *frame,
                  fop_opendir_t fn,
                  loc_t *loc, fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_OPENDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.opendir.fn = fn;
        loc_copy (&stub->args.opendir.loc, loc);
        if (fd)
                stub->args.opendir.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_opendir_cbk_stub (call_frame_t *frame,
                      fop_opendir_cbk_t fn,
                      int32_t op_ret,
                      int32_t op_errno,
                      fd_t *fd, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_OPENDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.opendir_cbk.fn = fn;
        stub->args.opendir_cbk.op_ret = op_ret;
        stub->args.opendir_cbk.op_errno = op_errno;

        if (fd)
                stub->args.opendir_cbk.fd = fd_ref (fd);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fsyncdir_stub (call_frame_t *frame,
                   fop_fsyncdir_t fn,
                   fd_t *fd,
                   int32_t datasync, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FSYNCDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsyncdir.fn = fn;
        if (fd)
                stub->args.fsyncdir.fd = fd_ref (fd);
        stub->args.fsyncdir.datasync = datasync;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fsyncdir_cbk_stub (call_frame_t *frame,
                       fop_fsyncdir_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FSYNCDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsyncdir_cbk.fn = fn;
        stub->args.fsyncdir_cbk.op_ret = op_ret;
        stub->args.fsyncdir_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_statfs_stub (call_frame_t *frame,
                 fop_statfs_t fn,
                 loc_t *loc, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_STATFS);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.statfs.fn = fn;
        loc_copy (&stub->args.statfs.loc, loc);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_statfs_cbk_stub (call_frame_t *frame,
                     fop_statfs_cbk_t fn,
                     int32_t op_ret,
                     int32_t op_errno,
                     struct statvfs *buf, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_STATFS);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.statfs_cbk.fn = fn;
        stub->args.statfs_cbk.op_ret = op_ret;
        stub->args.statfs_cbk.op_errno = op_errno;
        if (op_ret == 0)
                stub->args.statfs_cbk.buf = *buf;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_setxattr_stub (call_frame_t *frame,
                   fop_setxattr_t fn,
                   loc_t *loc,
                   dict_t *dict,
                   int32_t flags, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_SETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.setxattr.fn = fn;
        loc_copy (&stub->args.setxattr.loc, loc);
        /* TODO */
        if (dict)
                stub->args.setxattr.dict = dict_ref (dict);
        stub->args.setxattr.flags = flags;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_setxattr_cbk_stub (call_frame_t *frame,
                       fop_setxattr_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_SETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.setxattr_cbk.fn = fn;
        stub->args.setxattr_cbk.op_ret = op_ret;
        stub->args.setxattr_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_getxattr_stub (call_frame_t *frame,
                   fop_getxattr_t fn,
                   loc_t *loc,
                   const char *name, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);

        stub = stub_new (frame, 1, GF_FOP_GETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.getxattr.fn = fn;
        loc_copy (&stub->args.getxattr.loc, loc);

        if (name)
                stub->args.getxattr.name = gf_strdup (name);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_getxattr_cbk_stub (call_frame_t *frame,
                       fop_getxattr_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       dict_t *dict, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_GETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.getxattr_cbk.fn = fn;
        stub->args.getxattr_cbk.op_ret = op_ret;
        stub->args.getxattr_cbk.op_errno = op_errno;
        /* TODO */
        if (dict)
                stub->args.getxattr_cbk.dict = dict_ref (dict);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fsetxattr_stub (call_frame_t *frame,
                    fop_fsetxattr_t fn,
                    fd_t *fd,
                    dict_t *dict,
                    int32_t flags, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fd, out);

        stub = stub_new (frame, 1, GF_FOP_FSETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsetxattr.fn = fn;
        stub->args.fsetxattr.fd = fd_ref (fd);

        /* TODO */
        if (dict)
                stub->args.fsetxattr.dict = dict_ref (dict);
        stub->args.fsetxattr.flags = flags;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fsetxattr_cbk_stub (call_frame_t *frame,
                        fop_fsetxattr_cbk_t fn,
                        int32_t op_ret,
                        int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FSETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsetxattr_cbk.fn = fn;
        stub->args.fsetxattr_cbk.op_ret = op_ret;
        stub->args.fsetxattr_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fgetxattr_stub (call_frame_t *frame,
                    fop_fgetxattr_t fn,
                    fd_t *fd,
                    const char *name, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fd, out);

        stub = stub_new (frame, 1, GF_FOP_FGETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fgetxattr.fn = fn;
        stub->args.fgetxattr.fd = fd_ref (fd);

        if (name)
                stub->args.fgetxattr.name = gf_strdup (name);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fgetxattr_cbk_stub (call_frame_t *frame,
                        fop_fgetxattr_cbk_t fn,
                        int32_t op_ret,
                        int32_t op_errno,
                        dict_t *dict, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_GETXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fgetxattr_cbk.fn = fn;
        stub->args.fgetxattr_cbk.op_ret = op_ret;
        stub->args.fgetxattr_cbk.op_errno = op_errno;

        /* TODO */
        if (dict)
                stub->args.fgetxattr_cbk.dict = dict_ref (dict);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_removexattr_stub (call_frame_t *frame,
                      fop_removexattr_t fn,
                      loc_t *loc,
                      const char *name, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", loc, out);
        GF_VALIDATE_OR_GOTO ("call-stub", name, out);

        stub = stub_new (frame, 1, GF_FOP_REMOVEXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.removexattr.fn = fn;
        loc_copy (&stub->args.removexattr.loc, loc);
        stub->args.removexattr.name = gf_strdup (name);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_removexattr_cbk_stub (call_frame_t *frame,
                          fop_removexattr_cbk_t fn,
                          int32_t op_ret,
                          int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_REMOVEXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.removexattr_cbk.fn = fn;
        stub->args.removexattr_cbk.op_ret = op_ret;
        stub->args.removexattr_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_fremovexattr_stub (call_frame_t *frame,
                       fop_fremovexattr_t fn,
                       fd_t *fd,
                       const char *name, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fd, out);
        GF_VALIDATE_OR_GOTO ("call-stub", name, out);

        stub = stub_new (frame, 1, GF_FOP_FREMOVEXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fremovexattr.fn = fn;
        stub->args.fremovexattr.fd = fd_ref (fd);
        stub->args.fremovexattr.name = gf_strdup (name);
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fremovexattr_cbk_stub (call_frame_t *frame,
                           fop_fremovexattr_cbk_t fn,
                           int32_t op_ret,
                           int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FREMOVEXATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fremovexattr_cbk.fn = fn;
        stub->args.fremovexattr_cbk.op_ret = op_ret;
        stub->args.fremovexattr_cbk.op_errno = op_errno;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_lk_stub (call_frame_t *frame,
             fop_lk_t fn,
             fd_t *fd,
             int32_t cmd,
             struct gf_flock *lock, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", lock, out);

        stub = stub_new (frame, 1, GF_FOP_LK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.lk.fn = fn;
        if (fd)
                stub->args.lk.fd = fd_ref (fd);
        stub->args.lk.cmd = cmd;
        stub->args.lk.lock = *lock;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_lk_cbk_stub (call_frame_t *frame,
                 fop_lk_cbk_t fn,
                 int32_t op_ret,
                 int32_t op_errno,
                 struct gf_flock *lock, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_LK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.lk_cbk.fn = fn;
        stub->args.lk_cbk.op_ret = op_ret;
        stub->args.lk_cbk.op_errno = op_errno;
        if (op_ret == 0)
                stub->args.lk_cbk.lock = *lock;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_inodelk_stub (call_frame_t *frame, fop_inodelk_t fn,
                  const char *volume, loc_t *loc, int32_t cmd,
                  struct gf_flock *lock, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", lock, out);

        stub = stub_new (frame, 1, GF_FOP_INODELK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.inodelk.fn = fn;

        if (volume)
                stub->args.inodelk.volume = gf_strdup (volume);

        loc_copy (&stub->args.inodelk.loc, loc);
        stub->args.inodelk.cmd  = cmd;
        stub->args.inodelk.lock = *lock;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_inodelk_cbk_stub (call_frame_t *frame, fop_inodelk_cbk_t fn,
                      int32_t op_ret, int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_INODELK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.inodelk_cbk.fn       = fn;
        stub->args.inodelk_cbk.op_ret   = op_ret;
        stub->args.inodelk_cbk.op_errno = op_errno;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_finodelk_stub (call_frame_t *frame, fop_finodelk_t fn,
                   const char *volume, fd_t *fd, int32_t cmd,
                   struct gf_flock *lock, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", lock, out);

        stub = stub_new (frame, 1, GF_FOP_FINODELK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.finodelk.fn = fn;

        if (fd)
                stub->args.finodelk.fd   = fd_ref (fd);

        if (volume)
                stub->args.finodelk.volume = gf_strdup (volume);

        stub->args.finodelk.cmd  = cmd;
        stub->args.finodelk.lock = *lock;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_finodelk_cbk_stub (call_frame_t *frame, fop_inodelk_cbk_t fn,
                       int32_t op_ret, int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FINODELK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.finodelk_cbk.fn       = fn;
        stub->args.finodelk_cbk.op_ret   = op_ret;
        stub->args.finodelk_cbk.op_errno = op_errno;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_entrylk_stub (call_frame_t *frame, fop_entrylk_t fn,
                  const char *volume, loc_t *loc, const char *name,
                  entrylk_cmd cmd, entrylk_type type, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_ENTRYLK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.entrylk.fn = fn;

        if (volume)
                stub->args.entrylk.volume = gf_strdup (volume);

        loc_copy (&stub->args.entrylk.loc, loc);

        stub->args.entrylk.cmd = cmd;
        stub->args.entrylk.type = type;
        if (name)
                stub->args.entrylk.name = gf_strdup (name);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_entrylk_cbk_stub (call_frame_t *frame, fop_entrylk_cbk_t fn,
                      int32_t op_ret, int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_ENTRYLK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.entrylk_cbk.fn       = fn;
        stub->args.entrylk_cbk.op_ret   = op_ret;
        stub->args.entrylk_cbk.op_errno = op_errno;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fentrylk_stub (call_frame_t *frame, fop_fentrylk_t fn,
                   const char *volume, fd_t *fd, const char *name,
                   entrylk_cmd cmd, entrylk_type type, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 1, GF_FOP_FENTRYLK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fentrylk.fn = fn;

        if (volume)
                stub->args.fentrylk.volume = gf_strdup (volume);

        if (fd)
                stub->args.fentrylk.fd = fd_ref (fd);
        stub->args.fentrylk.cmd = cmd;
        stub->args.fentrylk.type = type;
        if (name)
                stub->args.fentrylk.name = gf_strdup (name);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_fentrylk_cbk_stub (call_frame_t *frame, fop_fentrylk_cbk_t fn,
                       int32_t op_ret, int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FENTRYLK);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fentrylk_cbk.fn       = fn;
        stub->args.fentrylk_cbk.op_ret   = op_ret;
        stub->args.fentrylk_cbk.op_errno = op_errno;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readdirp_cbk_stub (call_frame_t *frame,
                       fop_readdirp_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       gf_dirent_t *entries, dict_t *xdata)
{
        call_stub_t *stub = NULL;
        gf_dirent_t *stub_entry = NULL, *entry = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_READDIRP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readdirp_cbk.fn = fn;
        stub->args.readdirp_cbk.op_ret = op_ret;
        stub->args.readdirp_cbk.op_errno = op_errno;
        INIT_LIST_HEAD (&stub->args.readdirp_cbk.entries.list);

        /* This check must come after the init of head above
         * so we're sure the list is empty for list_empty.
         */
        GF_VALIDATE_OR_GOTO ("call-stub", entries, out);

        if (op_ret > 0) {
                list_for_each_entry (entry, &entries->list, list) {
                        stub_entry = gf_dirent_for_name (entry->d_name);
                        if (!stub_entry)
                                goto out;
                        stub_entry->d_off = entry->d_off;
                        stub_entry->d_ino = entry->d_ino;
                        stub_entry->d_stat = entry->d_stat;
                        list_add_tail (&stub_entry->list,
                                       &stub->args.readdirp_cbk.entries.list);
                }
        }
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_readdir_cbk_stub (call_frame_t *frame,
                      fop_readdir_cbk_t fn,
                      int32_t op_ret,
                      int32_t op_errno,
                      gf_dirent_t *entries, dict_t *xdata)
{
        call_stub_t *stub = NULL;
        gf_dirent_t *stub_entry = NULL, *entry = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_READDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readdir_cbk.fn = fn;
        stub->args.readdir_cbk.op_ret = op_ret;
        stub->args.readdir_cbk.op_errno = op_errno;
        INIT_LIST_HEAD (&stub->args.readdir_cbk.entries.list);

        /* This check must come after the init of head above
         * so we're sure the list is empty for list_empty.
         */
        GF_VALIDATE_OR_GOTO ("call-stub", entries, out);

        if (op_ret > 0) {
                list_for_each_entry (entry, &entries->list, list) {
                        stub_entry = gf_dirent_for_name (entry->d_name);
                        if (!stub_entry)
                                goto out;
                        stub_entry->d_off = entry->d_off;
                        stub_entry->d_ino = entry->d_ino;

                        list_add_tail (&stub_entry->list,
                                       &stub->args.readdir_cbk.entries.list);
                }
        }
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_readdir_stub (call_frame_t *frame,
                  fop_readdir_t fn,
                  fd_t *fd,
                  size_t size,
                  off_t off, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        stub = stub_new (frame, 1, GF_FOP_READDIR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readdir.fn = fn;
        stub->args.readdir.fd = fd_ref (fd);
        stub->args.readdir.size = size;
        stub->args.readdir.off = off;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_readdirp_stub (call_frame_t *frame,
                   fop_readdirp_t fn,
                   fd_t *fd,
                   size_t size,
                   off_t off,
                   dict_t *xdata)
{
        call_stub_t *stub = NULL;

        stub = stub_new (frame, 1, GF_FOP_READDIRP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.readdirp.fn = fn;
        stub->args.readdirp.fd = fd_ref (fd);
        stub->args.readdirp.size = size;
        stub->args.readdirp.off = off;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_rchecksum_stub (call_frame_t *frame,
                    fop_rchecksum_t fn,
                    fd_t *fd, off_t offset,
                    int32_t len, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fd, out);

        stub = stub_new (frame, 1, GF_FOP_RCHECKSUM);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rchecksum.fn = fn;
        stub->args.rchecksum.fd = fd_ref (fd);
        stub->args.rchecksum.offset = offset;
        stub->args.rchecksum.len    = len;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_rchecksum_cbk_stub (call_frame_t *frame,
                        fop_rchecksum_cbk_t fn,
                        int32_t op_ret,
                        int32_t op_errno,
                        uint32_t weak_checksum,
                        uint8_t *strong_checksum, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_RCHECKSUM);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.rchecksum_cbk.fn = fn;
        stub->args.rchecksum_cbk.op_ret = op_ret;
        stub->args.rchecksum_cbk.op_errno = op_errno;

        if (op_ret >= 0)
        {
                stub->args.rchecksum_cbk.weak_checksum =
                        weak_checksum;

                stub->args.rchecksum_cbk.strong_checksum =
                        memdup (strong_checksum, MD5_DIGEST_LENGTH);
        }
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_xattrop_cbk_stub (call_frame_t *frame,
                      fop_xattrop_cbk_t fn,
                      int32_t op_ret,
                      int32_t op_errno, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_XATTROP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.xattrop_cbk.fn       = fn;
        stub->args.xattrop_cbk.op_ret   = op_ret;
        stub->args.xattrop_cbk.op_errno = op_errno;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_fxattrop_cbk_stub (call_frame_t *frame,
                       fop_fxattrop_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       dict_t *xattr, dict_t *xdata)
{
        call_stub_t *stub = NULL;
        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FXATTROP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fxattrop_cbk.fn = fn;
        stub->args.fxattrop_cbk.op_ret = op_ret;
        stub->args.fxattrop_cbk.op_errno = op_errno;
        if (xattr)
                stub->args.fxattrop_cbk.xattr = dict_ref (xattr);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_xattrop_stub (call_frame_t *frame,
                  fop_xattrop_t fn,
                  loc_t *loc,
                  gf_xattrop_flags_t optype,
                  dict_t *xattr, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", xattr, out);

        stub = stub_new (frame, 1, GF_FOP_XATTROP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.xattrop.fn = fn;

        loc_copy (&stub->args.xattrop.loc, loc);

        stub->args.xattrop.optype = optype;
        stub->args.xattrop.xattr = dict_ref (xattr);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_fxattrop_stub (call_frame_t *frame,
                   fop_fxattrop_t fn,
                   fd_t *fd,
                   gf_xattrop_flags_t optype,
                   dict_t *xattr, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", xattr, out);

        stub = stub_new (frame, 1, GF_FOP_FXATTROP);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fxattrop.fn = fn;

        stub->args.fxattrop.fd = fd_ref (fd);

        stub->args.fxattrop.optype = optype;
        stub->args.fxattrop.xattr = dict_ref (xattr);

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}


call_stub_t *
fop_setattr_cbk_stub (call_frame_t *frame,
                      fop_setattr_cbk_t fn,
                      int32_t op_ret,
                      int32_t op_errno,
                      struct iatt *statpre,
                      struct iatt *statpost, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_SETATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.setattr_cbk.fn = fn;

        stub->args.setattr_cbk.op_ret = op_ret;
        stub->args.setattr_cbk.op_errno = op_errno;

        if (statpre)
                stub->args.setattr_cbk.statpre = *statpre;
        if (statpost)
                stub->args.setattr_cbk.statpost = *statpost;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_fsetattr_cbk_stub (call_frame_t *frame,
                       fop_setattr_cbk_t fn,
                       int32_t op_ret,
                       int32_t op_errno,
                       struct iatt *statpre,
                       struct iatt *statpost, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);

        stub = stub_new (frame, 0, GF_FOP_FSETATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsetattr_cbk.fn = fn;

        stub->args.fsetattr_cbk.op_ret = op_ret;
        stub->args.fsetattr_cbk.op_errno = op_errno;

        if (statpre)
                stub->args.setattr_cbk.statpre = *statpre;
        if (statpost)
                stub->args.fsetattr_cbk.statpost = *statpost;
        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_setattr_stub (call_frame_t *frame,
                  fop_setattr_t fn,
                  loc_t *loc,
                  struct iatt *stbuf,
                  int32_t valid, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fn, out);

        stub = stub_new (frame, 1, GF_FOP_SETATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.setattr.fn = fn;

        loc_copy (&stub->args.setattr.loc, loc);

        if (stbuf)
                stub->args.setattr.stbuf = *stbuf;

        stub->args.setattr.valid = valid;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

call_stub_t *
fop_fsetattr_stub (call_frame_t *frame,
                   fop_fsetattr_t fn,
                   fd_t *fd,
                   struct iatt *stbuf,
                   int32_t valid, dict_t *xdata)
{
        call_stub_t *stub = NULL;

        GF_VALIDATE_OR_GOTO ("call-stub", frame, out);
        GF_VALIDATE_OR_GOTO ("call-stub", fn, out);

        stub = stub_new (frame, 1, GF_FOP_FSETATTR);
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        stub->args.fsetattr.fn = fn;

        if (fd)
                stub->args.fsetattr.fd = fd_ref (fd);

        if (stbuf)
                stub->args.fsetattr.stbuf = *stbuf;

        stub->args.fsetattr.valid = valid;

        if (xdata)
                stub->xdata = dict_ref (xdata);

out:
        return stub;
}

static void
call_resume_wind (call_stub_t *stub)
{
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        switch (stub->fop) {
        case GF_FOP_OPEN:
        {
                stub->args.open.fn (stub->frame,
                                    stub->frame->this,
                                    &stub->args.open.loc,
                                    stub->args.open.flags, stub->args.open.fd,
                                    stub->xdata);
                break;
        }
        case GF_FOP_CREATE:
        {
                stub->args.create.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.create.loc,
                                      stub->args.create.flags,
                                      stub->args.create.mode,
                                      stub->args.create.umask,
                                      stub->args.create.fd,
                                      stub->xdata);
                break;
        }
        case GF_FOP_STAT:
        {
                stub->args.stat.fn (stub->frame,
                                    stub->frame->this,
                                    &stub->args.stat.loc, stub->xdata);
                break;
        }
        case GF_FOP_READLINK:
        {
                stub->args.readlink.fn (stub->frame,
                                        stub->frame->this,
                                        &stub->args.readlink.loc,
                                        stub->args.readlink.size, stub->xdata);
                break;
        }

        case GF_FOP_MKNOD:
        {
                stub->args.mknod.fn (stub->frame, stub->frame->this,
                                     &stub->args.mknod.loc,
                                     stub->args.mknod.mode,
                                     stub->args.mknod.rdev,
                                     stub->args.mknod.umask, stub->xdata);
        }
        break;

        case GF_FOP_MKDIR:
        {
                stub->args.mkdir.fn (stub->frame, stub->frame->this,
                                     &stub->args.mkdir.loc,
                                     stub->args.mkdir.mode,
                                     stub->args.mkdir.umask, stub->xdata);
        }
        break;

        case GF_FOP_UNLINK:
        {
                stub->args.unlink.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.unlink.loc,
                                      stub->args.unlink.xflag, stub->xdata);
        }
        break;

        case GF_FOP_RMDIR:
        {
                stub->args.rmdir.fn (stub->frame, stub->frame->this,
                                     &stub->args.rmdir.loc,
                                     stub->args.rmdir.flags, stub->xdata);
        }
        break;

        case GF_FOP_SYMLINK:
        {
                stub->args.symlink.fn (stub->frame,
                                       stub->frame->this,
                                       stub->args.symlink.linkname,
                                       &stub->args.symlink.loc,
                                       stub->args.symlink.umask, stub->xdata);
        }
        break;

        case GF_FOP_RENAME:
        {
                stub->args.rename.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.rename.old,
                                      &stub->args.rename.new, stub->xdata);
        }
        break;

        case GF_FOP_LINK:
        {
                stub->args.link.fn (stub->frame,
                                    stub->frame->this,
                                    &stub->args.link.oldloc,
                                    &stub->args.link.newloc, stub->xdata);
        }
        break;

        case GF_FOP_TRUNCATE:
        {
                stub->args.truncate.fn (stub->frame,
                                        stub->frame->this,
                                        &stub->args.truncate.loc,
                                        stub->args.truncate.off, stub->xdata);
                break;
        }

        case GF_FOP_READ:
        {
                stub->args.readv.fn (stub->frame,
                                     stub->frame->this,
                                     stub->args.readv.fd,
                                     stub->args.readv.size,
                                     stub->args.readv.off,
                                     stub->args.readv.flags, stub->xdata);
                break;
        }

        case GF_FOP_WRITE:
        {
                stub->args.writev.fn (stub->frame,
                                      stub->frame->this,
                                      stub->args.writev.fd,
                                      stub->args.writev.vector,
                                      stub->args.writev.count,
                                      stub->args.writev.off,
                                      stub->args.writev.flags,
                                      stub->args.writev.iobref, stub->xdata);
                break;
        }

        case GF_FOP_STATFS:
        {
                stub->args.statfs.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.statfs.loc, stub->xdata);
                break;
        }
        case GF_FOP_FLUSH:
        {
                stub->args.flush.fn (stub->frame,
                                     stub->frame->this,
                                     stub->args.flush.fd, stub->xdata);
                break;
        }

        case GF_FOP_FSYNC:
        {
                stub->args.fsync.fn (stub->frame,
                                     stub->frame->this,
                                     stub->args.fsync.fd,
                                     stub->args.fsync.datasync, stub->xdata);
                break;
        }

        case GF_FOP_SETXATTR:
        {
                stub->args.setxattr.fn (stub->frame,
                                        stub->frame->this,
                                        &stub->args.setxattr.loc,
                                        stub->args.setxattr.dict,
                                        stub->args.setxattr.flags, stub->xdata);
                break;
        }

        case GF_FOP_GETXATTR:
        {
                stub->args.getxattr.fn (stub->frame,
                                        stub->frame->this,
                                        &stub->args.getxattr.loc,
                                        stub->args.getxattr.name, stub->xdata);
                break;
        }

        case GF_FOP_FSETXATTR:
        {
                stub->args.fsetxattr.fn (stub->frame,
                                         stub->frame->this,
                                         stub->args.fsetxattr.fd,
                                         stub->args.fsetxattr.dict,
                                         stub->args.fsetxattr.flags, stub->xdata);
                break;
        }

        case GF_FOP_FGETXATTR:
        {
                stub->args.fgetxattr.fn (stub->frame,
                                         stub->frame->this,
                                         stub->args.fgetxattr.fd,
                                         stub->args.fgetxattr.name, stub->xdata);
                break;
        }

        case GF_FOP_REMOVEXATTR:
        {
                stub->args.removexattr.fn (stub->frame,
                                           stub->frame->this,
                                           &stub->args.removexattr.loc,
                                           stub->args.removexattr.name, stub->xdata);
                break;
        }

        case GF_FOP_FREMOVEXATTR:
        {
                stub->args.fremovexattr.fn (stub->frame,
                                            stub->frame->this,
                                            stub->args.fremovexattr.fd,
                                            stub->args.fremovexattr.name, stub->xdata);
                break;
        }

        case GF_FOP_OPENDIR:
        {
                stub->args.opendir.fn (stub->frame,
                                       stub->frame->this,
                                       &stub->args.opendir.loc,
                                       stub->args.opendir.fd, stub->xdata);
                break;
        }

        case GF_FOP_FSYNCDIR:
        {
                stub->args.fsyncdir.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.fsyncdir.fd,
                                        stub->args.fsyncdir.datasync, stub->xdata);
                break;
        }

        case GF_FOP_ACCESS:
        {
                stub->args.access.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.access.loc,
                                      stub->args.access.mask, stub->xdata);
                break;
        }

        case GF_FOP_FTRUNCATE:
        {
                stub->args.ftruncate.fn (stub->frame,
                                         stub->frame->this,
                                         stub->args.ftruncate.fd,
                                         stub->args.ftruncate.off, stub->xdata);
                break;
        }

        case GF_FOP_FSTAT:
        {
                stub->args.fstat.fn (stub->frame,
                                     stub->frame->this,
                                     stub->args.fstat.fd, stub->xdata);
                break;
        }

        case GF_FOP_LK:
        {
                stub->args.lk.fn (stub->frame,
                                  stub->frame->this,
                                  stub->args.lk.fd,
                                  stub->args.lk.cmd,
                                  &stub->args.lk.lock, stub->xdata);
                break;
        }

        case GF_FOP_INODELK:
        {
                stub->args.inodelk.fn (stub->frame,
                                       stub->frame->this,
                                       stub->args.inodelk.volume,
                                       &stub->args.inodelk.loc,
                                       stub->args.inodelk.cmd,
                                       &stub->args.inodelk.lock, stub->xdata);
                break;
        }

        case GF_FOP_FINODELK:
        {
                stub->args.finodelk.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.finodelk.volume,
                                        stub->args.finodelk.fd,
                                        stub->args.finodelk.cmd,
                                        &stub->args.finodelk.lock, stub->xdata);
                break;
        }

        case GF_FOP_ENTRYLK:
        {
                stub->args.entrylk.fn (stub->frame,
                                       stub->frame->this,
                                       stub->args.entrylk.volume,
                                       &stub->args.entrylk.loc,
                                       stub->args.entrylk.name,
                                       stub->args.entrylk.cmd,
                                       stub->args.entrylk.type, stub->xdata);
                break;
        }

        case GF_FOP_FENTRYLK:
        {
                stub->args.fentrylk.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.fentrylk.volume,
                                        stub->args.fentrylk.fd,
                                        stub->args.fentrylk.name,
                                        stub->args.fentrylk.cmd,
                                        stub->args.fentrylk.type, stub->xdata);
                break;
        }

        break;

        case GF_FOP_LOOKUP:
        {
                stub->args.lookup.fn (stub->frame,
                                      stub->frame->this,
                                      &stub->args.lookup.loc,
                                      stub->xdata);
                break;
        }

        case GF_FOP_RCHECKSUM:
        {
                stub->args.rchecksum.fn (stub->frame,
                                         stub->frame->this,
                                         stub->args.rchecksum.fd,
                                         stub->args.rchecksum.offset,
                                         stub->args.rchecksum.len, stub->xdata);
                break;
        }

        case GF_FOP_READDIR:
        {
                stub->args.readdir.fn (stub->frame,
                                       stub->frame->this,
                                       stub->args.readdir.fd,
                                       stub->args.readdir.size,
                                       stub->args.readdir.off, stub->xdata);
                break;
        }

        case GF_FOP_READDIRP:
        {
                stub->args.readdirp.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.readdirp.fd,
                                        stub->args.readdirp.size,
                                        stub->args.readdirp.off,
                                        stub->xdata);
                break;
        }

        case GF_FOP_XATTROP:
        {
                stub->args.xattrop.fn (stub->frame,
                                       stub->frame->this,
                                       &stub->args.xattrop.loc,
                                       stub->args.xattrop.optype,
                                       stub->args.xattrop.xattr, stub->xdata);

                break;
        }
        case GF_FOP_FXATTROP:
        {
                stub->args.fxattrop.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.fxattrop.fd,
                                        stub->args.fxattrop.optype,
                                        stub->args.fxattrop.xattr, stub->xdata);

                break;
        }
        case GF_FOP_SETATTR:
        {
                stub->args.setattr.fn (stub->frame,
                                       stub->frame->this,
                                       &stub->args.setattr.loc,
                                       &stub->args.setattr.stbuf,
                                       stub->args.setattr.valid, stub->xdata);
                break;
        }
        case GF_FOP_FSETATTR:
        {
                stub->args.fsetattr.fn (stub->frame,
                                        stub->frame->this,
                                        stub->args.fsetattr.fd,
                                        &stub->args.fsetattr.stbuf,
                                        stub->args.fsetattr.valid, stub->xdata);
                break;
        }
        default:
        {
                gf_log_callingfn ("call-stub", GF_LOG_ERROR,
                                  "Invalid value of FOP (%d)",
                                  stub->fop);
                break;
        }

        }
out:
        return;
}



static void
call_resume_unwind (call_stub_t *stub)
{
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        switch (stub->fop) {
        case GF_FOP_OPEN:
        {
                if (!stub->args.open_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.open_cbk.op_ret,
                                      stub->args.open_cbk.op_errno,
                                      stub->args.open_cbk.fd, stub->xdata);
                else
                        stub->args.open_cbk.fn (stub->frame,
                                                stub->frame->cookie,
                                                stub->frame->this,
                                                stub->args.open_cbk.op_ret,
                                                stub->args.open_cbk.op_errno,
                                                stub->args.open_cbk.fd, stub->xdata);
                break;
        }

        case GF_FOP_CREATE:
        {
                if (!stub->args.create_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.create_cbk.op_ret,
                                      stub->args.create_cbk.op_errno,
                                      stub->args.create_cbk.fd,
                                      stub->args.create_cbk.inode,
                                      &stub->args.create_cbk.buf,
                                      &stub->args.create_cbk.preparent,
                                      &stub->args.create_cbk.postparent, stub->xdata);
                else
                        stub->args.create_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.create_cbk.op_ret,
                                                  stub->args.create_cbk.op_errno,
                                                  stub->args.create_cbk.fd,
                                                  stub->args.create_cbk.inode,
                                                  &stub->args.create_cbk.buf,
                                                  &stub->args.create_cbk.preparent,
                                                  &stub->args.create_cbk.postparent, stub->xdata);

                break;
        }

        case GF_FOP_STAT:
        {
                if (!stub->args.stat_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.stat_cbk.op_ret,
                                      stub->args.stat_cbk.op_errno,
                                      &stub->args.stat_cbk.buf, stub->xdata);
                else
                        stub->args.stat_cbk.fn (stub->frame,
                                                stub->frame->cookie,
                                                stub->frame->this,
                                                stub->args.stat_cbk.op_ret,
                                                stub->args.stat_cbk.op_errno,
                                                &stub->args.stat_cbk.buf, stub->xdata);

                break;
        }

        case GF_FOP_READLINK:
        {
                if (!stub->args.readlink_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.readlink_cbk.op_ret,
                                      stub->args.readlink_cbk.op_errno,
                                      stub->args.readlink_cbk.buf,
                                      &stub->args.readlink_cbk.sbuf, stub->xdata);
                else
                        stub->args.readlink_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.readlink_cbk.op_ret,
                                                    stub->args.readlink_cbk.op_errno,
                                                    stub->args.readlink_cbk.buf,
                                                    &stub->args.readlink_cbk.sbuf, stub->xdata);

                break;
        }

        case GF_FOP_MKNOD:
        {
                if (!stub->args.mknod_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.mknod_cbk.op_ret,
                                      stub->args.mknod_cbk.op_errno,
                                      stub->args.mknod_cbk.inode,
                                      &stub->args.mknod_cbk.buf,
                                      &stub->args.mknod_cbk.preparent,
                                      &stub->args.mknod_cbk.postparent, stub->xdata);
                else
                        stub->args.mknod_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.mknod_cbk.op_ret,
                                                 stub->args.mknod_cbk.op_errno,
                                                 stub->args.mknod_cbk.inode,
                                                 &stub->args.mknod_cbk.buf,
                                                 &stub->args.mknod_cbk.preparent,
                                                 &stub->args.mknod_cbk.postparent, stub->xdata);
                break;
        }

        case GF_FOP_MKDIR:
        {
                if (!stub->args.mkdir_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.mkdir_cbk.op_ret,
                                      stub->args.mkdir_cbk.op_errno,
                                      stub->args.mkdir_cbk.inode,
                                      &stub->args.mkdir_cbk.buf,
                                      &stub->args.mkdir_cbk.preparent,
                                      &stub->args.mkdir_cbk.postparent, stub->xdata);
                else
                        stub->args.mkdir_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.mkdir_cbk.op_ret,
                                                 stub->args.mkdir_cbk.op_errno,
                                                 stub->args.mkdir_cbk.inode,
                                                 &stub->args.mkdir_cbk.buf,
                                                 &stub->args.mkdir_cbk.preparent,
                                                 &stub->args.mkdir_cbk.postparent, stub->xdata);

                if (stub->args.mkdir_cbk.inode)
                        inode_unref (stub->args.mkdir_cbk.inode);

                break;
        }

        case GF_FOP_UNLINK:
        {
                if (!stub->args.unlink_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.unlink_cbk.op_ret,
                                      stub->args.unlink_cbk.op_errno,
                                      &stub->args.unlink_cbk.preparent,
                                      &stub->args.unlink_cbk.postparent, stub->xdata);
                else
                        stub->args.unlink_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.unlink_cbk.op_ret,
                                                  stub->args.unlink_cbk.op_errno,
                                                  &stub->args.unlink_cbk.preparent,
                                                  &stub->args.unlink_cbk.postparent, stub->xdata);
                break;
        }

        case GF_FOP_RMDIR:
        {
                if (!stub->args.rmdir_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.rmdir_cbk.op_ret,
                                      stub->args.rmdir_cbk.op_errno,
                                      &stub->args.rmdir_cbk.preparent,
                                      &stub->args.rmdir_cbk.postparent, stub->xdata);
                else
                        stub->args.rmdir_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.rmdir_cbk.op_ret,
                                                 stub->args.rmdir_cbk.op_errno,
                                                 &stub->args.rmdir_cbk.preparent,
                                                 &stub->args.rmdir_cbk.postparent, stub->xdata);
                break;
        }

        case GF_FOP_SYMLINK:
        {
                if (!stub->args.symlink_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.symlink_cbk.op_ret,
                                      stub->args.symlink_cbk.op_errno,
                                      stub->args.symlink_cbk.inode,
                                      &stub->args.symlink_cbk.buf,
                                      &stub->args.symlink_cbk.preparent,
                                      &stub->args.symlink_cbk.postparent, stub->xdata);
                else
                        stub->args.symlink_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.symlink_cbk.op_ret,
                                                   stub->args.symlink_cbk.op_errno,
                                                   stub->args.symlink_cbk.inode,
                                                   &stub->args.symlink_cbk.buf,
                                                   &stub->args.symlink_cbk.preparent,
                                                   &stub->args.symlink_cbk.postparent, stub->xdata);
        }
        break;

        case GF_FOP_RENAME:
        {
                if (!stub->args.rename_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.rename_cbk.op_ret,
                                      stub->args.rename_cbk.op_errno,
                                      &stub->args.rename_cbk.buf,
                                      &stub->args.rename_cbk.preoldparent,
                                      &stub->args.rename_cbk.postoldparent,
                                      &stub->args.rename_cbk.prenewparent,
                                      &stub->args.rename_cbk.postnewparent, stub->xdata);
                else
                        stub->args.rename_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.rename_cbk.op_ret,
                                                  stub->args.rename_cbk.op_errno,
                                                  &stub->args.rename_cbk.buf,
                                                  &stub->args.rename_cbk.preoldparent,
                                                  &stub->args.rename_cbk.postoldparent,
                                                  &stub->args.rename_cbk.prenewparent,
                                                  &stub->args.rename_cbk.postnewparent, stub->xdata);
                break;
        }

        case GF_FOP_LINK:
        {
                if (!stub->args.link_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.link_cbk.op_ret,
                                      stub->args.link_cbk.op_errno,
                                      stub->args.link_cbk.inode,
                                      &stub->args.link_cbk.buf, stub->xdata);
                else
                        stub->args.link_cbk.fn (stub->frame,
                                                stub->frame->cookie,
                                                stub->frame->this,
                                                stub->args.link_cbk.op_ret,
                                                stub->args.link_cbk.op_errno,
                                                stub->args.link_cbk.inode,
                                                &stub->args.link_cbk.buf,
                                                &stub->args.link_cbk.preparent,
                                                &stub->args.link_cbk.postparent, stub->xdata);
                break;
        }

        case GF_FOP_TRUNCATE:
        {
                if (!stub->args.truncate_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.truncate_cbk.op_ret,
                                      stub->args.truncate_cbk.op_errno,
                                      &stub->args.truncate_cbk.prebuf,
                                      &stub->args.truncate_cbk.postbuf, stub->xdata);
                else
                        stub->args.truncate_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.truncate_cbk.op_ret,
                                                    stub->args.truncate_cbk.op_errno,
                                                    &stub->args.truncate_cbk.prebuf,
                                                    &stub->args.truncate_cbk.postbuf, stub->xdata);
                break;
        }

        case GF_FOP_READ:
        {
                if (!stub->args.readv_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.readv_cbk.op_ret,
                                      stub->args.readv_cbk.op_errno,
                                      stub->args.readv_cbk.vector,
                                      stub->args.readv_cbk.count,
                                      &stub->args.readv_cbk.stbuf,
                                      stub->args.readv_cbk.iobref, stub->xdata);
                else
                        stub->args.readv_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.readv_cbk.op_ret,
                                                 stub->args.readv_cbk.op_errno,
                                                 stub->args.readv_cbk.vector,
                                                 stub->args.readv_cbk.count,
                                                 &stub->args.readv_cbk.stbuf,
                                                 stub->args.readv_cbk.iobref, stub->xdata);
        }
        break;

        case GF_FOP_WRITE:
        {
                if (!stub->args.writev_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.writev_cbk.op_ret,
                                      stub->args.writev_cbk.op_errno,
                                      &stub->args.writev_cbk.prebuf,
                                      &stub->args.writev_cbk.postbuf, stub->xdata);
                else
                        stub->args.writev_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.writev_cbk.op_ret,
                                                  stub->args.writev_cbk.op_errno,
                                                  &stub->args.writev_cbk.prebuf,
                                                  &stub->args.writev_cbk.postbuf, stub->xdata);
                break;
        }

        case GF_FOP_STATFS:
        {
                if (!stub->args.statfs_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.statfs_cbk.op_ret,
                                      stub->args.statfs_cbk.op_errno,
                                      &(stub->args.statfs_cbk.buf), stub->xdata);
                else
                        stub->args.statfs_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.statfs_cbk.op_ret,
                                                  stub->args.statfs_cbk.op_errno,
                                                  &(stub->args.statfs_cbk.buf), stub->xdata);
        }
        break;

        case GF_FOP_FLUSH:
        {
                if (!stub->args.flush_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.flush_cbk.op_ret,
                                      stub->args.flush_cbk.op_errno, stub->xdata);
                else
                        stub->args.flush_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.flush_cbk.op_ret,
                                                 stub->args.flush_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_FSYNC:
        {
                if (!stub->args.fsync_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fsync_cbk.op_ret,
                                      stub->args.fsync_cbk.op_errno,
                                      &stub->args.fsync_cbk.prebuf,
                                      &stub->args.fsync_cbk.postbuf, stub->xdata);
                else
                        stub->args.fsync_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.fsync_cbk.op_ret,
                                                 stub->args.fsync_cbk.op_errno,
                                                 &stub->args.fsync_cbk.prebuf,
                                                 &stub->args.fsync_cbk.postbuf, stub->xdata);
                break;
        }

        case GF_FOP_SETXATTR:
        {
                if (!stub->args.setxattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.setxattr_cbk.op_ret,
                                      stub->args.setxattr_cbk.op_errno, stub->xdata);

                else
                        stub->args.setxattr_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.setxattr_cbk.op_ret,
                                                    stub->args.setxattr_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_GETXATTR:
        {
                if (!stub->args.getxattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.getxattr_cbk.op_ret,
                                      stub->args.getxattr_cbk.op_errno,
                                      stub->args.getxattr_cbk.dict, stub->xdata);
                else
                        stub->args.getxattr_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.getxattr_cbk.op_ret,
                                                    stub->args.getxattr_cbk.op_errno,
                                                    stub->args.getxattr_cbk.dict, stub->xdata);
                break;
        }

        case GF_FOP_FSETXATTR:
        {
                if (!stub->args.fsetxattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fsetxattr_cbk.op_ret,
                                      stub->args.fsetxattr_cbk.op_errno, stub->xdata);

                else
                        stub->args.fsetxattr_cbk.fn (stub->frame,
                                                     stub->frame->cookie,
                                                     stub->frame->this,
                                                     stub->args.fsetxattr_cbk.op_ret,
                                                     stub->args.fsetxattr_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_FGETXATTR:
        {
                if (!stub->args.fgetxattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fgetxattr_cbk.op_ret,
                                      stub->args.fgetxattr_cbk.op_errno,
                                      stub->args.fgetxattr_cbk.dict, stub->xdata);
                else
                        stub->args.fgetxattr_cbk.fn (stub->frame,
                                                     stub->frame->cookie,
                                                     stub->frame->this,
                                                     stub->args.fgetxattr_cbk.op_ret,
                                                     stub->args.fgetxattr_cbk.op_errno,
                                                     stub->args.fgetxattr_cbk.dict, stub->xdata);
                break;
        }

        case GF_FOP_REMOVEXATTR:
        {
                if (!stub->args.removexattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.removexattr_cbk.op_ret,
                                      stub->args.removexattr_cbk.op_errno, stub->xdata);
                else
                        stub->args.removexattr_cbk.fn (stub->frame,
                                                       stub->frame->cookie,
                                                       stub->frame->this,
                                                       stub->args.removexattr_cbk.op_ret,
                                                       stub->args.removexattr_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_FREMOVEXATTR:
        {
                if (!stub->args.fremovexattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fremovexattr_cbk.op_ret,
                                      stub->args.fremovexattr_cbk.op_errno, stub->xdata);
                else
                        stub->args.fremovexattr_cbk.fn (stub->frame,
                                                        stub->frame->cookie,
                                                        stub->frame->this,
                                                        stub->args.fremovexattr_cbk.op_ret,
                                                        stub->args.fremovexattr_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_OPENDIR:
        {
                if (!stub->args.opendir_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.opendir_cbk.op_ret,
                                      stub->args.opendir_cbk.op_errno,
                                      stub->args.opendir_cbk.fd, stub->xdata);
                else
                        stub->args.opendir_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.opendir_cbk.op_ret,
                                                   stub->args.opendir_cbk.op_errno,
                                                   stub->args.opendir_cbk.fd, stub->xdata);
                break;
        }

        case GF_FOP_FSYNCDIR:
        {
                if (!stub->args.fsyncdir_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fsyncdir_cbk.op_ret,
                                      stub->args.fsyncdir_cbk.op_errno, stub->xdata);
                else
                        stub->args.fsyncdir_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.fsyncdir_cbk.op_ret,
                                                    stub->args.fsyncdir_cbk.op_errno, stub->xdata);
                break;
        }

        case GF_FOP_ACCESS:
        {
                if (!stub->args.access_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.access_cbk.op_ret,
                                      stub->args.access_cbk.op_errno, stub->xdata);
                else
                        stub->args.access_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.access_cbk.op_ret,
                                                  stub->args.access_cbk.op_errno, stub->xdata);

                break;
        }

        case GF_FOP_FTRUNCATE:
        {
                if (!stub->args.ftruncate_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.ftruncate_cbk.op_ret,
                                      stub->args.ftruncate_cbk.op_errno,
                                      &stub->args.ftruncate_cbk.prebuf,
                                      &stub->args.ftruncate_cbk.postbuf, stub->xdata);
                else
                        stub->args.ftruncate_cbk.fn (stub->frame,
                                                     stub->frame->cookie,
                                                     stub->frame->this,
                                                     stub->args.ftruncate_cbk.op_ret,
                                                     stub->args.ftruncate_cbk.op_errno,
                                                     &stub->args.ftruncate_cbk.prebuf,
                                                     &stub->args.ftruncate_cbk.postbuf, stub->xdata);
                break;
        }

        case GF_FOP_FSTAT:
        {
                if (!stub->args.fstat_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fstat_cbk.op_ret,
                                      stub->args.fstat_cbk.op_errno,
                                      &stub->args.fstat_cbk.buf, stub->xdata);
                else
                        stub->args.fstat_cbk.fn (stub->frame,
                                                 stub->frame->cookie,
                                                 stub->frame->this,
                                                 stub->args.fstat_cbk.op_ret,
                                                 stub->args.fstat_cbk.op_errno,
                                                 &stub->args.fstat_cbk.buf, stub->xdata);

                break;
        }

        case GF_FOP_LK:
        {
                if (!stub->args.lk_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.lk_cbk.op_ret,
                                      stub->args.lk_cbk.op_errno,
                                      &stub->args.lk_cbk.lock, stub->xdata);
                else
                        stub->args.lk_cbk.fn (stub->frame,
                                              stub->frame->cookie,
                                              stub->frame->this,
                                              stub->args.lk_cbk.op_ret,
                                              stub->args.lk_cbk.op_errno,
                                              &stub->args.lk_cbk.lock, stub->xdata);
                break;
        }

        case GF_FOP_INODELK:
        {
                if (!stub->args.inodelk_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.inodelk_cbk.op_ret,
                                      stub->args.inodelk_cbk.op_errno, stub->xdata);

                else
                        stub->args.inodelk_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.inodelk_cbk.op_ret,
                                                   stub->args.inodelk_cbk.op_errno, stub->xdata);
                break;
        }

        case GF_FOP_FINODELK:
        {
                if (!stub->args.finodelk_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.finodelk_cbk.op_ret,
                                      stub->args.finodelk_cbk.op_errno, stub->xdata);

                else
                        stub->args.finodelk_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.finodelk_cbk.op_ret,
                                                    stub->args.finodelk_cbk.op_errno, stub->xdata);
                break;
        }

        case GF_FOP_ENTRYLK:
        {
                if (!stub->args.entrylk_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.entrylk_cbk.op_ret,
                                      stub->args.entrylk_cbk.op_errno, stub->xdata);

                else
                        stub->args.entrylk_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.entrylk_cbk.op_ret,
                                                   stub->args.entrylk_cbk.op_errno, stub->xdata);
                break;
        }

        case GF_FOP_FENTRYLK:
        {
                if (!stub->args.fentrylk_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fentrylk_cbk.op_ret,
                                      stub->args.fentrylk_cbk.op_errno, stub->xdata);

                else
                        stub->args.fentrylk_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.fentrylk_cbk.op_ret,
                                                    stub->args.fentrylk_cbk.op_errno, stub->xdata);
                break;
        }

        case GF_FOP_LOOKUP:
        {
                if (!stub->args.lookup_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.lookup_cbk.op_ret,
                                      stub->args.lookup_cbk.op_errno,
                                      stub->args.lookup_cbk.inode,
                                      &stub->args.lookup_cbk.buf,
                                      stub->xdata,
                                      &stub->args.lookup_cbk.postparent);
                else
                        stub->args.lookup_cbk.fn (stub->frame,
                                                  stub->frame->cookie,
                                                  stub->frame->this,
                                                  stub->args.lookup_cbk.op_ret,
                                                  stub->args.lookup_cbk.op_errno,
                                                  stub->args.lookup_cbk.inode,
                                                  &stub->args.lookup_cbk.buf,
                                                  stub->xdata,
                                                  &stub->args.lookup_cbk.postparent);
                /* FIXME NULL should not be passed */

                if (stub->args.lookup_cbk.inode)
                        inode_unref (stub->args.lookup_cbk.inode);

                break;
        }

        case GF_FOP_RCHECKSUM:
        {
                if (!stub->args.rchecksum_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.rchecksum_cbk.op_ret,
                                      stub->args.rchecksum_cbk.op_errno,
                                      stub->args.rchecksum_cbk.weak_checksum,
                                      stub->args.rchecksum_cbk.strong_checksum, stub->xdata);
                else
                        stub->args.rchecksum_cbk.fn (stub->frame,
                                                     stub->frame->cookie,
                                                     stub->frame->this,
                                                     stub->args.rchecksum_cbk.op_ret,
                                                     stub->args.rchecksum_cbk.op_errno,
                                                     stub->args.rchecksum_cbk.weak_checksum,
                                                     stub->args.rchecksum_cbk.strong_checksum, stub->xdata);

                if (stub->args.rchecksum_cbk.op_ret >= 0)
                        GF_FREE (stub->args.rchecksum_cbk.strong_checksum);

                break;
        }

        case GF_FOP_READDIR:
        {
                if (!stub->args.readdir_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.readdir_cbk.op_ret,
                                      stub->args.readdir_cbk.op_errno,
                                      &stub->args.readdir_cbk.entries, stub->xdata);
                else
                        stub->args.readdir_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.readdir_cbk.op_ret,
                                                   stub->args.readdir_cbk.op_errno,
                                                   &stub->args.readdir_cbk.entries, stub->xdata);

                if (stub->args.readdir_cbk.op_ret > 0)
                        gf_dirent_free (&stub->args.readdir_cbk.entries);

                break;
        }

        case GF_FOP_READDIRP:
        {
                if (!stub->args.readdirp_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.readdirp_cbk.op_ret,
                                      stub->args.readdirp_cbk.op_errno,
                                      &stub->args.readdirp_cbk.entries, stub->xdata);
                else
                        stub->args.readdirp_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.readdirp_cbk.op_ret,
                                                    stub->args.readdirp_cbk.op_errno,
                                                    &stub->args.readdirp_cbk.entries, stub->xdata);

                if (stub->args.readdirp_cbk.op_ret > 0)
                        gf_dirent_free (&stub->args.readdirp_cbk.entries);

                break;
        }

        case GF_FOP_XATTROP:
        {
                if (!stub->args.xattrop_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.xattrop_cbk.op_ret,
                                      stub->args.xattrop_cbk.op_errno, stub->xdata);
                else
                        stub->args.xattrop_cbk.fn (stub->frame,
                                                   stub->frame->cookie,
                                                   stub->frame->this,
                                                   stub->args.xattrop_cbk.op_ret,
                                                   stub->args.xattrop_cbk.op_errno,
                                                   stub->args.xattrop_cbk.xattr, stub->xdata);

                if (stub->args.xattrop_cbk.xattr)
                        dict_unref (stub->args.xattrop_cbk.xattr);

                break;
        }
        case GF_FOP_FXATTROP:
        {
                if (!stub->args.fxattrop_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fxattrop_cbk.op_ret,
                                      stub->args.fxattrop_cbk.op_errno, stub->xdata);
                else
                        stub->args.fxattrop_cbk.fn (stub->frame,
                                                    stub->frame->cookie,
                                                    stub->frame->this,
                                                    stub->args.fxattrop_cbk.op_ret,
                                                    stub->args.fxattrop_cbk.op_errno,
                                                    stub->args.fxattrop_cbk.xattr, stub->xdata);

                if (stub->args.fxattrop_cbk.xattr)
                        dict_unref (stub->args.fxattrop_cbk.xattr);

                break;
        }
        case GF_FOP_SETATTR:
        {
                if (!stub->args.setattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.setattr_cbk.op_ret,
                                      stub->args.setattr_cbk.op_errno,
                                      &stub->args.setattr_cbk.statpre,
                                      &stub->args.setattr_cbk.statpost, stub->xdata);
                else
                        stub->args.setattr_cbk.fn (
                                stub->frame,
                                stub->frame->cookie,
                                stub->frame->this,
                                stub->args.setattr_cbk.op_ret,
                                stub->args.setattr_cbk.op_errno,
                                &stub->args.setattr_cbk.statpre,
                                &stub->args.setattr_cbk.statpost, stub->xdata);
                break;
        }
        case GF_FOP_FSETATTR:
        {
                if (!stub->args.fsetattr_cbk.fn)
                        STACK_UNWIND (stub->frame,
                                      stub->args.fsetattr_cbk.op_ret,
                                      stub->args.fsetattr_cbk.op_errno,
                                      &stub->args.fsetattr_cbk.statpre,
                                      &stub->args.fsetattr_cbk.statpost, stub->xdata);
                else
                        stub->args.fsetattr_cbk.fn (
                                stub->frame,
                                stub->frame->cookie,
                                stub->frame->this,
                                stub->args.fsetattr_cbk.op_ret,
                                stub->args.fsetattr_cbk.op_errno,
                                &stub->args.fsetattr_cbk.statpre,
                                &stub->args.fsetattr_cbk.statpost, stub->xdata);
                break;
        }
        default:
        {
                gf_log_callingfn ("call-stub", GF_LOG_ERROR,
                                  "Invalid value of FOP (%d)",
                                  stub->fop);
                break;
        }
        }
out:
        return;
}


static void
call_stub_destroy_wind (call_stub_t *stub)
{
        if (stub->xdata)
                dict_unref (stub->xdata);

        switch (stub->fop) {
        case GF_FOP_OPEN:
        {
                loc_wipe (&stub->args.open.loc);
                if (stub->args.open.fd)
                        fd_unref (stub->args.open.fd);
                break;
        }
        case GF_FOP_CREATE:
        {
                loc_wipe (&stub->args.create.loc);
                if (stub->args.create.fd)
                        fd_unref (stub->args.create.fd);
                break;
        }
        case GF_FOP_STAT:
        {
                loc_wipe (&stub->args.stat.loc);
                break;
        }
        case GF_FOP_READLINK:
        {
                loc_wipe (&stub->args.readlink.loc);
                break;
        }

        case GF_FOP_MKNOD:
        {
                loc_wipe (&stub->args.mknod.loc);
        }
        break;

        case GF_FOP_MKDIR:
        {
                loc_wipe (&stub->args.mkdir.loc);
        }
        break;

        case GF_FOP_UNLINK:
        {
                loc_wipe (&stub->args.unlink.loc);
        }
        break;

        case GF_FOP_RMDIR:
        {
                loc_wipe (&stub->args.rmdir.loc);
        }
        break;

        case GF_FOP_SYMLINK:
        {
                GF_FREE ((char *)stub->args.symlink.linkname);
                loc_wipe (&stub->args.symlink.loc);
        }
        break;

        case GF_FOP_RENAME:
        {
                loc_wipe (&stub->args.rename.old);
                loc_wipe (&stub->args.rename.new);
        }
        break;

        case GF_FOP_LINK:
        {
                loc_wipe (&stub->args.link.oldloc);
                loc_wipe (&stub->args.link.newloc);
        }
        break;

        case GF_FOP_TRUNCATE:
        {
                loc_wipe (&stub->args.truncate.loc);
                break;
        }

        case GF_FOP_READ:
        {
                if (stub->args.readv.fd)
                        fd_unref (stub->args.readv.fd);
                break;
        }

        case GF_FOP_WRITE:
        {
                struct iobref *iobref = stub->args.writev.iobref;
                if (stub->args.writev.fd)
                        fd_unref (stub->args.writev.fd);
                GF_FREE (stub->args.writev.vector);
                if (iobref)
                        iobref_unref (iobref);
                break;
        }

        case GF_FOP_STATFS:
        {
                loc_wipe (&stub->args.statfs.loc);
                break;
        }
        case GF_FOP_FLUSH:
        {
                if (stub->args.flush.fd)
                        fd_unref (stub->args.flush.fd);
                break;
        }

        case GF_FOP_FSYNC:
        {
                if (stub->args.fsync.fd)
                        fd_unref (stub->args.fsync.fd);
                break;
        }

        case GF_FOP_SETXATTR:
        {
                loc_wipe (&stub->args.setxattr.loc);
                if (stub->args.setxattr.dict)
                        dict_unref (stub->args.setxattr.dict);
                break;
        }

        case GF_FOP_GETXATTR:
        {
                GF_FREE ((char *)stub->args.getxattr.name);
                loc_wipe (&stub->args.getxattr.loc);
                break;
        }

        case GF_FOP_FSETXATTR:
        {
                fd_unref (stub->args.fsetxattr.fd);
                if (stub->args.fsetxattr.dict)
                        dict_unref (stub->args.fsetxattr.dict);
                break;
        }

        case GF_FOP_FGETXATTR:
        {
                GF_FREE ((char *)stub->args.fgetxattr.name);
                fd_unref (stub->args.fgetxattr.fd);
                break;
        }

        case GF_FOP_REMOVEXATTR:
        {
                loc_wipe (&stub->args.removexattr.loc);
                GF_FREE ((char *)stub->args.removexattr.name);
                break;
        }

        case GF_FOP_FREMOVEXATTR:
        {
                fd_unref (stub->args.fremovexattr.fd);
                GF_FREE ((char *)stub->args.fremovexattr.name);
                break;
        }

        case GF_FOP_OPENDIR:
        {
                loc_wipe (&stub->args.opendir.loc);
                if (stub->args.opendir.fd)
                        fd_unref (stub->args.opendir.fd);
                break;
        }

        case GF_FOP_FSYNCDIR:
        {
                if (stub->args.fsyncdir.fd)
                        fd_unref (stub->args.fsyncdir.fd);
                break;
        }

        case GF_FOP_ACCESS:
        {
                loc_wipe (&stub->args.access.loc);
                break;
        }

        case GF_FOP_FTRUNCATE:
        {
                if (stub->args.ftruncate.fd)
                        fd_unref (stub->args.ftruncate.fd);
                break;
        }

        case GF_FOP_FSTAT:
        {
                if (stub->args.fstat.fd)
                        fd_unref (stub->args.fstat.fd);
                break;
        }

        case GF_FOP_LK:
        {
                if (stub->args.lk.fd)
                        fd_unref (stub->args.lk.fd);
                break;
        }

        case GF_FOP_INODELK:
        {
                GF_FREE ((char *)stub->args.inodelk.volume);

                loc_wipe (&stub->args.inodelk.loc);
                break;
        }
        case GF_FOP_FINODELK:
        {
                GF_FREE ((char *)stub->args.finodelk.volume);

                if (stub->args.finodelk.fd)
                        fd_unref (stub->args.finodelk.fd);
                break;
        }
        case GF_FOP_ENTRYLK:
        {
                GF_FREE ((char *)stub->args.entrylk.volume);

                GF_FREE ((char *)stub->args.entrylk.name);
                loc_wipe (&stub->args.entrylk.loc);
                break;
        }
        case GF_FOP_FENTRYLK:
        {
                GF_FREE ((char *)stub->args.fentrylk.volume);

                GF_FREE ((char *)stub->args.fentrylk.name);

                if (stub->args.fentrylk.fd)
                        fd_unref (stub->args.fentrylk.fd);
                break;
        }

        case GF_FOP_LOOKUP:
        {
                loc_wipe (&stub->args.lookup.loc);
                break;
        }

        case GF_FOP_RCHECKSUM:
        {
                if (stub->args.rchecksum.fd)
                        fd_unref (stub->args.rchecksum.fd);
                break;
        }

        case GF_FOP_READDIR:
        {
                if (stub->args.readdir.fd)
                        fd_unref (stub->args.readdir.fd);
                break;
        }

        case GF_FOP_READDIRP:
        {
                if (stub->args.readdirp.fd)
                        fd_unref (stub->args.readdirp.fd);

                break;
        }

        case GF_FOP_XATTROP:
        {
                loc_wipe (&stub->args.xattrop.loc);
                dict_unref (stub->args.xattrop.xattr);
                break;
        }
        case GF_FOP_FXATTROP:
        {
                if (stub->args.fxattrop.fd)
                        fd_unref (stub->args.fxattrop.fd);
                dict_unref (stub->args.fxattrop.xattr);
                break;
        }
        case GF_FOP_SETATTR:
        {
                loc_wipe (&stub->args.setattr.loc);
                break;
        }
        case GF_FOP_FSETATTR:
        {
                if (stub->args.fsetattr.fd)
                        fd_unref (stub->args.fsetattr.fd);
                break;
        }
        default:
        {
                gf_log_callingfn ("call-stub", GF_LOG_ERROR,
                                  "Invalid value of FOP (%d)",
                                  stub->fop);
                break;
        }
        }
}


static void
call_stub_destroy_unwind (call_stub_t *stub)
{
        if (stub->xdata)
                dict_unref (stub->xdata);

        switch (stub->fop) {
        case GF_FOP_OPEN:
        {
                if (stub->args.open_cbk.fd)
                        fd_unref (stub->args.open_cbk.fd);
        }
        break;

        case GF_FOP_CREATE:
        {
                if (stub->args.create_cbk.fd)
                        fd_unref (stub->args.create_cbk.fd);

                if (stub->args.create_cbk.inode)
                        inode_unref (stub->args.create_cbk.inode);
        }
        break;

        case GF_FOP_STAT:
                break;

        case GF_FOP_READLINK:
        {
                GF_FREE ((char *)stub->args.readlink_cbk.buf);
        }
        break;

        case GF_FOP_MKNOD:
        {
                if (stub->args.mknod_cbk.inode)
                        inode_unref (stub->args.mknod_cbk.inode);
        }
        break;

        case GF_FOP_MKDIR:
        {
                if (stub->args.mkdir_cbk.inode)
                        inode_unref (stub->args.mkdir_cbk.inode);
        }
        break;

        case GF_FOP_UNLINK:
                break;

        case GF_FOP_RMDIR:
                break;

        case GF_FOP_SYMLINK:
        {
                if (stub->args.symlink_cbk.inode)
                        inode_unref (stub->args.symlink_cbk.inode);
        }
        break;

        case GF_FOP_RENAME:
                break;

        case GF_FOP_LINK:
        {
                if (stub->args.link_cbk.inode)
                        inode_unref (stub->args.link_cbk.inode);
        }
        break;

        case GF_FOP_TRUNCATE:
                break;

        case GF_FOP_READ:
        {
                if (stub->args.readv_cbk.op_ret >= 0) {
                        struct iobref *iobref = stub->args.readv_cbk.iobref;
                        GF_FREE (stub->args.readv_cbk.vector);

                        if (iobref) {
                                iobref_unref (iobref);
                        }
                }
        }
        break;

        case GF_FOP_WRITE:
                break;

        case GF_FOP_STATFS:
                break;

        case GF_FOP_FLUSH:
                break;

        case GF_FOP_FSYNC:
                break;

        case GF_FOP_SETXATTR:
                break;

        case GF_FOP_GETXATTR:
        {
                if (stub->args.getxattr_cbk.dict)
                        dict_unref (stub->args.getxattr_cbk.dict);
        }
        break;

        case GF_FOP_FSETXATTR:
                break;

        case GF_FOP_FGETXATTR:
        {
                if (stub->args.fgetxattr_cbk.dict)
                        dict_unref (stub->args.fgetxattr_cbk.dict);
        }
        break;

        case GF_FOP_REMOVEXATTR:
                break;
        case GF_FOP_FREMOVEXATTR:
                break;

        case GF_FOP_OPENDIR:
        {
                if (stub->args.opendir_cbk.fd)
                        fd_unref (stub->args.opendir_cbk.fd);
        }
        break;

        case GF_FOP_FSYNCDIR:
                break;

        case GF_FOP_ACCESS:
                break;

        case GF_FOP_FTRUNCATE:
                break;

        case GF_FOP_FSTAT:
                break;

        case GF_FOP_LK:
                break;

        case GF_FOP_INODELK:
                break;

        case GF_FOP_FINODELK:
                break;

        case GF_FOP_ENTRYLK:
                break;

        case GF_FOP_FENTRYLK:
                break;

        case GF_FOP_LOOKUP:
        {
                if (stub->args.lookup_cbk.inode)
                        inode_unref (stub->args.lookup_cbk.inode);
        }
        break;

        case GF_FOP_RCHECKSUM:
        {
                if (stub->args.rchecksum_cbk.op_ret >= 0) {
                        GF_FREE (stub->args.rchecksum_cbk.strong_checksum);
                }
        }
        break;

        case GF_FOP_READDIR:
        {
                if (stub->args.readdir_cbk.op_ret > 0) {
                        gf_dirent_free (&stub->args.readdir_cbk.entries);
                }
        }
        break;

        case GF_FOP_READDIRP:
        {
                if (stub->args.readdirp_cbk.op_ret > 0) {
                        gf_dirent_free (&stub->args.readdirp_cbk.entries);
                }
        }
        break;

        case GF_FOP_XATTROP:
        {
                if (stub->args.xattrop_cbk.xattr)
                        dict_unref (stub->args.xattrop_cbk.xattr);
        }
        break;

        case GF_FOP_FXATTROP:
        {
                if (stub->args.fxattrop_cbk.xattr)
                        dict_unref (stub->args.fxattrop_cbk.xattr);
        }
        break;

        case GF_FOP_SETATTR:
        {
                break;
        }

        case GF_FOP_FSETATTR:
        {
                break;
        }

        default:
        {
                gf_log_callingfn ("call-stub", GF_LOG_ERROR,
                                  "Invalid value of FOP (%d)",
                                  stub->fop);
                break;
        }
        }
}


void
call_stub_destroy (call_stub_t *stub)
{
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        if (stub->wind) {
                call_stub_destroy_wind (stub);
        } else {
                call_stub_destroy_unwind (stub);
        }

        stub->stub_mem_pool = NULL;
        mem_put (stub);
out:
        return;
}

void
call_resume (call_stub_t *stub)
{
        xlator_t *old_THIS = NULL;

        errno = EINVAL;
        GF_VALIDATE_OR_GOTO ("call-stub", stub, out);

        list_del_init (&stub->list);

        old_THIS = THIS;
        THIS = stub->frame->this;
        {
                if (stub->wind)
                        call_resume_wind (stub);
                else
                        call_resume_unwind (stub);
        }
        THIS = old_THIS;

        call_stub_destroy (stub);
out:
        return;
}
