#pragma once

#include <string>
#include <fstream>
#include <vector>

namespace manager {
    enum class Level{
        IMPORTANT = 1,
        MEDIUM = 2,
        UNIMPORTANT = 3
    };

    struct Message{
        std::string lvl;
        std::string data;
        std::string message;
    };

    class Manager{
    public:

        //  конструкторы 
        Manager();
        Manager(std::string, manager::Level );

        // основные методы класса
        bool Write(std::string, Level);
        std::vector<Message> Read();

        // Вспомогательные методы
        void ChangeDefaultLevel(Level);
        std::string ConvertRow(std::string , Level  ); 
        bool WriteToJournal(std::string);
        Level GetDefaultLevel();

        
    private:

        //  Поля класса
        std::string journalName;
        manager::Level defaultLevel;

        // Скрытые технические методы
        Message parseData(std::string);
    };
}