# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = util-linux
VERSION = 2.12p

BASE_URL = ftp://ftp.win.tue.nl/pub/linux-local/utils/util-linux/

ifeq ($(BUILDENV), 1)
CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@(cd $(DIR); \
	cp configure configure.backup; \
	sed "s@/usr/include@/tools/include@g" configure.backup > configure; \
	./configure --prefix=/$(PKG_DIR) $(CONFIG_OPTS));

compile: configure
	@make -C $(COMPILE_DIR)/lib
	@make -C $(COMPILE_DIR)/mount mount umount
	@make -C $(COMPILE_DIR)/text-utils more

install: compile
	@cp $(DIR)/mount/umount $(DIR)/mount/mount \
		$(DIR)/text-utils/more $(PKG_ROOT)/$(PKG_DIR)/bin
endif

include ../Makefile.rules
