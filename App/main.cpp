#include "../Manager/Manager.h"
#include <iostream>
#include <limits>
#include <thread>
#include <mutex>


std::mutex mu; //мьютекс для блокирования потока

// Выводит уровень важности задачи по его номеру
void ShowLevel(int i);
// устанавливает новый уровень важности задач по умолчанию
void SetLevel(manager::Manager& mng);
// выводит все сообщения из журнала
void ShowMessages(manager::Manager& mng);
// записывает сообщение в журнал
void WriteMessage();
// разбирает параметры переданные при старте приложения
void ParseArgs();

int main(int args, char* argv[]){
    manager::Manager mng("journal", manager::Level::UNIMPORTANT) ; // менеджер
    std::thread writer; //поток для отправки сообщений в журнал
    
    bool flag = false; // флаг для окончаиня работы программы

    int command = 0; //переменная для записи номера команды
    
    // бесконечно крутим цикл с выбором команд до ввода команды 0
    do{
        system("clear"); // очистка консоли
        std::cout << "=================\t" << "Добро пожаловать в приложение\t" <<"================= \n";
        std::cout << "1. Внести сообщение в журнал. \n";
        std::cout << "2. Прочитать сообщения из журнала. \n";
        std::cout << "3. Прочитать текущий уровень важности по умолчанию. \n";
        std::cout << "4. Изменить уровень важности сообщений по умолчанию. \n";
        std::cout << "0. Выйти. \n";

        std::cout << "Номер команды: ";
        std::cin >> command;
        switch (command) {
            case 0:
                flag = true;
                break;
            case 2:
                ShowMessages(mng);
                break;
            case 3:
                {
                    manager::Level lvl = mng.GetDefaultLevel();
                    ShowLevel(int(lvl));
                    std::cout << "\n";
                }
                break;
            case 4:
                SetLevel(mng);
                break;
            default:
                std::cout << "Такой команды не существует\n";
        }
        std::cout << "Нажмите любую клавишу...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }while(!flag);
    // тут дожидаться конца потока

}

void ShowLevel(int i){
    switch (i){
        case 1 : std::cout << "IMPORTANT"; break;
        case 2 : std::cout << "MEDIUM"; break;
        case 3 : std::cout << "UNIMPORTANT"; break;
    }
}

void ShowMessages(manager::Manager& mng){
    std::unique_lock<std::mutex> locker(mu); // не даём другим потокам читать или записывать в файл в это время, чтобы не было неопределённого результата
    std::vector<manager::Message> messages = mng.Read();
    locker.unlock(); // снимаем блокировку, так как больше c файлом не работаем и можно дальше туда что-то записывать

    for (auto iter = messages.begin(); iter != messages.end();++iter){
        ShowLevel(std::stoi(iter->lvl));
        std::cout << ":::" << iter->data << ":::" << iter->message <<std::endl; 
    }
}

void SetLevel(manager::Manager& mng){
    manager::Level lvl;
    int number;
    std::cout << "\nВведите номер нужной важности: " << std::endl;
    std::cout << "1. IMPORTANT" << std::endl;
    std::cout << "2. MEDIUM" << std::endl;
    std::cout << "3. UNIMPORTANT" << std::endl;

    std::cout << "Введите номер: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin >> number;

    if (number < 1 || number >3){
        lvl = manager::Level::UNIMPORTANT;
    }else{
        lvl = static_cast<manager::Level>(number);
    }
    
    
    mng.ChangeDefaultLevel(lvl);
}