#
# Dan Wilcox <danomatika@gmail.com> 2022
#

NAME=loaf
SRC=icon-rounded.png
SRC-DEBUG=icon-debug-rounded.png

ICNS=icon.icns
ICONSET=icon.iconset

ICNS-DEBUG=icon-debug.icns
ICONSET-DEBUG=icon-debug.iconset

.PHONY: clean

all: $(ICNS) $(ICNS-DEBUG)

# macOS iconset
$(ICNS): $(SRC)
	mkdir $(ICONSET)
	sips -z 16 16     $(SRC) --out $(ICONSET)/icon_16x16.png
	sips -z 32 32     $(SRC) --out $(ICONSET)/icon_16x16@2x.png
	sips -z 32 32     $(SRC) --out $(ICONSET)/icon_32x32.png
	sips -z 64 64     $(SRC) --out $(ICONSET)/icon_32x32@2x.png
	sips -z 128 128   $(SRC) --out $(ICONSET)/icon_128x128.png
	sips -z 256 256   $(SRC) --out $(ICONSET)/icon_128x128@2x.png
	sips -z 256 256   $(SRC) --out $(ICONSET)/icon_256x256.png
	sips -z 512 512   $(SRC) --out $(ICONSET)/icon_256x256@2x.png
	sips -z 512 512   $(SRC) --out $(ICONSET)/icon_512x512.png
	cp                $(SRC)       $(ICONSET)/icon_512x512@2x.png
	iconutil -c icns $(ICONSET)
	rm -rf $(ICONSET)

# debug macOS iconset
$(ICNS-DEBUG): $(SRC-DEBUG)
	mkdir $(ICONSET-DEBUG)
	sips -z 16 16     $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_16x16.png
	sips -z 32 32     $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_16x16@2x.png
	sips -z 32 32     $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_32x32.png
	sips -z 64 64     $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_32x32@2x.png
	sips -z 128 128   $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_128x128.png
	sips -z 256 256   $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_128x128@2x.png
	sips -z 256 256   $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_256x256.png
	sips -z 512 512   $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_256x256@2x.png
	sips -z 512 512   $(SRC-DEBUG) --out $(ICONSET-DEBUG)/icon_512x512.png
	cp                $(SRC-DEBUG)       $(ICONSET-DEBUG)/icon_512x512@2x.png
	iconutil -c icns $(ICONSET-DEBUG)
	rm -rf $(ICONSET-DEBUG)

clean:
	rm -f $(ICNS) $(ICNS-DEBUG)
