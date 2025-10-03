# Compiler settings
CXX_LINUX = g++
CXX_WIN = /usr/bin/x86_64-w64-mingw32-g++-win32
SDL_WIN_INC = -I$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/include/SDL2 -I$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/include/SDL2
SDL_WIN_LIB = -L$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/lib -L$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/lib
CXXFLAGS = -Wall -Wextra -std=c++17 -Wno-unused-parameter
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2) -lSDL2_image

# Directories
INC_DIR = include
BUILD_DIR = build
SRC_DIR = src

# Source 
SRC = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp) main.cpp

# Targets
OUT_LINUX = $(BUILD_DIR)/game
OUT_WIN = $(BUILD_DIR)/game.exe

all: linux windows

linux:
	$(CXX_LINUX) $(CXXFLAGS) -I$(INC_DIR) $(SRC) -o $(OUT_LINUX) $(SDL_FLAGS)

windows:
	$(CXX_WIN) $(CXXFLAGS) -I$(INC_DIR) $(SRC) -o $(OUT_WIN) $(SDL_WIN_INC) $(SDL_WIN_LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

clean:
	rm -f $(OUT_LINUX) $(OUT_WIN)