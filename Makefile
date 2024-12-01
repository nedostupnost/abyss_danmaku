CXX = clang++
CXXFLAGS = -std=c++17 -I./src
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Находим все исходные файлы по новой структуре
CORE_SRC = $(wildcard $(SRC_DIR)/libprojectabyss/core/*.cpp)
ENTITIES_SRC = $(wildcard $(SRC_DIR)/libprojectabyss/entities/*.cpp)
STATES_SRC = $(wildcard $(SRC_DIR)/libprojectabyss/states/*.cpp)
MANAGERS_SRC = $(wildcard $(SRC_DIR)/libprojectabyss/managers/*.cpp)
MAIN_SRC = $(wildcard $(SRC_DIR)/projectabyss/main.cpp)

# Генерируем пути к объектным файлам
CORE_OBJ = $(patsubst $(SRC_DIR)/libprojectabyss/core/%.cpp,$(OBJ_DIR)/libprojectabyss/core/%.o,$(CORE_SRC))
ENTITIES_OBJ = $(patsubst $(SRC_DIR)/libprojectabyss/entities/%.cpp,$(OBJ_DIR)/libprojectabyss/entities/%.o,$(ENTITIES_SRC))
STATES_OBJ = $(patsubst $(SRC_DIR)/libprojectabyss/states/%.cpp,$(OBJ_DIR)/libprojectabyss/states/%.o,$(STATES_SRC))
MANAGERS_OBJ = $(patsubst $(SRC_DIR)/libprojectabyss/managers/%.cpp,$(OBJ_DIR)/libprojectabyss/managers/%.o,$(MANAGERS_SRC))
MAIN_OBJ = $(patsubst $(SRC_DIR)/projectabyss/%.cpp,$(OBJ_DIR)/projectabyss/%.o,$(MAIN_SRC))

# Финальная цель
TARGET = $(BIN_DIR)/projectabyss

# Основная цель сборки
all: $(TARGET)

# Линковка всех объектных файлов
$(TARGET): $(MAIN_OBJ) $(CORE_OBJ) $(ENTITIES_OBJ) $(STATES_OBJ) $(MANAGERS_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Правила компиляции для каждой подпапки
$(OBJ_DIR)/libprojectabyss/core/%.o: $(SRC_DIR)/libprojectabyss/core/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/libprojectabyss/entities/%.o: $(SRC_DIR)/libprojectabyss/entities/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/libprojectabyss/states/%.o: $(SRC_DIR)/libprojectabyss/states/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/libprojectabyss/managers/%.o: $(SRC_DIR)/libprojectabyss/managers/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/projectabyss/%.o: $(SRC_DIR)/projectabyss/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка сгенерированных файлов
clear:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/projectabyss

.PHONY: all clean
