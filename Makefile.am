ACLOCAL_AMFLAGS = -I m4

SUBDIRS = backends doc lib po test utils

EXTRA_DIST = config.rpath \
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/docs \
	debian/rules \
	debian/watch \
	opendbx.pc.in \
	opendbx.spec.in \
	opendbx.spec \
	ABOUT-NLS\
	Doxyfile

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = opendbx.pc


if ENABLEMAN

doc/man/man3/OpenDBX.3 doc/man/man3/OpenDBX_Conn.3 doc/man/man3/OpenDBX_Exception.3 doc/man/man3/OpenDBX_Lob.3 doc/man/man3/OpenDBX_Result.3 doc/man/man3/OpenDBX_Stmt.3: lib/opendbx/api
	if test ! -e doxygen.lock; then touch doxygen.lock; doxygen Doxyfile; rm doxygen.lock; fi

clean:
	rm -r doxgen.lock
	for file in `ls doc/man/man3/OpenDBX_*.3`; do rm -f $$file; done

man_MANS = \
	doc/man/man3/OpenDBX.3 \
	doc/man/man3/OpenDBX_Conn.3 \
	doc/man/man3/OpenDBX_Exception.3 \
	doc/man/man3/OpenDBX_Lob.3 \
	doc/man/man3/OpenDBX_Result.3 \
	doc/man/man3/OpenDBX_Stmt.3

endif
