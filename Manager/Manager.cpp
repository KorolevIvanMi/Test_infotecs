#include "Manager.h"

#include <ctime>
#include <iomanip>
#include <time.h>
#include <string>
#include <sstream>

// конструктор по умолчанию
manager::Manager::Manager(){
    this->defaultLevel = manager::Level::UNIMPORTANT;
    this->journalName = "journal";
}

// конструктор с параметрами
manager::Manager::Manager(std::string journalName, manager::Level defaultLevel): journalName{journalName} , defaultLevel{defaultLevel}{}

std::string manager::Manager::convertRow(std::string text,manager::Level level){
    std::string result = " ";
    time_t t= time(NULL);
    int lvl = static_cast<int>(level);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H:%M:%S");
    
    result.append(std::to_string(lvl));
    result.append(" ");
    result.append(ss.str());
    result.append(" ");
    result.append(text);
    return result;
}