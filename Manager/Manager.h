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
        bool Write(std::string&, Level) noexcept;
        [[nodiscard]] std::vector<Message> Read() const noexcept;

        // Вспомогательные методы
        void ChangeDefaultLevel(Level) noexcept;
        [[nodiscard]] std::string ConvertRow(std::string& , Level) const noexcept; 
        bool WriteToJournal(std::string&)const noexcept;
        [[nodiscard]] Level GetDefaultLevel()const noexcept;

        
    private:

        //  Поля класса
        std::string journalName;
        manager::Level defaultLevel;

        // Скрытые технические методы
        Message parseData(std::string&) const noexcept;
    };
}