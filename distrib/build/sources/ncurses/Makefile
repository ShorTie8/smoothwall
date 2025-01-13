# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ncurses
VERSION = 5.9

BASE_URL = ftp://ftp.gnu.org/gnu/ncurses/

ifeq ($(BUILD_PHASE), Stage_3)
CONFIG_OPTS += --with-shared \
    --without-debug --without-ada --enable-overwrite
else
CONFIG_OPTS += --with-shared --without-debug --with-termlib

INSTALL = yes

install: compile
	mkdir -p $(PKG_ROOT)
	+$(MAKE) -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	rm $(PKG_ROOT)/usr/lib/libncurses.so
	echo "INPUT(libncurses.so.5 -ltinfo)" > $(PKG_ROOT)/usr/lib/libncurses.so
endif

include ../Makefile.rules
