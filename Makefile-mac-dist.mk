# Makefile-mac-dist.mk
# assemble distribution zip or notarized dmg
#
# Copyright (c) 2022 ZKM | Hertz-Lab
# Dan Wilcox <dan.wilcox@zkm.de>
#
# BSD Simplified License.
# For information on usage and redistribution, and for a DISCLAIMER OF ALL
# WARRANTIES, see the file, "LICENSE.txt," in this distribution.
#
# This code has been developed at ZKM | Hertz-Lab as part of "The Intelligent
# Museum" generously funded by the German Federal Cultural Foundation.
#
# See https://github.com/zkmkarlsruhe/mac-dist-helper

# references:
# * https://developer.apple.com/documentation/security/notarizing_macos_software_before_distribution/customizing_the_notarization_workflow
# * https://scriptingosx.com/2021/07/notarize-a-command-line-tool-with-notarytool/
# * https://github.com/neurolabusc/NotarizeC
# * https://www.gnu.org/software/make/manual/html_node/Text-Functions.html

# makefile version
makefile.mac.dist.version = 0.3.0

# require make 3.81 as a minimum for modern functions like $(info), etc
ifneq ($(firstword $(sort 3.81 $(MAKE_VERSION))), 3.81)
  $(error GNU make version 3.81 or higher is required)
endif

# base app name, if building simple app
mac.app.name ?=

# base dist name, required
mac.dist.name ?= $(mac.app.name)
ifeq ($(mac.dist.name),)
$(error required mac.dist.name not set...)
endif

# macOS app bundles (.app)
mac.dist.apps ?=

# console programs
mac.dist.progs ?=

# dynamic libraries (.dylib)
mac.dist.libs ?=

# temp root build dir
mac.builddir ?= dist

## combined targets ############################################################

$(mac.builddir):
	mkdir -p $@

.PHONY: dist dist-zip dist-dmg dist-clean dist-clobber

# create a zip for distribution with notarized contents
dist-zip: distdir codesign zip notarize-zip staple zip-clean zip verify

# create and notarize dmg for distribution
dist-dmg: distdir codesign dmg notarize-dmg staple verify

# clean entire dist build directory
dist-clean:
	rm -rf $(mac.builddir)

# clean all dist outputs
dist-clobber: app-clean zip-clean dmg-clean

## app #########################################################################

# output single app?
mac.app ?= $(mac.app.name).app

.PHONY: app app-verify app-clean

# single app specified...
ifneq ($(mac.app.name)$(mac.app),.app)

mac.dist.apps += $(mac.app)

# xcode project
mac.app.project ?= $(mac.app.name).xcodeproj

# xcode build config: Debug or Release
mac.app.project.config ?= Release

# xcode build scheme
mac.app.project.scheme ?= $(mac.app.name)

app: $(mac.app)

# export a signed app
$(mac.app): | $(mac.builddir)
	@echo "===== app"
	echo '<?xml version="1.0" encoding="UTF-8"?>\n\
	<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n\
	<plist version="1.0">\n\
	<dict>\n\
	    <key>method</key>\n\
	    <string>developer-id</string>\n\
	</dict>\n\
	</plist>' > $(mac.builddir)/ExportOptions.plist
	xcodebuild -project $(mac.app.project) \
	           -config "$(mac.app.project.config)" \
	           -scheme "$(mac.app.project.scheme)" \
	           -archivePath $(mac.builddir)/$(mac.app.name).xcarchive archive
	xcodebuild -exportArchive -archivePath $(mac.builddir)/$(mac.app.name).xcarchive \
	           -exportOptionsPlist $(mac.builddir)/ExportOptions.plist -exportPath $(mac.builddir)
	mv $(mac.builddir)/$@ .

# verify the app is both signed and accepted by the SIP system aka Gatekeeper
app-verify: $(mac.app)
	@echo "===== app verify"
	codesign --verify --display -vv $<
	spctl --assess --type exec -v $<

app-clean:
	rm -rf $(mac.app)

else # ... nothing to do
app:
app-verify:
app-clean:
endif

## distdir #####################################################################

# temp dist dir
mac.dist.dir ?= $(mac.builddir)/dist

# main app or binaries to distribute (relative to main makefile dir)
mac.dist ?= $(strip $(mac.dist.apps) $(mac.dist.progs) $(mac.dist.libs))
ifeq ($(words $(mac.dist)), 0)
$(error required mac.dist empty! nothing to do...)
endif

