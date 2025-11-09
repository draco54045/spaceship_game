# Compiler settings
CXX_LINUX = g++
CXX_WIN = /usr/bin/x86_64-w64-mingw32-g++
SDL_WIN_INC = -I$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/include/SDL2 -I$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/include/SDL2 -I$(HOME)/Documents/Libraries/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2 -I$(HOME)/Documents/Libraries/SDL2_mixer-2.8.1/x86_64-w64-mingw32/include/SDL2
SDL_WIN_LIB = -L$(HOME)/Documents/Libraries/SDL2-2.32.10/x86_64-w64-mingw32/lib -L$(HOME)/Documents/Libraries/SDL2_image-2.8.8/x86_64-w64-mingw32/lib -L$(HOME)/Documents/Libraries/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib -L$(HOME)/Documents/Libraries/SDL2_mixer-2.8.1/x86_64-w64-mingw32/lib
CXXFLAGS = -Wall -Wextra -std=c++17 -Wno-unused-parameter -Wno-unused-variable
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2) -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Directories
INC_DIR = include
BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = obj

# Sources and Objects
SRC = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJ_LINUX = $(patsubst %.cpp,$(OBJ_DIR)/linux/%.o,$(SRC))
OBJ_WIN   = $(patsubst %.cpp,$(OBJ_DIR)/windows/%.o,$(SRC))

# Targets
OUT_LINUX = $(BUILD_DIR)/game
OUT_WIN = $(BUILD_DIR)/game.exe

all: linux windows

# Linux build
linux: $(OUT_LINUX)

$(OUT_LINUX): $(OBJ_LINUX)
	$(CXX_LINUX) $(CXXFLAGS) $^ -o $@ $(SDL_FLAGS)

# Windows build
windows: $(OUT_WIN)

$(OUT_WIN): $(OBJ_WIN)
	$(CXX_WIN) $(CXXFLAGS) -I$(INC_DIR) $^ -o $@ \
		$(SDL_WIN_INC) $(SDL_WIN_LIB) \
		-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Compile rules
$(OBJ_DIR)/linux/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_LINUX) $(CXXFLAGS) -I$(INC_DIR) $(SDL_FLAGS) -c $< -o $@

$(OBJ_DIR)/windows/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_WIN) $(CXXFLAGS) -I$(INC_DIR) $(SDL_WIN_INC) -c $< -o $@

clean:
	rm -rf $(OUT_LINUX) $(OUT_WIN) $(OBJ_DIR)