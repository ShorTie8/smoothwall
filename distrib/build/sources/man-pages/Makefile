# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = man-pages
VERSION = 2.60

BASE_URL = http://www.kernel.org/pub/linux/docs/manpages/

INSTALL = yes

install: patch
	@make -C $(DIR) prefix=$(PKG_ROOT) install

include ../Makefile.rules
