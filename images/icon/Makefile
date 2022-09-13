#
# Dan Wilcox <danomatika@gmail.com> 2022
#

NAME=loaf
SRC_PNG=loaf-rounded.png
SRC-DEBUG_PNG=loaf-debug-rounded.png

DEST=../../bin/data

ICNS=icon.icns
ICONSET=icon.iconset

ICNS-DEBUG=icon-debug.icns
ICONSET-DEBUG=icon-debug.iconset

.PHONY: clean

all: $(ICNS) $(ICNS-DEBUG)

# macOS iconset
$(ICNS): $(SRC_PNG)
	mkdir $(ICONSET)
	sips -z 16 16     $(SRC_PNG) --out $(ICONSET)/icon_16x16.png
	sips -z 32 32     $(SRC_PNG) --out $(ICONSET)/icon_16x16@2x.png
	sips -z 32 32     $(SRC_PNG) --out $(ICONSET)/icon_32x32.png
	sips -z 64 64     $(SRC_PNG) --out $(ICONSET)/icon_32x32@2x.png
	sips -z 128 128   $(SRC_PNG) --out $(ICONSET)/icon_128x128.png
	sips -z 256 256   $(SRC_PNG) --out $(ICONSET)/icon_128x128@2x.png
	sips -z 256 256   $(SRC_PNG) --out $(ICONSET)/icon_256x256.png
	sips -z 512 512   $(SRC_PNG) --out $(ICONSET)/icon_256x256@2x.png
	sips -z 512 512   $(SRC_PNG) --out $(ICONSET)/icon_512x512.png
	cp                $(SRC_PNG)       $(ICONSET)/icon_512x512@2x.png
	iconutil -c icns $(ICONSET)
	rm -rf $(ICONSET)

# debug macOS iconset
$(ICNS-DEBUG): $(SRC-DEBUG_PNG)
	mkdir $(ICONSET-DEBUG)
	sips -z 16 16     $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_16x16.png
	sips -z 32 32     $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_16x16@2x.png
	sips -z 32 32     $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_32x32.png
	sips -z 64 64     $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_32x32@2x.png
	sips -z 128 128   $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_128x128.png
	sips -z 256 256   $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_128x128@2x.png
	sips -z 256 256   $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_256x256.png
	sips -z 512 512   $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_256x256@2x.png
	sips -z 512 512   $(SRC-DEBUG_PNG) --out $(ICONSET-DEBUG)/icon_512x512.png
	cp                $(SRC-DEBUG_PNG)       $(ICONSET-DEBUG)/icon_512x512@2x.png
	iconutil -c icns $(ICONSET-DEBUG)
	rm -rf $(ICONSET-DEBUG)

clean:
	rm -f $(ICNS) $(ICNS-DEBUG)
