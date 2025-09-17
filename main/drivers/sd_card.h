#pragma once
#include <esp_err.h>
 

 
#define TAG "SDCARD"
#define SD_CARD_PIN_CLK  GPIO_NUM_47
#define SD_CARD_PIN_CMD  GPIO_NUM_48
#define SD_CARD_PIN_D0   GPIO_NUM_21
#define MOUNT_POINT "/sdcard"
class SdCard
{
private:
    /* data */
public:
    SdCard();
    ~SdCard();
    esp_err_t sdWriteFile(char *path, char *data,bool isAppend=false);

    esp_err_t sdCardReadFile(const char *filename, char *output, size_t output_size);

    esp_err_t sdCardReadLine(const char *filename, int line_num, char *output, size_t output_size);
};

 