# dist version, ex. "1.2.3"
mac.dist.version ?=

# dist name with version: name-version
ifeq ($(mac.dist.version),)
mac.dist.name.version ?= $(mac.dist.name)
else
mac.dist.name.version ?= $(mac.dist.name)-$(mac.dist.version)
endif 

# include these in the release dist package (relative to main makefile dir)
mac.dist.include ?=

# remove these from the release dist package (relative to main makefile dir)
mac.dist.exclude ?=

# is distribution a single app? 1 dist, 1 app, no includes = true
mac.dist.apponly ?= true
ifneq ($(words $(mac.dist)) $(words $(mac.dist.apps)) $(words $(mac.dist.include)), 1 1 0)
mac.dist.apponly = false
endif

# rsync dest subdir and zip/dmg srcdir
# * apponly true: .app bundle
# * apponly false: named subdir
ifeq ($(mac.dist.apponly),true) # single app
mac.dist.subdir = $(mac.dist.dir)
mac.dist.srcdir = $(mac.dist.dir)/$(firstword $(mac.dist.apps))
else
mac.dist.subdir = $(mac.dist.dir)/$(mac.dist.name.version)
mac.dist.srcdir = $(mac.dist.dir)/$(mac.dist.name.version)
endif

.PHONY: distdir distdir-clean

distdir: predistdir $(mac.dist.dir) postdistdir

# copy files into dist dir
# FIXME: this probably can't handle paths with spaces
$(mac.dist.dir): $(mac.builddir)
	@echo "===== distdir"
	mkdir -p $(mac.dist.subdir)
	rsync -a --exclude ".*" $(foreach exc,$(mac.dist.exclude), --exclude="$(exc)") \
		$(mac.dist) $(mac.dist.include) $(mac.dist.subdir)

# clean dist directory
distdir-clean:
	rm -rf $(mac.dist.dir)

# called before creating dist dir
predistdir::

# called after creating dist dir
postdistdir::

## codesign ####################################################################

# files to sign
mac.codesign ?= $(foreach path,$(mac.dist.progs) $(mac.dist.libs),$(mac.dist.srcdir)/$(path))

# codesign identity, usually a Developer ID Application string
# default: "-" aka ad-hoc
mac.codesign.identity ?= -

.PHONY: codesign codesign-verify codesign-identities

# codesign programs and libs
ifneq ($(mac.codesign),)

# entitlements plist which allows camera access, etc
mac.codesign.entitlements ?= $(mac.dist.name)/$(mac.dist.name).entitlements

# don't apply entitlements if not available
mac.codesign.flags = --force --options runtime --timestamp
ifneq ($(wildcard $(mac.codesign.entitlements)),)
mac.codesign.flags += --entitlements $(mac.codesign.entitlements)
endif

# codesign non-apps
codesign:
	@echo "===== codesign"
	@if test "$(mac.codesign.identity)" = "-" ; then echo "warning: signing using ad-hoc identity \"-\"" ; fi
	codesign --sign "Developer ID Application: $(mac.codesign.identity)" $(mac.codesign.flags) $(mac.codesign)

# verify code signature(s)
codesign-verify:
	@echo "===== codesign verify"
	codesign --verify --deep -vv $(mac.codesign)

else # nothing to do
codesign:
codesign-verify:
endif

# list available codesign identities
codesign-identities:
	@echo "===== codesign identities"
	security find-identity -p basic -v

## zip #########################################################################

# base zip name
mac.zip.name ?= $(mac.dist.name.version)

# zip file
mac.zip ?= $(mac.zip.name).zip

# zip content src directory
mac.zip.dir ?= $(mac.dist.srcdir)

.PHONY: zip zip-clean

zip: $(mac.zip)

# create zip
$(mac.zip):
	@echo "===== zip"
	ditto -c -k --keepParent $(mac.zip.dir) $(mac.zip)

# rm zip
zip-clean:
	rm -f $(mac.zip)

## dmg #########################################################################

# base dmg name
mac.dmg.name ?= $(mac.dist.name.version)

# dmg file
mac.dmg ?= $(mac.dmg.name).dmg

# dmg content src directory
mac.dmg.dir ?= $(mac.dist.srcdir)/..

# add a convenience link to /Applications inside the dmg?
mac.dmg.appslink ?= false

.PHONY: dmg ddmg-verify dmg-clean

dmg: $(mac.dmg)

