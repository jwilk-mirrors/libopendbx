ACLOCAL_AMFLAGS = -I m4

SUBDIRS = backends doc lib po test utils

EXTRA_DIST = config.rpath \
	@PACKAGE@.spec.in \
	@PACKAGE@.spec \
	@PACKAGE@1.pc.in \
	@PACKAGE@plus1.pc.in \
	ABOUT-NLS

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE@1.pc @PACKAGE@plus1.pc

ldconfigdir = $(sysconfdir)/ld.so.conf.d
ldconfig_DATA = @PACKAGE@1.conf

CLEANFILES = @PACKAGE@1.conf

@PACKAGE@1.conf: config.status
	echo "$(libdir)/opendbx" > @PACKAGE@1.conf
