# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = wget
VERSION = 1.12

BASE_URL = ftp://ftp.gnu.org/gnu/wget

ifndef BUILDENV
CONFIG_OPTS = --with-ssl --with-libssl-prefix=/usr/ssl
else
CONFIG_OPTS = --without-ssl
endif

include ../Makefile.rules
