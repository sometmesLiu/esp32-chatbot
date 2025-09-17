#pragma once
#include <esp_err.h>
 
#include "file_interface.h"
 
#define TAG "SDCARD"
#define SD_CARD_PIN_CLK  GPIO_NUM_47
#define SD_CARD_PIN_CMD  GPIO_NUM_48
#define SD_CARD_PIN_D0   GPIO_NUM_21
#define MOUNT_POINT "/sdcard"


class SdCard : public FileInterface
{
private:
    FILE* m_file = nullptr;
public:
    SdCard();
    ~SdCard();
    esp_err_t open(const char* filename, const char* mode) override;
    esp_err_t close() override; 
    esp_err_t write_file(const char* data,size_t size ) override; 
    esp_err_t read_file( char *output, size_t output_size) override; 
    esp_err_t read_line(int line_num, char *output, size_t output_size) override;
    esp_err_t seek(size_t offset,SeekMode mode) override;
};
 