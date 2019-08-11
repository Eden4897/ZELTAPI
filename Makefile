################################################################################
######################### User configurable parameters #########################
# filename extensions
CEXTS:=c
ASMEXTS:=s S
CXXEXTS:=cpp c++ cc

# probably shouldn't modify these, but you may need them below
ROOT=.
FWDIR:=$(ROOT)/firmware
BINDIR=$(ROOT)/bin
SRCDIR=$(ROOT)/src
INCDIR=$(ROOT)/include

WARNFLAGS+=
EXTRA_CFLAGS=
EXTRA_CXXFLAGS=

# Set to 1 to enable hot/cold linking
USE_PACKAGE:=0

# Set this to 1 to add additional rules to compile your project as a PROS library template
IS_LIBRARY:=1
# TODO: CHANGE THIS!
LIBNAME:=ZELTAPI
VERSION:=0.9.1
# EXCLUDE_SRC_FROM_LIB= $(SRCDIR)/unpublishedfile.c
# this line excludes opcontrol.c and similar files
EXCLUDE_SRC_FROM_LIB+=$(foreach file, $(SRCDIR)/opcontrol $(SRCDIR)/initialize $(SRCDIR)/autonomous,$(foreach cext,$(CEXTS),$(file).$(cext)) $(foreach cxxext,$(CXXEXTS),$(file).$(cxxext)))

# files that get distributed to every user (beyond your source archive) - add
# whatever files you want here. This line is configured to add all header files
# that are in the the include directory get exported
TEMPLATE_FILES=$(INCDIR)/zeltapi/**/*.* $(INCDIR)/zeltapi.h $(INCDIR)/zeltapi/*.*
TEMPLATE_CONFIG_FILES=Makefile common.mk MakeProject.cmd .gitignore
USER_FILES=$(SRCDIR)/autoplan/*.* $(SRCDIR)/*.*

CREATE_TEMPLATE_FLAGS=$(foreach file,$(USER_FILES),--user "$(file)")
CREATE_TEMPLATE_FLAGS+=$(foreach file, $(TEMPLATE_CONFIG_FILES),--system "$(ROOT)/$(file)")
CREATE_TEMPLATE_FLAGS+=--destination ./../../vexLibrary/ZELTAPI@$(VERSION).zip

.DEFAULT_GOAL=quick

################################################################################
################################################################################
########## Nothing below this line should be edited by typical users ###########
-include ./common.mk
template: clean-template $(LIBAR)
	$(VV)mkdir -p $(TEMPLATE_DIR)
	@echo "Moving template files to $(TEMPLATE_DIR)"
	$Dcp --parents -r $(TEMPLATE_FILES) $(TEMPLATE_DIR)
	$Dcp --parents -r $(USER_FILES) $(TEMPLATE_DIR)
	$(VV)mkdir -p $(TEMPLATE_DIR)/bin
	$Dcp $(LIBAR) $(TEMPLATE_DIR)/bin
	$D$(foreach f, $(TEMPLATE_CONFIG_FILES), cp "$(ROOT)/template-$(f)" "$(TEMPLATE_DIR)/$(f)" &)
	@echo "Creating template"
	$Dprosv5 c create-template $(TEMPLATE_DIR) $(LIBNAME) $(VERSION) $(foreach file,$(TEMPLATE_FILES) $(LIBAR),--system "$(file)") --target v5 $(CREATE_TEMPLATE_FLAGS)
	-$Drm -rf $(TEMPLATE_DIR)
