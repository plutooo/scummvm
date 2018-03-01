TARGET := scummvm

ARCH     := -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIE
CXXFLAGS := $(CFLAGS) -fno-exceptions -std=gnu++11 -I$(DEVKITPRO)/libnx/include
ASFLAGS	 := -g $(ARCH)
LDFLAGS  += -specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -L$(DEVKITPRO)/libnx/lib

.PHONY: clean_switch

clean: clean_switch

clean_switch:
	$(RM) $(TARGET).nro
