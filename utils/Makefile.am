if ENABLEUTILS

SUBDIRS = po

INCLUDES = -I../lib

bin_PROGRAMS = odbx-sql

odbx_sql_SOURCES = odbx-sql.cpp config.cpp config.hpp commands.cpp commands.hpp
odbx_sql_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
odbx_sql_LDADD = ../lib/.libs/libopendbxplus.so -lconfuse -lreadline

endif
