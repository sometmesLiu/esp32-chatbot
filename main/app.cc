#include "app.h"

void App::print_all_tasks()
{
    char task_list_buffer[1024];
    vTaskList(task_list_buffer);
    printf("Task List:\n%s\n", task_list_buffer);
}

App::App()
{
    printf("App::App()\n");
    work_task = new WorkTask(4096 * 2);
}

App::~App() {}

void App::run()
{
    printf("App::run()\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
    print_all_tasks();
}