# create dmg
$(mac.dmg):
	@echo "===== dmg"
	@if test "$(mac.dmg.appslink)" = "true" ; then ln -s /Applications $(mac.dmg.dir)/Applications ; fi
	hdiutil create -srcfolder $(mac.dmg.dir) -volname "$(mac.dmg.name)" -format UDZO -o $(mac.dmg)
	rm -f $(mac.dmg.dir)/Applications
	xattr -rc $(mac.dmg)
	@if test "$(mac.codesign.identity)" != "-" ; then \
		codesign --sign "Developer ID Application: $(mac.codesign.identity)" --timestamp -v $(mac.dmg) ; \
	else echo "warning: ad-hoc codesign identity \"-\", dmg will be unsigned" ; fi

# rm dmg
dmg-clean:
	rm -f $(mac.dmg)

## notarize ####################################################################

# binaries to notarize
mac.notarize ?= $(foreach path,$(mac.dist),$(mac.dist.srcdir)/$(path))

# Keychain profile name for App Store Connect app-specific password
mac.notarize.profile ?= AC_PASSWORD

# current submission upload path
mac.notarize.submit = $(shell grep -m 1 "path:" "$(mac.notarize.submit.log)" | awk '{print $$2}' || "")

# output during submission and processing
mac.notarize.submit.log = $(mac.builddir)/NotarizationSubmission.log

# current submission uuid
mac.notarize.submit.uuid = $(shell grep -m 1 "id:" $(mac.notarize.submit.log) | awk '{print $$2}' || "")

# current submission status, leading whitespace is important!
mac.notarize.submit.status = $(shell grep -m 1 "  status:" $(mac.notarize.submit.log) | awk '{print $$2}' || "")

# summary log after notarization is finished, either successfully or not
# check this for details on errors
mac.notarize.log = $(mac.builddir)/NotarizationSummary.json

.PHONY: notarize notarize-zip notarize-dmg \
        notarize-submit-zip notarize-submit-dmg \
        notarize-log notarize-staple notarize-history notarize-clean

notarize: notarize-dmg

notarize-zip: notarize-submit-zip notarize-log notarize-status

notarize-dmg: notarize-submit-dmg notarize-log notarize-status

# upload zip
notarize-submit-zip:
	@echo "===== notarize zip"
	rm -f $(mac.notarize.submit.log) $(mac.notarize.log)
	xcrun notarytool submit $(mac.zip) \
	    --keychain-profile "$(mac.notarize.profile)" \
	    --wait 2>&1 | tee $(mac.notarize.submit.log)

# upload dmg
notarize-submit-dmg:
	@echo "===== notarize dmg"
	rm -f $(mac.notarize.submit.log) $(mac.notarize.log)
	xcrun notarytool submit $(mac.dmg) \
	    --keychain-profile "$(mac.notarize.profile)" \
	    --wait 2>&1 | tee $(mac.notarize.submit.log)

# download notarization summary log
notarize-log: $(mac.notarize.log)
$(mac.notarize.log):
	@echo "===== notarize log"
	xcrun notarytool log $(mac.notarize.submit.uuid) \
	    --keychain-profile "$(mac.notarize.profile)" > $(mac.notarize.log)

# check status from logs and exit on error
notarize-status: $(mac.notarize.log)
	@echo "===== notarize status"
	@echo "submission $(mac.notarize.submit.status)"
	cat $(mac.notarize.log)
	@if [ "$(mac.notarize.submit.status)" != "Accepted" ]; then false ; fi

# print request history
notarize-history:
	@echo "===== notarize history"
	@xcrun notarytool history --keychain-profile "$(mac.notarize.profile)"

## staple ######################################################################

.PHONY: staple

# staple notarized binaries and dmg
# note: zip files cannot be notarized
staple:
	@echo "===== staple"
	stapler staple $(filter-out %.zip, $(mac.notarize) $(mac.notarize.submit))

## verify ######################################################################

.PHONY: verify

# verify signature and acceptance by the SIP system aka Gatekeeper
verify:
	@echo "===== verify"
	codesign --verify --deep -vv $(filter-out %.dmg %.zip, $(mac.notarize))
	$(foreach path,$(filter %.app, $(mac.notarize)),spctl --assess --type exec -v $(path))
	$(foreach path,$(filter %.dmg, $(mac.notarize.submit)),codesign --verify --deep -vv $(path))
	$(foreach path,$(filter %.dmg, $(mac.notarize.submit)),spctl --assess --type open --context context:primary-signature -v $(path)) 
