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

# generated bindings filename
NAME = oscBindings

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# any extra SWIG flags per-language, etc
SWIG_FLAGS = -O -small

# path to OF headers
OF_HEADERS = ../../../../libs/openFrameworks

# path to addons
ADDON_DIR = ../../../../addons

# C & C++ flags
CFLAGS = -I$(OF_HEADERS) -I$(ADDON_DIR) -I$(DEST_DIR) -DSWIG_NOINCLUDE

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
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) $(CFLAGS) -o $(DEST_DIR)/$(NAME).cpp $(TARGET).i

# outputs debug symbols
symbols:
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CFLAGS) $(TARGET).i > $(TARGET)_symbols.txt
	rm -f *.cxx

# clean up
clean:
	rm -f $(DEST_DIR)/*Bindings.cpp
	rm -f *_symbols.txt

# osc bindings
osc-prepare:
	$(eval TARGET := osc)
	$(eval NAME := oscBindings)

osc: osc-prepare bindings

# loaf bindings
loaf-prepare:
	$(eval TARGET := loaf)
	$(eval NAME := loafBindings)

loaf: loaf-prepare bindings
