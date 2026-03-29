# Include all external package .mk files (if you add packages later)
include $(sort $(wildcard $(BR2_EXTERNAL_IIOTGATEWAY_PATH)/package/*/*.mk))