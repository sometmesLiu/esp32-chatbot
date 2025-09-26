#pragma once
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "work_task.h"

class App
{
private:
    App();
    ~App(); 
    WorkTask* work_task=nullptr;

public:
    static App& GetInstance() {
        static App instance;
        return instance;
    }
    // 删除拷贝构造函数和赋值运算符
    App(const App&) = delete;
    App& operator=(const App&) = delete; 
    void run();

    void print_all_tasks();
};
