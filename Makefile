manager : Manager/Manager.cpp
	g++ -o build/Manager.so -fPIC -shared  Manager/Manager.cpp 

test_app : manager App/main.cpp
	g++ -o build/main App/main.cpp -I ./Manager/ build/Manager.so  

all: manager test_app