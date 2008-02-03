#
#  OpenDBX rpm spec file
#
#  By default OpenDBX is build with this backends:
#  - mysql
#  - pgsql
#  - sqlite3
#  to disable use --without [module-name]
#
#  Optional supported backends are:
#  - sqlite
#  - firebird
#  - mssql
#  - sybase
#  - oracle
#  to enable use --with [module-name]
#


Name:    opendbx
Version:    1.3.5
Release:    1%{?dist}
Summary:    Unified database layer with a clean and lightweight interface
Summary(de):    Bibliothek zum Zugriff auf Datenbanken über eine einheitliche Schnittstelle
Group:    Development/Libraries
License:    LGPL
URL:    http://www.linuxnetworks.de/opendbx/
Source0:    http://www.linuxnetworks.de/opendbx/download/%{name}-%{version}.tar.gz
BuildRoot:    %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:    gettext

%description
OpenDBX provides a clean and lightweight API for interfacing native relational
database APIs in a consistent way. By using the OpenDBX API you don't have to
adapt your program to the different database APIs by yourself.

%description -l de
OpenDBX ist eine schlanke und einfach zu verwendende Bibliothek, die es
ermöglicht verschiedene Datenbankserver über eine konsistente Schnittstelle
anzusprechen.


%package devel
Summary:    OpenDBX development headers
Summary(de):    Entwicklungsschnittstellen für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    pkgconfig

%description devel
Header files for the OpenDBX database abstraction library

%description -l de devel
Schnittstellen der OpenDBX Datenbankbibliothek zur Softwareentwicklung


%if %{!?_without_mysql:1}%{?_without_mysql:0}
%package mysql
Summary:    MySQL backend for OpenDBX
Summary(de):    MySQL Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    mysql
BuildRequires:    mysql-devel

%description mysql
MySQL backend for the OpenDBX database abstraction library

%description -l de mysql
MySQL Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{!?_without_pgsql:1}%{?_without_pgsql:0}
%package pgsql
Summary:    PostgreSQL backend for OpenDBX
Summary(de):    PostgreSQL Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    postgresql
BuildRequires:    postgresql-devel

%description pgsql
PostgreSQL backend for the OpenDBX database abstraction library

%description -l de pgsql
PostgreSQL Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{!?_without_sqlite3:1}%{?_without_sqlite3:0}
%package sqlite3
Summary:    SQLite3 backend for OpenDBX
Summary(de):    SQLite3 Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    sqlite >= 3.0
BuildRequires:    sqlite-devel >= 3.0

%description sqlite3
SQLite3 backend for the OpenDBX database abstraction library

%description -l de sqlite3
Sqlite3 Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{?_with_sqlite:1}%{!?_with_sqlite:0}
%package sqlite
Summary:    SQLite backend for OpenDBX
Summary(de):    SQLite Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    sqlite < 3.0
BuildRequires:    sqlite-devel < 3.0

%description sqlite
SQLite backend for the OpenDBX database abstraction library

%description -l de sqlite
Sqlite Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{?_with_firebird:1}%{!?_with_firebird:0}
%package firebird
Summary:    Firebird/Interbase backend for OpenDBX
Summary(de):    Firebird/Interbase Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    firebird
BuildRequires:    firebird-devel

%description firebird
Firebird/Interbase backend for the OpenDBX database abstraction library

%description -l de firebird
Firebird/Interbase Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{?_with_mssql:1}%{!?_with_mssql:0}
%package mssql
Summary:    MS SQL Server backend for OpenDBX
Summary(de):    MS SQL Server Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    freetds
BuildRequires:    freetds-devel

%description mssql
MS SQL Server backend for the OpenDBX database abstraction library

%description -l de mssql
MS SQL Server Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%if %{?_with_sybase:1}%{!?_with_sybase:0}
%package sybase
Summary:    Sybase backend for OpenDBX
Summary(de):    Sybase Unterstützung für OpenDBX
Group:    Development/Libraries
Requires:    %{name} = %{version}-%{release}
Requires:    freetds
BuildRequires:    freetds-devel

%description sybase
Sybase ctlib backend for the OpenDBX database abstraction library

