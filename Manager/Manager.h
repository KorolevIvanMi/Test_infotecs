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
        bool Write(std::string, Level);
        void Read(...);
        void ReadLastN(...);

        // Вспомогательные методы
        void ChangeDefaultLevel(Level);
        std::string ConvertRow(std::string , Level ); 
        bool WriteToJournal(std::string);
        Level GetDefaultLevel();

    private:

        //  Поля класса
        std::string journalName;
        manager::Level defaultLevel;

        // Скрытые технические методы
        std::string parseData(std::string);
    };
}