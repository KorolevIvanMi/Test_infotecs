COMPILER = g++
STANDART = -std=c++17


LIB = manager
LIB_DATA = Manager/Manager.cpp 
LIB_RES = build/Manager.so
LIB_FLAGS = -fPIC -shared  -Wall -Wextra

APP = test_app
APP_DATA = App/main.cpp
APP_RES = build/app
APP_FLAGS = -Wall -Wextra -I

TEST = test_manager
TEST_DATA = Manager/Manager.cpp tests/test_manager.cpp
TEST_RES = tests/test_manager
TEST_FLAGS = -Wall -Wextra

$(LIB) : $(LIB_DATA)
	$(COMPILER) $(STANDART) -o $(LIB_RES) $(LIB_FLAGS) $(LIB_DATA)

$(APP) : $(LIB) $(APP_DATA)
	$(COMPILER) $(STANDART) -o $(APP_RES) $(APP_DATA) $(APP_FLAGS) ./Manager/ $(LIB_RES) 

$(TEST) : $(TEST_DATA)
	$(COMPILER) $(STANDART)  $(TEST_DATA) $(TEST_FLAGS) -o $(TEST_RES)

test: $(TEST)
	tests/test_manager
	rm tests/test_manager

all: manager test_app