#include "../Manager/Manager.h"
#include <iostream>
#include <ctime>
#include <filesystem>
#include <cassert>
#include <functional>
#include <vector>


// макрос для тестов 
#define TEST_ASSERT(expr)\
    if(!(expr)){ \
        std::cerr << "Ошибка: " << #expr <<" провален в " << __FILE__ << ":" << __LINE__ << std::endl; \
        return false; \
    }

// Вспомогательная функция для создания временного файла
std::string CreateTempFilename() {
    return "tests/test_journal_" + std::to_string(std::time(nullptr));
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
    TEST_ASSERT(result.find(" 1|") == 0);
    TEST_ASSERT(result.find(message) != std::string::npos);

    return true;
}
// преобразование строки с уровнем medium
bool test_ConvertRow_MediumLevel(){
    manager::Manager mng("test", manager::Level::MEDIUM);
    std::string message = "Test medium message";
    std::string result = mng.ConvertRow(message, manager::Level::MEDIUM);

    TEST_ASSERT(!result.empty())
    TEST_ASSERT(result.find(" 2|") == 0);
    TEST_ASSERT(result.find(message) != std::string::npos);

    return true;
}
// преобразование строки с уровнем unimportant
bool test_ConvertRow_UnimportantLevel(){
    manager::Manager mng("test", manager::Level::UNIMPORTANT);
    std::string message = "Test unimportant message";
    std::string result = mng.ConvertRow(message, manager::Level::UNIMPORTANT);

    TEST_ASSERT(!result.empty())
    TEST_ASSERT(result.find(" 3|") == 0);
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
// тест на создание новой записи
bool test_WriteToJournal_Success(){
    std::string filename = CreateTempFilename();
    manager::Manager mng (filename, manager::Level::MEDIUM);

    std::string value = "Test message";
    std::string message = mng.ConvertRow(value, manager::Level::IMPORTANT);
    bool result = mng.WriteToJournal(message);

    TEST_ASSERT(result);
    TEST_ASSERT(std::filesystem::exists(filename + ".txt"));
    
    std::ifstream file(filename + ".txt");
    std::string content;
    std::getline(file, content);
    TEST_ASSERT(content == message);
    
    Cleanup(filename + ".txt");
    return true;
}

bool test_WriteToJournal_AppendMode() {
    std::string filename = CreateTempFilename();
    manager::Manager mng(filename, manager::Level::MEDIUM);
    
    std::string value = "First";
    std::string msg1 = mng.ConvertRow(value, manager::Level::MEDIUM);
    value = "Second";
    std::string msg2 = mng.ConvertRow(value, manager::Level::MEDIUM);
    
    mng.WriteToJournal(msg1);
    mng.WriteToJournal(msg2);
    
    // Проверяем, что обе записи есть
    std::ifstream file(filename + ".txt");
    std::string line1, line2;
    std::getline(file, line1);
    std::getline(file, line2);
    
    TEST_ASSERT(line1 == msg1);
    TEST_ASSERT(line2 == msg2);
    
    Cleanup(filename + ".txt");
    return true;
}

bool test_Write_WithValidLevel(){
    std::string filename = CreateTempFilename();
    manager::Manager mng(filename, manager::Level::MEDIUM);

    std::string value = "Test value";
    bool result = mng.Write(value, manager::Level::MEDIUM);
    TEST_ASSERT(result);

    std::vector<manager::Message> messages = mng.Read();
    TEST_ASSERT(messages.size() == 1);
    TEST_ASSERT(messages[0].message == value);
    TEST_ASSERT(messages[0].lvl == " 2");

    Cleanup(filename+".txt");
    return true;
}

bool test_Write_WithInvalidLevel(){
    std::string filename = CreateTempFilename();
    manager::Manager mng(filename, manager::Level::MEDIUM);

    std::string value = "Test value";
    bool result = mng.Write(value, manager::Level::UNIMPORTANT);
    TEST_ASSERT(!result);

    std::vector<manager::Message> messages = mng.Read();
    TEST_ASSERT(messages.empty());

    Cleanup(filename+".txt");
    return true;
}

struct Test {
    std::string name;
    std::function<bool()> func;
};

int main() {
    std::vector<Test> tests = {
        {"DefaultConstructor", test_DefaultConstructor},
        {"ParameterizedConstructor", test_ParameterizedConstructor},
        {"ConvertRow_ImportantLevel", test_ConvertRow_ImportantLevel},
        {"ConvertRow_MediumLevel", test_ConvertRow_MediumLevel},
        {"ConvertRow_UnimportantLevel", test_ConvertRow_UnimportantLevel},
        {"ConvertRow_LevelBelowDefault", test_ConvertRow_LevelBelowDefault},
        {"WriteToJournal_Success", test_WriteToJournal_Success},
        {"WriteToJournal_AppendMode", test_WriteToJournal_AppendMode},
        {"Write_WithValidLevel", test_Write_WithValidLevel},
        {"Write_WithInvalidLevel", test_Write_WithInvalidLevel},
        // {"Read_EmptyFile", test_Read_EmptyFile},
        // {"Read_MultipleMessages", test_Read_MultipleMessages},
        // {"ChangeDefaultLevel", test_ChangeDefaultLevel},
        // {"GetDefaultLevel", test_GetDefaultLevel},
        // {"EndToEnd", test_EndToEnd},
    };
    
    int passed = 0;
    int failed = 0;
    
    for (const auto& test : tests) {
        std::cout << "Running: " << test.name << "... ";
        try {
            if (test.func()) {
                std::cout << "OK" << std::endl;
                passed++;
            } else {
                std::cout << "FAIL" << std::endl;
                failed++;
            }
        } catch (const std::exception& e) {
            std::cout << "EXCEPTION: " << e.what() << std::endl;
            failed++;
        }
    }
    
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << failed << std::endl;
    
    return failed > 0 ? 1 : 0;
}