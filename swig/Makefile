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
NAME = OscBindings

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# path to OF headers
OF_HEADERS = ../../../../libs/openFrameworks

# path to addons
ADDON_DIR = ../../../../addons

# C & C++ flags
CFLAGS = -I$(OF_HEADERS) -I$(ADDON_DIR) -I$(DEST_DIR) -DSWIG_NOINCLUDE
CXXFLAGS = $(CFLAGS)

.PHONY: all bindings debug clean

all: osc loaf

# generates bindings
bindings:

	@echo Generating target: $(TARGET)
	@echo LANG = $(LANG)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo NAME = $(NAME)
	@echo DEST_DIR = $(DEST_DIR)
	
	$(SWIG) -c++ -$(LANG) -fcompact -fvirtual $(CXXFLAGS) $(TARGET).i
	mv $(TARGET)_wrap.cxx $(NAME).cpp

# move generated files to DEST_DIR
move:
	mkdir -p $(DEST_DIR)
	mv *.cpp $(DEST_DIR)

# outputs debug symbols
debug:
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CXXFLAGS) $(TARGET).i > debug.txt
	rm -f *.cxx

# clean up
clean:
	rm -f $(DEST_DIR)/*Bindings.cpp
	rm -f debug.txt

# osc bindings
osc-prepare:
	$(eval TARGET := osc)
	$(eval NAME := OscBindings)

osc: osc-prepare bindings move

# loaf bindings
loaf-prepare:
	$(eval TARGET := loaf)
	$(eval NAME := LoafBindings)

loaf: loaf-prepare bindings move
