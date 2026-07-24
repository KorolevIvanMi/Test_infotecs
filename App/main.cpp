#include "../Manager/Manager.h"
#include <cstring>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>
#include <mutex>


std::mutex mu; //мьютекс для блокирования потока

// Выводит уровень важности задачи по его номеру
void ShowLevel(int i);
// устанавливает новый уровень важности задач по умолчанию
void SetLevel(manager::Manager& mng);
// выводит все сообщения из журнала
void ShowMessages(manager::Manager mng);
// записывает сообщение в журнал
void WriteMessage(manager::Manager mng, std::string message, manager::Level lvl);
// разбирает параметры, переданные при старте приложения
void ParseArgs(int args, char* argv[], std::string&, int&);

// Очистка журнала
void CleanJournal(std::string);
// Вывод сообщений определённого уровня важности
void ShowSortedMessages(int);

// функция для выбора уровня важности
manager::Level choseLevel(manager::Manager mng);



int main(int args, char* argv[]){
    std::string journal_name;
    int base_lvl;
    ParseArgs(args, argv, journal_name, base_lvl);
    manager::Manager mng(journal_name, manager::Level(base_lvl)) ; // менеджер
    std::thread writer; //поток для отправки сообщений в журнал

    bool flag = false; // флаг для окончаиня работы программы
    bool write_success;

    int command = 0; //переменная для записи номера команды

    manager::Level lvl; //переменая для сохранения выбора уровня важности

    std::string message; // переменная для сохранения введённого сообщения

    // бесконечно крутим цикл с выбором команд до ввода команды 0
    do{
        system("clear"); // очистка консоли
        std::cout << "=================\t" << "Добро пожаловать в приложение\t" <<"================= \n";
        std::cout << "1. Внести сообщение в журнал. \n";
        std::cout << "2. Прочитать сообщения из журнала. \n";
        std::cout << "3. Прочитать текущий уровень важности по умолчанию. \n";
        std::cout << "4. Изменить уровень важности сообщений по умолчанию. \n";
        std::cout << "5. Очистить журнал. \n";
        std::cout << "0. Выйти. \n";

        std::cout << "Номер команды: ";
        std::cin >> command;
        switch (command) {
            case 0:
                flag = true;
                break;
            case 1:
                if (writer.joinable()) {
                    writer.join();
                }
                lvl = choseLevel(mng);
                std::cout << "Введите сообщение: ";
                std::getline(std::cin, message);
                writer = std::thread([mng, message, lvl](){
                    WriteMessage(mng,message,lvl);
                });

                break;
            case 2:
                ShowMessages(mng);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 3:
                {
                    manager::Level lvl = mng.GetDefaultLevel();
                    ShowLevel(int(lvl));
                    std::cout << "\n";
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 4:
                SetLevel(mng);
                break;
            case 5:
                CleanJournal(journal_name + ".txt");
                break;
            default:
                std::cout << "Такой команды не существует\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Нажмите любую клавишу...\n";
        std::cin.get();
    }while(!flag);

    if (writer.joinable()) {
        writer.join();
    }


}

void ShowLevel(int i){
    switch (i){
        case 1 : std::cout << "IMPORTANT"; break;
        case 2 : std::cout << "MEDIUM"; break;
        case 3 : std::cout << "UNIMPORTANT"; break;
    }
}

void ShowMessages(manager::Manager mng){
    std::unique_lock<std::mutex> locker(mu); // не даём другим потокам читать или записывать в файл в это время, чтобы не было неопределённого результата
    std::vector<manager::Message> messages = mng.Read();
    locker.unlock(); // снимаем блокировку, так как больше c файлом не работаем и можно дальше туда что-то записывать

    for (auto iter = messages.begin(); iter != messages.end();++iter){
        ShowLevel(std::stoi(iter->lvl));
        std::cout << ":::" << iter->data << ":::" << iter->message <<std::endl; 
    }
}

void SetLevel(manager::Manager& mng){
    manager::Level lvl = choseLevel(mng);
    mng.ChangeDefaultLevel(lvl);
}

manager::Level choseLevel(manager::Manager mng){
    manager::Level lvl; 
    int number;
    std::string input;
    std::cout << "\nВведите номер нужной важности: " << std::endl;
    std::cout << "1. IMPORTANT" << std::endl;
    std::cout << "2. MEDIUM" << std::endl;
    std::cout << "3. UNIMPORTANT" << std::endl;

    std::cout << "Введите номер (Enter - по умолчанию): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);
    if(input.empty()){
        lvl = mng.GetDefaultLevel();
    }else{
        number = std::stoi(input);
        if (number < 1 || number >3){
            lvl = mng.GetDefaultLevel();
        }else{
            lvl = manager::Level(number);
        }
    }
    

    return lvl;
}

void WriteMessage(manager::Manager mng, std::string message, manager::Level lvl){
    std::string res_message = mng.ConvertRow(message, lvl);
    // если возвращается пустая строка, т.е. добавляется задача с уровнем важности меньше чем заданный по умолчанию, то программа дальше не идет
    if (res_message == ""){ 
        std::cout << "Попытка добавить задачу, важность котороой меньше заданной по умолчанию\n";
        return;
    }
    std::unique_lock<std::mutex> locker(mu); //блокируем запись к файлу, чтобы другие потоки не могли читать и писать в файл
    bool flag = mng.WriteToJournal(res_message);
    locker.unlock();

    if (flag == false){
        std::cout << "Попытка записи в файл не удалась";
    }
}

void ParseArgs(int args, char *argv[], std::string& journal_name, int& base_lvl){
    if (args == 1){
        throw std::runtime_error("Неверные входные параметры !! введите --help для просмотра информации");
    }
    for(int i = 1; i < args; i++ ){
        if (std::strcmp(argv[i], "-jn") == 0 || std::strcmp(argv[i], "--journalname") == 0){
            if(i != args-1){
                journal_name = argv[i+1];
                i++;
            }
        }
        else if (std::strcmp(argv[i], "-dl") == 0 || std::strcmp(argv[i], "--defaultlevel") == 0){
            if(i != args-1){
                base_lvl = std::stoi(argv[i+1]);
                i++;
            }
        }
        else if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0){
            std::cout << "Использование: ./main -jn <name> -dl <1-3>\n";
            std::cout << "  -jn, --journalname <name>  Имя журнала\n";
            std::cout << "  -dl, --defaultlevel <1-3>  Уровень важности\n";
            std::cout << "  -h, --help                 Показать эту справку\n";
            exit(0);
        }
        else{
            throw std::runtime_error("Неверные входные параметры !! введите --help для просмотра информации");
        }
    }
}

void CleanJournal(std::string file){
    std::filesystem::resize_file(file, 0);
}