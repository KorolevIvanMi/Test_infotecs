#include "../Manager/Manager.h"
#include <iostream>
#include <limits>

void ShowLevel(int i){
    switch (i){
        case 1 : std::cout << "IMPORTANT\n"; break;
        case 2 : std::cout << "MEDIUM\n"; break;
        case 3 : std::cout << "UNIMPORTANT\n"; break;
    }
}

int main(int args, char* argv[]){
    manager::Manager mng("book", manager::Level::UNIMPORTANT) ; // менеджер
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
            case 3:
                {
                    manager::Level lvl = mng.GetDefaultLevel();
                    ShowLevel(int(lvl));
                }
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