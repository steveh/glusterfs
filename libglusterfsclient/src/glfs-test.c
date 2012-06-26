#include <stdio.h>
#include "libglusterfsclient.h"

int
main (int argc, char *argv[])
{
	glfs_ctx_t *ctx = NULL;
	int         ret = 0;

	ctx = glfs_ctx_new ("iops");
	if (!ctx) {
		fprintf (stderr, "glfs_ctx_new: returned NULL\n");
		return 1;
	}

//	ret = glfs_ctx_set_volfile (ctx, "/tmp/filename.vol");

	ret = glfs_ctx_set_volfile_server (ctx, "socket", "localhost", 24007);

//	ret = glfs_ctx_set_volfile_server (ctx, "unix", "/tmp/gluster.sock", 0);

	ret = glfs_ctx_set_logging (ctx, "/tmp/volname.log", 0);

	ret = glfs_ctx_init (ctx);
	if (ret)
		fprintf (stderr, "glfs_ctx_init: returned %d\n", ret);

	sleep (10);

	return ret;
}
