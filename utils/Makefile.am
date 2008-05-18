if ENABLEUTILS

SUBDIRS = po

INCLUDES = -I../lib

bin_PROGRAMS = odbx-sql

odbx_sql_SOURCES = odbx-sql.cpp config.cpp config.hpp commands.cpp commands.hpp completion.cpp completion.hpp
odbx_sql_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -DKEYWORDFILE=\"$(pkgdatadir)/keywords\"
odbx_sql_LDADD = ../lib/.libs/libopendbxplus.so -lconfuse -lreadline

pkgdata_DATA = keywords

endif
