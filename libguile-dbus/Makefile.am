## Copyright (C) 2020 Artyom V. Poptsov <poptsov.artyom@gmail.com>
##
## This file is part of Guile-DBus.
##
## Guile-DBus is free software: you can redistribute it and/or
## modify it under the terms of the GNU General Public License as
## published by the Free Software Foundation, either version 3 of the
## License, or (at your option) any later version.
##
## Guile-DBus is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Guile-DBus.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/am/snarf

lib_LTLIBRARIES = libguile-dbus.la

libguile_dbus_la_SOURCES = \
	common.h			\
	common.c			\
	error.h				\
	error.c				\
	symbols.h			\
	symbols.c			\
	dbus-connection-type.c		\
	dbus-connection-type.h		\
	dbus-connection-func.h		\
	dbus-connection-func.c		\
	dbus-connection-main.c		\
	dbus-message-type.h		\
	dbus-message-type.c		\
	dbus-message-func.h		\
	dbus-message-func.c		\
	dbus-message-main.c		\
	dbus-pending-call-type.h	\
	dbus-pending-call-type.c	\
	dbus-pending-call-main.c

BUILT_SOURCES = \
	dbus-connection-type.x 		\
	dbus-connection-func.x		\
	dbus-message-type.x		\
	dbus-message-func.x		\
	dbus-pending-call-type.x

libguile_dbus_la_CPPFLAGS = $(CFLAGS) $(GUILE_CFLAGS) $(DBUS_CFLAGS)

DBUS_LD_FLAGS = -ldbus-1

libguile_dbus_la_LDFLAGS = -module -no-undefined $(DBUS_LD_FLAGS)  \
	-version-info $(LIBGUILE_DBUS_INTERFACE) $(GUILE_LDFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)/libguile-dbus -I$(top_builddir)/libguile-dbus

snarfcppopts = $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(GUILE_CFLAGS) \
	$(AM_CPPFLAGS) $(DBUS_CFLAGS)

SUFFIXES = .x
.c.x:
	$(AM_V_SNARF) CPP="$(CPP)" $(GUILE_SNARF) -o $@ $< $(snarfcppopts)

CLEANFILES = *.x

## Makefile.am ends here
