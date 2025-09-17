#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h" 
#include "sd_card.h" 

char buffer[3000];

extern "C" void batch_wirte_text(SdCard *sdcard) {
    char line_buffer[100];
    for (size_t i = 0; i < 100; i++)
    {
        std::printf("写入第%d行数据\n",i);
        snprintf(line_buffer,sizeof(line_buffer),"%d行数据[小猪快跑]\n", i);
        sdcard->sdWriteFile("test.txt",line_buffer, true);
    }

}
extern "C" void app_main(void)
{
    esp_log_level_set("TAG", ESP_LOG_DEBUG); 
    std::printf("Hello World!\n");
    //创建SD卡对象
    SdCard sdcard = SdCard();
    // 写入文件
    sdcard.sdWriteFile("test.txt", "开始批量写入",false);
    ESP_LOGI(TAG,"开始批量写入文件");
    batch_wirte_text(&sdcard);
    ESP_LOGI(TAG,"写入完毕，开始读取文件");

    // 读取文件
    sdcard.sdCardReadFile("test.txt",buffer,sizeof(buffer));

    ESP_LOGI(TAG,"SD的内容为：%s\n",buffer);

    char tmp[50];
    // 读取指定行的文件内容
    sdcard.sdCardReadLine("test.txt",20,tmp,sizeof(tmp));
    ESP_LOGI(TAG,"SD,第20行的内容为：%s\n",tmp);

     
}
