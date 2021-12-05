.TARGET: build

SRC_PATH = src
BIN_PATH = build/bin
OBJ_PATH = build/obj

CXX = g++
CFLAGS = --std=c++17 -Wall
LFLAGS = --std=c++17 -Wall

SOURCES = $(wildcard $(SRC_PATH)/*.cpp)
OBJECTS = $(addprefix $(OBJ_PATH)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))
BIN_NAME = main

build: $(OBJECTS)
	$(CXX) -o $(BIN_PATH)/$(BIN_NAME) $(LFLAGS) $^

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) -o $(OBJ_PATH)/$(notdir $@) -c $^ $(CFLAGS)

run: build
	./$(BIN_PATH)/$(BIN_NAME)

clean:
	rm -fv $(OBJ_PATH)/* $(BIN_PATH)/*

clean_obj:
	rm -fv $(OBJ_PATH)/*
