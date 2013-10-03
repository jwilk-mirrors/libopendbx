EXTRA_DIST = libopendbx.ver libopendbxplus.ver


include_HEADERS = odbx.h
nobase_include_HEADERS = opendbx/api.h opendbx/api


lib_LTLIBRARIES = libopendbx.la libopendbxplus.la

libopendbx_la_SOURCES =  odbx.c odbxdrv.h odbxlib.h odbxlog.h avl.h avl.c odbxpool.h odbxpool.c
libopendbx_la_CFLAGS = -fno-strict-aliasing
libopendbx_la_CPPFLAGS = -DLIBVERSION=@LIBVERSION@ -DLOCALEDIR=\"$(localedir)\"
libopendbx_la_LDFLAGS = -no-undefined -export-dynamic -version-info @APIVERSION@
libopendbx_la_LIBADD = $(MAINLIBS) $(LTLIBINTL)

libopendbxplus_la_SOURCES =  odbx_iface.hpp odbx_impl.hpp odbx_impl.cpp odbx.cpp
libopendbxplus_la_CXXFLAGS = -DLIBVERSION=@LIBVERSION@
libopendbxplus_la_LDFLAGS = -no-undefined -export-dynamic -version-info @APIPLUSVERSION@
libopendbxplus_la_LIBADD = $(LTLIBINTL) libopendbx.la

if VSYMLDFLAGS
libopendbx_la_LDFLAGS += -Wl,--version-script,libopendbx.ver -Wl,-O1
libopendbxplus_la_LDFLAGS += -Wl,--version-script,libopendbxplus.ver -Wl,-O1
endif

if SINGLELIB
libopendbx_la_CPPFLAGS += -DODBX_SINGLELIB
libopendbx_la_LIBADD += @backendlibs@
else
libopendbx_la_CPPFLAGS += -DLIBPATH=\"$(libdir)/opendbx\" -DLIBPREFIX=\"@LIBPREFIX@\" -DLIBSUFFIX=\"@LIBSUFFIX@\"
endif
