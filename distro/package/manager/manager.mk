################################################################################
#
# manager
#
################################################################################

MANAGER_VERSION = 1.0.0
MANAGER_SITE = $(BR2_EXTERNAL_IIOTGATEWAY_PATH)/../manager
MANAGER_SITE_METHOD = local

MANAGER_GOMOD = manager

MANAGER_BUILD_OPTS = -mod=mod
MANAGER_GO_ENV = GOPROXY=https://proxy.golang.org,direct

MANAGER_BUILD_TARGETS = ./cmd/manager

MANAGER_LDFLAGS = -X manager/internal/config.VERSION=$(MANAGER_VERSION)

$(eval $(golang-package))

define MANAGER_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/bin/manager $(TARGET_DIR)/opt/iiotgateway/bin/manager
endef