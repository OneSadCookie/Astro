MAKEFLAGS := Rr $(shell echo -j$$(((1 + 3 * `sysctl -n hw.ncpu`) / 2)))

export APP := Astro
ARCHS      := ppc i386 x86_64

C_EXTS   := c m
CXX_EXTS := cc cpp cxx c++ mm
SRC_EXTS  = $(C_EXTS) $(CXX_EXTS)
H_EXTS   := h
HXX_EXTS := hh hpp hxx h++

BUNDLE     := $(APP).app
CONTENTS   := $(BUNDLE)/Contents
EXE        := $(CONTENTS)/MacOS/$(APP)
INFO_PLIST := $(CONTENTS)/Info.plist
PKGINFO    := $(CONTENTS)/PkgInfo

export CFLAGS   := -Wall -Wextra -Wno-unused-parameter -Wnewline-eof -Werror -O2 -gfull -pipe -mdynamic-no-pic
export CXXFLAGS  = $(CFLAGS) -fno-exceptions -fno-rtti
export LDFLAGS  := $(patsubst %,-framework %,Cocoa OpenGL GLUT)

define find_source #(dir)
$(foreach ext,$(SRC_EXTS),$(wildcard $(1)/*.$(ext)))
endef

export SOURCES := $(filter-out Source/_dummy%,$(call find_source,Source))
THIN_EXES := $(patsubst %,build/%/$(APP),$(ARCHS))

build/%/$(APP): $(SOURCES) Makefile Arch.mk
	./choosesdk.rb $* --run "$(MAKE) -f Arch.mk $@ ARCH=$*"

$(EXE): $(THIN_EXES) Makefile
	mkdir -p $(@D)
	lipo $(THIN_EXES) -create -output $@

$(INFO_PLIST): Info.plist Makefile
	mkdir -p $(@D)
	cat $< | perl -pe "s|SVNVERSION|`svn info | \
		grep 'Last Changed Rev' | \
		perl -pe 's|Last Changed Rev: (\d+)|\\1|'`|" > $@

$(PKGINFO): Makefile
	mkdir -p $(@D)
	echo 'APPL????' > $@

all: Makefile $(EXE) $(INFO_PLIST) $(PKGINFO)
	rsync -r --del --exclude '.*' Resources $(CONTENTS)/

Astro.dmg: Makefile all
	dmgcanvas Astro.dmgCanvas Astro.dmg -leopard-compatible

clean:
	rm -rf build $(APP)

.DEFAULT_GOAL := all
.PHONY: all clean
