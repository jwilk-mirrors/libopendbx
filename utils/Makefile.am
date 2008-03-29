if ENABLEUTILS

INCLUDES = -I../lib

bin_PROGRAMS = odbx-sql

odbx_sql_SOURCES = odbx-sql.cpp config.cpp config.hpp commands.cpp commands.hpp
odbx_sql_LDADD = -lopendbxplus -lconfuse -lreadline
odbx_sql_LDFLAGS = -L../lib

endif
