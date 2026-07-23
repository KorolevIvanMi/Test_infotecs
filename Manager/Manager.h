#pragma once

#include <string>
#include <fstream>

namespace manager {
    enum class Level{
        IMPORTANT = 1,
        MEDIUM = 2,
        UNIMPORTANT = 3
    };

    class Manager{
    public:

        //  конструкторы 
        Manager();
        Manager(std::string, manager::Level );

        // основные методы класса
        void Write(...);
        void Read(...);
        void ReadLastn(...);

        // Вспомогательные методы
        void ChangeDefaultLevel();
        
        
    private:

        //  Поля класса
        std::string journalName;
        manager::Level defaultLevel;

        // Скрытые технические методы
        std::string convertRow(std::string , Level );
        bool writeToJournal(std::string);
        void parseData();
    };
}