## Config file for GNU Automake.
##
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

include $(top_srcdir)/am/guilec

TEST_EXTENSIONS = .scm

if !CROSS_COMPILING

SCM_TESTS = \
	message.scm

TESTS = ${SCM_TESTS}

AM_TESTS_ENVIRONMENT = \
	abs_top_srcdir="$(abs_top_srcdir)"; export abs_top_srcdir; \
	abs_top_builddir="$(abs_top_builddir)"; export abs_top_builddir; \
	ORIGTERM=${TERM}; export ORIGTERM; \
	TERM=xterm; export TERM; \
	GUILE=$(GUILE); export GUILE; \
	GUILE_WARN_DEPRECATED=no; export GUILE_WARN_DEPRECATED; \
	GUILE_AUTO_COMPILE=0; export GUILE_AUTO_COMPILE;

# LOG_COMPILER was introduced in Automake 1.12; don't expect "make
# check" or "make distcheck" to work with earlier versions.
SCM_LOG_COMPILER = \
	${top_builddir}/libtool \
	-dlopen ${top_builddir}/libguile-dbus/libguile-dbus.la \
	--mode=execute $(GUILE)

AM_SCM_LOG_FLAGS = \
	-L "$(top_srcdir)" \
	-L "$(top_srcdir)/modules"	\
	-s

EXTRA_DIST = \
	${SCM_TESTS}

### Compile modules

# GOBJECTS = common.go

ccachedir=$(libdir)/guile/$(GUILE_EFFECTIVE_VERSION)/site-ccache/dbus
# nobase_dist_ccache_DATA = $(GOBJECTS)

guilec_warnings =				\
  -Wunbound-variable -Warity-mismatch		\
  -Wunused-variable -Wunused-toplevel

guilec_opts = 					\
	$(guilec_warnings)

# TODO: Move environment setup to a separate file.
guilec_env  = 									\
	GUILE_AUTO_COMPILE=0 							\
	GUILE_SYSTEM_EXTENSIONS_PATH="$(abs_top_builddir)/libguile-dbus/.libs/:${GUILE_SYSTEM_EXTENSIONS_PATH}"	\
	GUILE_LOAD_PATH="$(abs_top_srcdir)/modules"				\
	GUILE_LOAD_COMPILED_PATH="$(builddir)/dbus:$$GUILE_LOAD_COMPILED_PATH"

.scm.go:
	$(AM_V_GUILEC)$(guilec_env) $(GUILEC) $(guilec_opts) \
	--output=$@ $<

###

CLEANFILES = \
	*.log		\
	$(GOBJECTS)

else CROSS_COMPILING

TESTS =
SCM_TESTS =

endif CROSS_COMPILING
