#
# loaf: lua, osc, and openFrameworks
# 
# Makefile to generate custom bindings using SWIG
#
# 2016 Dan Wilcox <danomatika@gmail.com>
#

# swig command
SWIG = swig

# default output language, see swig -h for more
LANG = lua

# which module to build
TARGET = osc

# module SWIG interface file
INTERFACE = osc.i

# generated bindings filename
NAME = oscBindings

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# optional subdir within DEST_DIR
DEST_SUBDIR = .

# any extra SWIG flags per-language, etc
SWIG_FLAGS = -O -small

# path to OF root
OF_DIR = ../../../..

# C & C++ flags
CFLAGS = -I$(OF_DIR)/libs/openFrameworks -I$(OF_DIR)/addons -I$(DEST_DIR) \
         -DSWIG_NOINCLUDE

# no attribute by default, build with make ATTRIBUTES=true
ifeq ($(ATTRIBUTES), true)
	CFLAGS += -DATTRIBUTES
endif

.PHONY: all bindings symbols clean osc-prepare osc load-prepare loaf

all: osc loaf

# generates bindings
bindings:
	@echo "### Generating $(TARGET)"
	@mkdir -p $(DEST_DIR)
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) $(CFLAGS) -o $(DEST_DIR)/$(DEST_SUBDIR)/$(NAME).cpp $(INTERFACE)

# outputs debug symbols
symbols:
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CFLAGS) $(INTERFACE) > $(TARGET)_symbols.txt
	rm -f *.cxx

# clean up
clean:
	rm -f $(DEST_DIR)/*Bindings.cpp $(DEST_DIR)/macos/*Bindings.cpp *_symbols.txt

# osc bindings
osc-prepare:
	$(eval TARGET := osc)
	$(eval INTERFACE := osc.i)
	$(eval NAME := oscBindings)

osc: osc-prepare bindings

# loaf bindings
loaf-prepare:
	$(eval TARGET := loaf)
	$(eval INTERFACE := loaf.i)
	$(eval NAME := loafBindings)

loaf: loaf-prepare bindings

# syphon bindings
syphon-prepare:
	$(eval TARGET := syphon)
	$(eval INTERFACE := syphon.i)
	$(eval NAME := syphonBindings)
	$(eval DEST_SUBDIR := macos)
	mkdir -p $(DEST_DIR)/macos

syphon: syphon-prepare bindings
