TARGET = $(notdir $(CURDIR))
BUILDDIR = $(abspath $(CURDIR)/build)

OPTIONS = -DIMGUI_IMPL_API="extern \"C\"" \
	  -DIMGUI_IMPL_OPENGL_LOADER_GLAD \
	  -DIMGUI_IMPL_OPENGL_LOADER_GLFW

INCLUDES = -Isrc \
	   -Ideps/glad/include/ \
	   -Ideps/glfw/include/ \
	   -Ideps/cglm/include/ \
	   -Ideps/glm/ \
	   -Ideps/cimgui/ \
	   -Ideps/cimgui/imgui \
	   -Ideps/cimgui/imgui/examples/ \
	   -Ideps/cimplot/ \
	   -Ideps/stb/

LINKS = -Ldeps/glfw/build/src/ \
	-Ldeps/glad/src/

CPPFLAGS = -Wall -std=c++11 $(OPTIMIZATION) $(OPTIONS) $(LINKS) $(INCLUDES)
CFLAGS = -Wall -Wextra -pedantic -std=gnu11 $(OPTIMIZATION) $(OPTIONS) $(LINKS) $(INCLUDES)

OPTIMIZATION=-O0

LDFLAGS = $(OPTIMIZATION) -Wl,-Ldeps/glfw/build/src/

SDL_FLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_ttf
PYTHON_FLAGS = $(shell python3-config --cflags --ldflags)

LIBS = -lm -lglfw -lpthread -ldl -lstdc++
LIBS += $(SDL_FLAGS)
LIBS += $(filter-out -DNDEBUG,$(PYTHON_FLAGS))

BLACKLIST = -O0 -O1 -O2 -O3 -Os
LIBS := $(filter-out $(BLACKLIST), $(LIBS))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  LIBS += -lGL
  ECHOFLAGS = -e
endif
ifeq ($(UNAME_S),Darwin)
  LIBS += -framework OpenGL
  CFLAGS += -Wno-unused-command-line-argument
  CPPFLAGS += -Wno-unused-command-line-argument -Wno-mismatched-tags
endif

LD_LIBRARY_PATH = deps/glfw/build/src/

CC = gcc
CXX = g++

C_FILES := $(wildcard src/*.c) deps/glad/src/glad.c
CPP_FILES := $(wildcard src/*.cpp)
STB_FILES := $(wildcard deps/stb/*.c)
IMGUI_FILES := $(wildcard ./deps/cimgui/*.cpp) \
  $(wildcard ./deps/cimgui/imgui/*.cpp) \
  $(wildcard ./deps/cimgui/imgui/examples/imgui_impl_glfw.cpp) \
  $(wildcard ./deps/cimgui/imgui/examples/imgui_impl_opengl3.cpp) \
  $(wildcard ./deps/cimplot/*.cpp) \
  $(wildcard ./deps/cimplot/implot/*.cpp)

SOURCES := $(STB_FILES:.c=.o) \
  $(CPP_FILES:.cpp=.o) \
  $(IMGUI_FILES:.cpp=.o) \
  $(C_FILES:.c=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: build

build: $(TARGET)

rebuild: clean $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CURDIR)/$(TARGET)

gdb: $(TARGET)
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) gdb $(CURDIR)/$(TARGET)

$(BUILDDIR)/%.o: %.c
	@echo $(ECHOFLAGS) "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) $(LIBS) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo $(ECHOFLAGS) "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(LIBS) -o "$@" -c "$<"

$(TARGET).o: $(OBJS) $(LDSCRIPT)
	@echo $(ECHOFLAGS) "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

$(TARGET): $(OBJS) $(LDSCRIPT)
	@echo $(ECHOFLAGS) "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)/src/"
	@rm -f "$(TARGET).o"

superclean:
	@echo Activating clean slate protocol
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).o"
