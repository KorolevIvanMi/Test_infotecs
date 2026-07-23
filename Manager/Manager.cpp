#include "Manager.h"

#include <ctime>
#include <fstream>
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

// Превращает текст сообщения и уровень важности в строку для записи в журнал + добавляет дату записи
// также проверяет соотвествие уровня задач
// возврат пустой строки нужен для того, чтобы при реализации в приложении можно было отследить некорректный уровень
std::string manager::Manager::ConvertRow(std::string text,manager::Level level){

    if (static_cast<int>(level) < static_cast<int>(defaultLevel)) { 
        // необходимые переменные
        std::string result = " ";
        time_t t= time(NULL);
        int lvl = static_cast<int>(level); // получение значения из перечисления, так как хранить 1 символ лучше чем несколько, то храним номер важности

        // строка-поток для перевода времени в строку
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H:%M:%S");
        
        // формирование и возврат строки
        result.append(std::to_string(lvl));
        result.append(" ");
        result.append(ss.str());
        result.append(" ");
        result.append(text);
        return result;
    }
    return ""; //если важность задачи по умолчанию больше переданного, то возвращаем пустую строку
}

// записывает переданную строку в файл . Возвращает true есил всё получилось и flase, если возникли проблемы
bool manager::Manager::WriteToJournal(std::string text){
    std::string journal_name = journalName + ".txt"; // добавляем к названию журнала расширение файла
    std::ofstream journal(journal_name, std::ios::app | std::ios::ate); // открываем файл для чтения и переводим указатель в конец файла
    if (!journal.is_open()){
        return false; //если файл не открывается, то возвращаем неудачу
    }
    journal << text << "\n"; // записывает данные в файл
    journal.close(); // закрываем файл

    return true;
}

// обрабатывает и записывает данные в файл . По сути является объединением ConvertRow и WriteToJournal
// нужен для тех случаев, когда разработчику хочется вызвать один метод вместо двух. 
// только в таком случае при многопоточной работе уже внутри приложения нельзя пустить запись в файл под mutex и если и пускать, то и обработку текста сообщения
// так что в зависимости от требования приложения можно использовать или отдельную реализацию или объединнную
bool manager::Manager::Write(std::string text,manager::Level level){
    std::string line = ConvertRow(text,  level);
    if (line == ""){
        return false;
    }
    if (!WriteToJournal(line)) return false;
    return true;
}