%description -l de sybase
Sybase Unterstützung für die OpenDBX Datenbankbibliothek
%endif


%prep


%setup -q


%build
%configure \
    --prefix=/usr \
    --disable-static \
    --with-backends="\
%{!?_without_mysql:mysql }\
%{!?_without_pgsql:pgsql }\
%{!?_without_sqlite3:sqlite3 }\
%{?_with_sqlite:sqlite }\
%{?_with_firebird:firebird }\
%{?_with_mssql:mssql }\
%{?_with_sybase:sybase }\
" \
%{!?_without_mysql:CPPFLAGS="-I/usr/include/mysql"} \
%{!?_without_mysql:LDFLAGS="-L/usr/lib/mysql"} \
CFLAGS="%{optflags}"

make %{?_smp_mflags}


%install
if test "%{buildroot}" != "/"; then rm -rf %{buildroot}; fi
make DESTDIR=%{buildroot} install
rm %{buildroot}%{_libdir}/opendbx/lib*.*a
rm %{buildroot}%{_libdir}/libopendbx.*a
%find_lang %{name}


%clean
if test "%{buildroot}" != "/"; then rm -rf %{buildroot}; fi


%post -p /sbin/ldconfig


%postun -p /sbin/ldconfig


%files -f %{name}.lang
%defattr(-,root,root,-)
%{_libdir}/opendbx
%{_libdir}/libopendbx.so.*
%doc AUTHORS COPYING ChangeLog NEWS README TODO


%files devel
%defattr(-,root,root,-)
%{_includedir}/odbx.h
%{_libdir}/libopendbx.so
%{_libdir}/pkgconfig/opendbx.pc


%if %{!?_without_mysql:1}%{?_without_mysql:0}
%files mysql
%defattr(-,root,root,-)
%{_libdir}/opendbx/libmysqlbackend.so*
%endif


%if %{!?_without_pgsql:1}%{?_without_pgsql:0}
%files pgsql
%defattr(-,root,root,-)
%{_libdir}/opendbx/libpgsqlbackend.so*
%endif


%if %{!?_without_sqlite3:1}%{?_without_sqlite3:0}
%files sqlite3
%defattr(-,root,root,-)
%{_libdir}/opendbx/libsqlite3backend.so*
%endif


%if %{?_with_sqlite:1}%{!?_with_sqlite:0}
%files sqlite
%defattr(-,root,root,-)
%{_libdir}/opendbx/libsqlitebackend.so*
%endif


%if %{?_with_firebird:1}%{!?_with_firebird:0}
%files firebird
%defattr(-,root,root,-)
%{_libdir}/opendbx/libfirebirdbackend.so*
%endif


%if %{?_with_mssql:1}%{!?_with_mssql:0}
%files mssql
%defattr(-,root,root,-)
%{_libdir}/opendbx/libmssqlbackend.so*
%endif


%if %{?_with_sybase:1}%{!?_with_sybase:0}
%files sybase
%defattr(-,root,root,-)
%{_libdir}/opendbx/libsybasebackend.so*
%endif


%changelog
* Wed Jan 31 2007 Norbert Sendetzky <norbert@linuxnetworks.de> 1.2.1-1
- Added German summary and descriptions
- Disabled static library builds and removed libtool files
- Added ldconfig call in post and postun sections
- Added gettext and pkgconfig as requirements
- Replaced language file handling with find_lang macro
- Used optflags macro instead of hard coded compiler flags
- Used macro style consistently
- Corrected mail addresses
- Removed oracle sections
- Fixed _without_pgqql
- Minor changes

* Sat Dec 09 2006 Norbert Sendetzky <norbert@linuxnetworks.de> 1.1.8-1
- Added mssql, sybase and oracle backend

* Tue Jun 13 2006 Kees Monshouwer <mind@monshouwer.com> 1.1.0-2
- Fixed a few minor problems
- Added conditional build support
- Added firefird and freetds backend

* Mon Jun 12 2006 Kees Monshouwer <mind@monshouwer.com> 1.1.0-1
- Initial build for CentOS 4.3
