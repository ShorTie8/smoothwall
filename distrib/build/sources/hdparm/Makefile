# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = hdparm
VERSION = 6.1

BASE_URL = http://www.ibiblio.org/pub/Linux/system/hardware/

CONFIGURE = yes
INSTALL = yes

configure: patch
	@true

install: compile
	@mkdir -p $(PKG_ROOT)/sbin
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
