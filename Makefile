CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra
INCLUDES = -I./include

TARGET_BT = solver_bt
TARGET_BB = solver_bb
TARGET_DP = solver_dp

SRC_DIR = src
OBJ_DIR = obj

# Common objects
COMMON_SRCS = $(SRC_DIR)/common.cpp
COMMON_OBJS = $(OBJ_DIR)/common.o

# Helper to compile object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Targets
all: bt bb dp

# Backtracking
bt: $(TARGET_BT)
$(TARGET_BT): $(COMMON_OBJS) $(OBJ_DIR)/backtracking.o $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DRUN_BT $(SRC_DIR)/main.cpp $(COMMON_OBJS) $(OBJ_DIR)/backtracking.o -o $(TARGET_BT)

# Branch and Bound
bb: $(TARGET_BB)
$(TARGET_BB): $(COMMON_OBJS) $(OBJ_DIR)/branch_and_bound.o $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DRUN_BB $(SRC_DIR)/main.cpp $(COMMON_OBJS) $(OBJ_DIR)/branch_and_bound.o -o $(TARGET_BB)

# Dynamic Programming
dp: $(TARGET_DP)
$(TARGET_DP): $(COMMON_OBJS) $(OBJ_DIR)/dynamic_programming.o $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DRUN_DP $(SRC_DIR)/main.cpp $(COMMON_OBJS) $(OBJ_DIR)/dynamic_programming.o -o $(TARGET_DP)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_BT) $(TARGET_BB) $(TARGET_DP) solver

rebuild: clean all
