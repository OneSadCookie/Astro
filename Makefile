APP := Astro

C_EXTS   := c m
CXX_EXTS := cc cpp cxx c++ mm
EXTS      = $(C_EXTS) $(CXX_EXTS)

ARCHS    := ppc i386 x86_64

CC_ppc    := gcc-4.0
CC_i386   := gcc-4.0
CC_x86_64 := gcc-4.2

CXX_ppc    := g++-4.0
CXX_i386   := g++-4.0
CXX_x86_64 := g++-4.2

CFLAGS_generic := -Wall -Wextra -Wno-unused-parameter -Wnewline-eof -Werror -O2 -gfull
CFLAGS_ppc    := -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 $(CFLAGS_generic)
CFLAGS_i386   := -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 $(CFLAGS_generic)
CFLAGS_x86_64 := -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5 $(CFLAGS_generic)

CXXFLAGS_ppc    := $(CFLAGS_ppc)
CXXFLAGS_i386   := $(CFLAGS_i386)
CXXFLAGS_x86_64 := $(CFLAGS_x86_64)

LDFLAGS_generic := $(patsubst %,-framework %,Cocoa OpenGL GLUT)
LDFLAGS_ppc    := $(LDFLAGS_generic)
LDFLAGS_i386   := $(LDFLAGS_generic)
LDFLAGS_x86_64 := $(LDFLAGS_generic)

define src_to_obj_1 #(arch,ext,sources)
$(patsubst %.$(2),build/$(1)/%.$(2).o,$(filter %.$(2),$(3)))
endef

define src_to_obj #(arch,sources)
$(foreach ext,$(EXTS),$(call src_to_obj_1,$(1),$(ext),$(2)))
endef

define c_rules #(arch,ext)
build/$(1)/%.$(2).o: %.$(2) Makefile
	mkdir -p $$(@D)
	$$(CC_$(1)) -arch $(1) $$(CFLAGS_$(1)) -c $$< -o $$@

build/$(1)/%.$(2).d: %.$(2) Makefile
	mkdir -p $$(@D)
	$$(CC_$(1)) -arch $(1) $$(CFLAGS_$(1)) -MM $$< |\
		perl -pe 's|((.*)\.o.*)|build/$(1)/$$$$2.d build/$(1)/$$$$1|' > $$@

endef

define cxx_rules #(arch,ext)
build/$(1)/%.$(2).o: %.$(2) Makefile
	mkdir -p $$(@D)
	$$(CXX_$(1)) -arch $(1) $$(CXXFLAGS_$(1)) -c $$< -o $$@

build/$(1)/%.$(2).d: %.$(2) Makefile
	mkdir -p $$(@D)
	$$(CXX_$(1)) -arch $(1) $$(CXXFLAGS_$(1)) -MM $$< |\
		perl -pe 's|((.*)\.o.*)|build/$(1)/$$$$2.d build/$(1)/$$$$1|' > $$@

endef

define per_arch_rules #(arch)
$(foreach ext,$(C_EXTS),$(call c_rules,$(1),$(ext)))
$(foreach ext,$(CXX_EXTS),$(call cxx_rules,$(1),$(ext)))

build/$(1)/$$(APP): $$(OBJECTS_$(1)) Makefile
	mkdir -p $$(@D)
	$$(CXX_$(1)) -arch $(1) $$(CXXFLAGS_$(1)) $$(LDFLAGS_$(1)) $$(OBJECTS_$(1)) -o $$@
endef

define find_source #(dir)
$(foreach ext,$(EXTS),$(wildcard $(1)/*.$(ext)))
endef

define find_obj_1 #(arch,sources)
OBJECTS_$(1) := $(call src_to_obj,$(1),$(2))
OBJECTS += $$(OBJECTS_$(1))
endef

define find_obj #(sources)
$(foreach arch,$(ARCHS),$(eval $(call find_obj_1,$(arch),$(1))))
endef

SOURCES := $(filter-out Source/_dummy%,$(call find_source,Source))
$(call find_obj,$(SOURCES))

$(foreach arch,$(ARCHS),$(eval $(call per_arch_rules,$(arch))))
EXES := $(foreach arch,$(ARCHS),build/$(arch)/$(APP))

INFO_PLIST := $(APP).app/Contents/Info.plist
PKGINFO := $(APP).app/Contents/PkgInfo
EXE := $(APP).app/Contents/MacOS/$(APP)

$(EXE): $(EXES) Makefile
	mkdir -p $(@D)
	lipo $(EXES) -create -output $@

$(INFO_PLIST): Info.plist
	mkdir -p $(@D)
	cat $< | perl -pe "s|SVNVERSION|`svn info | \
		grep 'Last Changed Rev' | \
		perl -pe 's|Last Changed Rev: (\d+)|\\1|'`|" > $@

$(PKGINFO):
	mkdir -p $(@D)
	echo 'APPL????' > $@

all: $(EXE) $(INFO_PLIST) $(PKGINFO)
	rsync -r --del --exclude '.*' Resources $(APP).app/Contents/

clean:
	rm -rf build

.DEFAULT_GOAL := all

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJECTS:.o=.d)
endif

Makefile:;
$(foreach ext,$(EXTS),$(eval %.$(ext):;))
%.h:;
%.hpp:;
%.hxx:;
%.h++:;
%.plist:;
