#pragma once

#include <string>

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
        Manager(...);

        // деструктор
        ~Manager();

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
        std::string ConvertRow();
        std::string WriteToJournal();
        void ParseData();
    };


}