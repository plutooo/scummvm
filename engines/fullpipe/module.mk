MODULE := engines/fullpipe

MODULE_OBJS = \
	behavior.o \
	detection.o \
	fullpipe.o \
	gameloader.o \
	gfx.o \
	init.o \
	input.o \
	inventory.o \
	messages.o \
	modal.o \
	motion.o \
	ngiarchive.o \
	scene.o \
	scenes.o \
	sound.o \
	stateloader.o \
	statics.o \
	utils.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
