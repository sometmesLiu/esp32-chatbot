#include  <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_system.h"
#include "esp_log.h"


void my_task(void *pvParameters) {
    while (1) {
        ESP_LOGI("MY_TASK", "任务正在运行");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 延时1秒
    }
}


extern "C" void app_main() {
    xTaskCreate(
        my_task,           // 任务函数
        "MyTask",          // 任务名称
        configMINIMAL_STACK_SIZE,              // 堆栈大小（字节）
        NULL,              // 传递给任务的参数
        5,                 // 优先级
        NULL               // 任务句柄
    );

    xTaskCreatePinnedToCore(
        my_task,           // 任务函数
        "MyTaskPinned",    // 任务名称
        configMINIMAL_STACK_SIZE,              // 堆栈大小（字节）
        NULL,              // 传递给任务的参数
        5,                 // 优先级
        NULL,             // 任务句柄
        0                  // 核心ID（0或1）
    );
}








