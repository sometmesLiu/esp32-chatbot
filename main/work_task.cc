#include "work_task.h"

void WorkTask::work_task_loop()
{
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
        printf("Work task loop\n");
    }
}

WorkTask::WorkTask(uint32_t stack_size)
{
    xTaskCreate([](void *pvParameters){
        WorkTask *work_task = static_cast<WorkTask*>(pvParameters);
        work_task->work_task_loop();
        vTaskDelete(NULL); 
    }, "work_task", stack_size, this, 3, NULL);
}

WorkTask::~WorkTask()
{
}
