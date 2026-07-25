#include "../Manager/Manager.h"
#include <iostream>
#include <ctime>
#include <filesystem>
#include <cassert>


// макрос для тестов 
#define TEST_ASSERT(expr)\
    if(!(expr)){ \
        std::cerr << "Ошибка: " << #expr <<" провален в " << __FILE__ << ":" << __LINE__ << std::endl; \
        return false; \
    }

// Вспомогательная функция для создания временного файла
std::string CreateTempFilename() {
    return "test_journal_" + std::to_string(std::time(nullptr)) + ".txt";
}

// удаление тестового журнала
void Cleanup(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

// ТЕСТЫ КОНСТРУКТОРОВ 
// тест контруктора по умолчанию
bool test_DefaultConstructor(){
    manager::Manager mng;
    TEST_ASSERT(mng.GetDefaultLevel() == manager::Level::UNIMPORTANT);
    return true;
}
// тест конктруктора с параметрами
bool test_ParameterizedConstructor(){
    std::string filename =  CreateTempFilename();
    manager::Manager mng(filename, manager::Level::IMPORTANT);
    TEST_ASSERT(mng.GetDefaultLevel() == manager::Level::IMPORTANT);
    Cleanup(filename);
    return true;
}

// ТЕСТЫ CONVERTROW
// преобразование строки с уровнем important
bool test_ConvertRow_ImportantLevel(){
    manager::Manager mng("test", manager::Level::IMPORTANT);
    std::string message = "Test importnant message";
    std::string result = mng.ConvertRow(message, manager::Level::IMPORTANT);

    TEST_ASSERT(!result.empty())
    TEST_ASSERT(result.find("1|") == 0);
    TEST_ASSERT(result.find(message) != std::string::npos);

    return true;
}
// преобразование строки с уровнем medium
bool test_ConvertRow_MediumLevel(){
    manager::Manager mng("test", manager::Level::MEDIUM);
    std::string message = "Test medium message";
    std::string result = mng.ConvertRow(message, manager::Level::MEDIUM);

    TEST_ASSERT(!result.empty())
    TEST_ASSERT(result.find("2|") == 0);
    TEST_ASSERT(result.find(message) != std::string::npos);

    return true;
}
// преобразование строки с уровнем unimportant
bool test_ConvertRow_UnimportantLevel(){
    manager::Manager mng("test", manager::Level::MEDIUM);
    std::string message = "Test unimportant message";
    std::string result = mng.ConvertRow(message, manager::Level::MEDIUM);

    TEST_ASSERT(!result.empty())
    TEST_ASSERT(result.find("3|") == 0);
    TEST_ASSERT(result.find(message) != std::string::npos);

    return true;
}
// преобразование строки с уровнем ниже заданного по умолчанию
bool test_ConvertRow_LevelBelowDefault(){
    manager::Manager mng("test", manager::Level::IMPORTANT);
    std::string message = "This should be rejected";
    std::string result = mng.ConvertRow(message, manager::Level::UNIMPORTANT);
    TEST_ASSERT(result.empty());
    return true;
}

// ТЕСТЫ WRITE TO JOURNAL
