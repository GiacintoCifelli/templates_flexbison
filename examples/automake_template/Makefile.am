### SPDX-License-Identifier: BSD-2-Clause-FreeBSD ###

AM_MAKEFLAGS = --no-print-directory

srcdir = @srcdir@
VPATH  = @srcdir@

noinst_LTLIBRARIES =

bin_PROGRAMS = gen

files =
files += \
	src/vending_parser.y src/vending_lexer.l \
	src/main.c

built =
built_clean =

built += include/config.h include/version.h

built += src/vending_parser.c src/vending_lexer.c
built_clean += src/vending_parser.h src/vending_lexer.h

BUILT_SOURCES = $(built)

files += built
gen_SOURCES = $(files)

AM_CFLAGS = -Wextra -fplan9-extensions -DEXTENSIONSDIR=\""$(extensionsdir)"\"

AM_CPPFLAGS = -I$(srcdir) -I$(builddir)/include -I$(builddir)/gen

gen_CPPFLAGS = $(AM_CPPFLAGS)

gen_CFLAGS = $(AM_CFLAGS)
gen_LDFLAGS = -rdynamic -Wl,--export-dynamic -ldl -lpthread

gen_DEPENDENCIES =

CLEANFILES = $(BUILT_SOURCES) $(built_clean)

%.c : %.l
	flex -o $@  --header-file=$(strip $(patsubst %.c, %.h, $@)) $<

%.c : %.y
	bison -o $@ --defines=$(strip $(patsubst %.c, %.h, $@)) $<

help:
	@grep '^[^#[:space:]].*:' Makefile | sort
.PHONY: help
