OBJECTS := $(patsubst Source/%,build/$(ARCH)/%.o,$(SOURCES))

ARCHFLAGS := -arch $(ARCH) -isysroot $(SDK) -mmacosx-version-min=$(MACOSX_VERSION_MIN)
CFLAGS += $(ARCHFLAGS)
CXXFLAGS += $(ARCHFLAGS)
LDFLAGS += $(ARCHFLAGS)

build/$(ARCH)/$(APP): $(OBJECTS) Arch.mk
	mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

# TODO more exts / metaprogram across $(C_EXTS)/$(CXX_EXTS)

build/$(ARCH)/%.c.o: Source/%.c Arch.mk
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d) -MP -MT '$@ $(@:.o=.d)'

build/$(ARCH)/%.m.o: Source/%.m Arch.mk
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d) -MP -MT '$@ $(@:.o=.d)'

build/$(ARCH)/%.cpp.o: Source/%.cpp Arch.mk
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d) -MP -MT '$@ $(@:.o=.d)'

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJECTS:.o=.d)
endif
