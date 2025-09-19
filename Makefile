# define the correct path and name
export PATH 		:= c:/raylib/w64devkit/bin:$(PATH)
RAYLIB_PATH         = c:/raylib/raylib
PROJECT_NAME        ?= GAMENAME
OUTDIR				= build/
SOURCE				= src/
DEBUGGING           ?= FALSE

RAYLIB_LIBTYPE      = STATIC
USE_EXTERNAL_GLFW   = FALSE

CC = gcc
MAKE = mingw32-make

ifeq ($(DEBUGGING), TRUE)
	CFLAGS += -g
	OUTDIR = build/debug/
else
	CFLAGS += -O2 -s -Wl,--gc-sections,--subsystem,windows
	OUTDIR = build/release/
endif

CFLAGS += -Wall -D_DEFAULT_SOURCE -Wno-missing-braces

INCLUDE_PATHS = -I.
LDFLAGS = -L.
LDLIBS = -lraylib -lopengl32 -lgdi32 -lcomdlg32 -lole32 -lwinmm

PROJECT_SOURCE_FILES ?= $(wildcard $(SOURCE)*.c $(SOURCE)**/*.c)

OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

MAKEFILE_PARAMS = $(PROJECT_NAME)

.PHONY: all

all: clean
	$(MAKE) $(PROJECT_NAME)
	$(OUTDIR)$(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(OUTDIR)$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS)

clean:
ifneq ($(wildcard $(SOURCE)*.o $(SOURCE)**/*.o),)
	rm $(wildcard $(SOURCE)*.o $(SOURCE)**/*.o)
endif

play:
	$(OUTDIR)$(PROJECT_NAME).$(EXT)
