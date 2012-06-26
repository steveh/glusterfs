lib_LTLIBRARIES = libglusterfsclient.la
noinst_HEADERS = libglusterfsclient-mem-types.h
libglusterfsclient_HEADERS = libglusterfsclient.h
libglusterfsclientdir = $(includedir)

libglusterfsclient_la_SOURCES = libglusterfsclient.c libglusterfsclient-mgmt.c
libglusterfsclient_la_CFLAGS =  -fPIC -Wall
libglusterfsclient_la_LIBADD = $(top_builddir)/libglusterfs/src/libglusterfs.la
	$(top_builddir)/rpc/rpc-lib/src/libgfrpc.la \
	$(top_builddir)/rpc/xdr/src/libgfxdr.la \
	$(GF_LDADD)
libglusterfsclient_la_CPPFLAGS = -D_FILE_OFFSET_BITS=64 -D$(GF_HOST_OS) -D__USE_FILE_OFFSET64 -D_GNU_SOURCE\
	-I$(top_srcdir)/libglusterfs/src \
	-DDATADIR=\"$(localstatedir)\" -DCONFDIR=\"$(sysconfdir)/glusterfs\" $(GF_CFLAGS) \
	-I$(top_srcdir)/rpc/rpc-lib/src -I$(top_srcdir)/rpc/xdr/src

libglusterfsclient_la_LDFLAGS = -shared -nostartfiles


noinst_PROGRAMS = glfs-test
glfs_test_SOURCES = glfs-test.c
glfs_test_CFLAGS =  -fPIC -Wall
glfs_test_LDADD = $(top_builddir)/libglusterfsclient/src/libglusterfsclient.la \
	$(top_builddir)/libglusterfs/src/libglusterfs.la \
	$(top_builddir)/rpc/rpc-lib/src/libgfrpc.la \
	$(top_builddir)/rpc/xdr/src/libgfxdr.la \
	$(GF_LDADD)

glfs_test_CPPFLAGS = -D_FILE_OFFSET_BITS=64 -D$(GF_HOST_OS) -D__USE_FILE_OFFSET64 -D_GNU_SOURCE \
	-I$(top_srcdir)/libglusterfsclient/src -DDATADIR=\"$(localstatedir)\" -DCONFDIR=\"$(sysconfdir)/glusterfs\" $(GF_CFLAGS)

CLEANFILES =

$(top_builddir)/libglusterfs/src/libglusterfs.la:
	$(MAKE) -C $(top_builddir)/libglusterfs/src/ all

