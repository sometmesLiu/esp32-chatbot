#include <cstdio>
#include <cstring>  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h" 
#include <memory>
#include "sd_card.h"
#include <cstdio>
#include <bits/shared_ptr.h>

char buffer[100];

extern "C" void app_main(void)
{  
    // std::make_shared<SdCard>() 的作用是创建一个 SdCard 类的智能指针对象。
    auto sdFile = std::make_shared<SdCard>();
    ESP_ERROR_CHECK(sdFile->open("aaa.txt", "w")); // 改为追加模式  
    sdFile->write_file("Hello World!", strlen("Hello World!"));  
    sdFile->read_file(buffer, sizeof(buffer));
    std::printf("buffer = %s\n", buffer);
    sdFile->close(); 
}

