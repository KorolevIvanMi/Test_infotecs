#include "Manager.h"

// конструктор по умолчанию
manager::Manager::Manager(){
    this->defaultLevel = manager::Level::UNIMPORTANT;
    this->journalName = "journal";
}

// конструктор с параметрами
manager::Manager::Manager(std::string journalName, manager::Level defaultLevel): journalName{journalName} , defaultLevel{defaultLevel}{}

