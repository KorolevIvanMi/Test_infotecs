COMPILER = g++
STANDART = -std=c++17
LIB_FLAGS = -fPIC -shared  -Wall -Wextra
APP_FLAGS = -Wall -Wextra -I

LIB = manager
LIB_DATA = Manager/Manager.cpp 
LIB_RES = build/Manager.so

APP = test_app
APP_DATA = App/main.cpp
APP_RES = build/main

$(LIB) : $(LIB_DATA)
	$(COMPILER) $(STANDART) -o $(LIB_RES) $(LIB_FLAGS) $(LIB_DATA)

$(APP) : $(LIB) $(APP_DATA)
	$(COMPILER) $(STANDART) -o $(APP_RES) $(APP_DATA) $(APP_FLAGS) ./Manager/ $(LIB_RES) 

all: manager test_app