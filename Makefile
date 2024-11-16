CXX = clang++
CXXFLAGS = -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRC_DIR = src
OBJ_DIR = obj/src
BIN_DIR = bin


LIB_SRC = $(wildcard $(SRC_DIR)/libprojectabyss/*.cpp)
MAIN_SRC = $(wildcard $(SRC_DIR)/projectabyss/main.cpp)


LIB_OBJ = $(patsubst $(SRC_DIR)/libprojectabyss/%.cpp,$(OBJ_DIR)/libprojectabyss/%.o,$(LIB_SRC))
MAIN_OBJ = $(patsubst $(SRC_DIR)/projectabyss/%.cpp,$(OBJ_DIR)/projectabyss/%.o,$(MAIN_SRC))


TARGET = $(BIN_DIR)/projectabyss

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/libprojectabyss/%.o: $(SRC_DIR)/libprojectabyss/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/projectabyss/%.o: $(SRC_DIR)/projectabyss/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJ_DIR)/* $(TARGET)
