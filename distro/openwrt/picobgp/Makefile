#
# Copyright (C) 2012 - Claudio Mignanti
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=picobgp
PKG_REV:=2d27a5329644b687baa2c527d06386d9de754529
PKG_VERSION:=v0.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://github.com/claudyus/picobgp.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define Package/picobgp
  SECTION:=network
  CATEGORY:=Network
  TITLE:=A tiny advertising routing deamon
  URL:=http://www.mignanti.eu/progetti/picobgp
endef

define Package/picobgp/description
 picobgp is minimalistic not-compliant-bgp free-inspired daemon for
 subnetwork advertising and route topology update.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/picobgp/install
	$(INSTALL_DIR) $(1)/sbin/
	$(CP) $(PKG_BUILD_DIR)/bgp $(1)/sbin/
endef

$(eval $(call BuildPackage,picobgp))
