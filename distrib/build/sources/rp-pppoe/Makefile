# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = rp-pppoe
VERSION = 3.8

DIR = $(PACKAGE)-$(VERSION)/src

BASE_URL = http://www.roaringpenguin.com/files/download/

INSTALL = yes
CLEAN = yes

install: compile
	@mkdir -p $(PKG_ROOT)/$(PKG_DIR)/sbin
	@install -D $(DIR)/pppoe $(PKG_ROOT)/$(PKG_DIR)/sbin/pppoe

clean:
	@rm -rf $(PACKAGE)-$(VERSION)
	@rm -rf $(PKG_ROOT)
	
include ../Makefile.rules
