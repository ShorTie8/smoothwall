# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libpcap
VERSION = 2007.02.12

BASE_URL = http://www.tcpdump.org/weekly/

INSTALL = yes

install: compile
	-mkdir $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/usr/include/net
	@(cd $(PKG_ROOT)/usr/include/net; ln -s ../pcap-bpf.h bpf.h)

include ../Makefile.rules
