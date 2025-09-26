#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h" 
#include "sd_card.h"
#include <string.h>
#include <bits/shared_ptr.h>
#include "wav_recorder.h"
#include "audio_es7210.h"

char buffer[100];

void print_all_tasks() {
    char task_list_buffer[1024]; // 缓冲区大小建议 >= 512 字节
    vTaskList(task_list_buffer); // 获取任务信息
    printf("Task List:\n%s\n", task_list_buffer);
} 
extern "C" void app_main(void)
{
    // auto sdFile = std::make_shared<SdCard>();
    // ESP_ERROR_CHECK(sdFile->open("vvv.wav","w+"));

    // auto audioEs7210 = std::make_shared<AudioEs7210>();

    // // // 通过构造函数注入依赖
    // WavRecorder recorder(sdFile, audioEs7210);
    // recorder.record(10);
    
    // sdFile->close();

    while (true)
    {
        print_all_tasks();
        vTaskDelay(pdMS_TO_TICKS(1000));    
        TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
        UBaseType_t stack_remaining = uxTaskGetStackHighWaterMark(main_task); 
        // vTaskResume(xTaskGetHandle("ipc0"));
        std::printf("app_main 栈剩余空间: %u 字节 \n",  
               stack_remaining * sizeof(StackType_t)); 
        const char* task_name = pcTaskGetName(main_task);
        std::printf("当前任务名称: %s \n", task_name);
    }
    
}
