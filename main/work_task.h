#pragma once
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class WorkTask
{
    private:

    public:
    WorkTask(uint32_t stack_size);
    ~WorkTask();

    void work_task_loop();
};
