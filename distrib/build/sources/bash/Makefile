# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = bash
VERSION = 2.05b

BASE_URL = ftp://ftp.gnu.org/gnu/bash/

PATCH_FILE1 = bash-2.05b-2.patch

ifeq ($(ARCH), i386)
BUILD = --build=i386-pc-linux-gnu
endif

ifndef BUILDENV
CONFIG_OPTS = --bindir=/bin $(BUILD)
endif

ifeq ($(BUILDENV), 1)
INSTALL = yes

install: compile
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@(cd $(PKG_ROOT)/$(PKG_DIR)/bin; ln -s bash sh);
endif

ifndef BUILDENV
INSTALL = yes

install: compile
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install
	@(cd $(PKG_ROOT)/bin; ln -s bash sh);
endif

include ../Makefile.rules
