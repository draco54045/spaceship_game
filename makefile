# Compiler settings
CXX_LINUX = g++
CXX_WIN = /usr/bin/x86_64-w64-mingw32-g++-win32
SDL_WIN_INC = -I$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/include/SDL2 -I$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/include/SDL2
SDL_WIN_LIB = -L$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/lib -L$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/lib
CXXFLAGS = -Wall -Wextra -std=c++17
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2) -lSDL2_image

# Targets
SRC = test.cpp
OUT_LINUX = ./build/game
OUT_WIN = ./build/game.exe

all: linux windows

linux:
	$(CXX_LINUX) $(CXXFLAGS) $(SRC) -o $(OUT_LINUX) $(SDL_FLAGS)

windows:
	$(CXX_WIN) $(CXXFLAGS) $(SRC) -o $(OUT_WIN) \
		$(SDL_WIN_INC) $(SDL_WIN_LIB) \
		-lmingw32 -lSDL2main -lSDL2 -lSDL2_image

clean:
	rm -f $(OUT_LINUX) $(OUT_WIN)

