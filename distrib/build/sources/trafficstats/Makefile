# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions # for IPTABLES version to use


PACKAGE = trafficstats
VERSION = 0.0

MODSWROOT = /var/smoothwall/traffic

compile:
	@make -C $(COMPILE_DIR) CFLAGS="$(CFLAGS)" MODSWROOT=$(MODSWROOT) IPTABLES_VERSION=$(IPTABLES_VER) KERNEL_VER=$(KERNEL_VER) 

install: compile
	@install -D $(COMPILE_DIR)/trafficlogger $(PKG_ROOT)/$(PKG_DIR)/sbin/trafficlogger
	@install -D $(COMPILE_DIR)/trafficmon $(PKG_ROOT)/$(PKG_DIR)/sbin/trafficmon

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /

clean:
	@make -C $(COMPILE_DIR) CFLAGS="$(CFLAGS)" clean
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

packageinfo.html: clean
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@tar --exclude .svn -zcvf /tmp/downloads/$(TARBALL).tar.gz $(DIR)
	@echo "Download: <a href='downloads/$(TARBALL).tar.gz'>$(TARBALL).tar.gz</a><br>" >>/tmp/packageinfo.html
