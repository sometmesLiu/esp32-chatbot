

#include "sd_card.h" 
#include "esp_log.h"
#include "esp_err.h"
#include "driver/sdmmc_host.h" 
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h" 
#include <cstdio>
#include <cstring>

SdCard::SdCard()
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.cmd = SD_CARD_PIN_CMD;
    slot_config.clk = SD_CARD_PIN_CLK;
    slot_config.d0 = SD_CARD_PIN_D0;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount SD card VFAT filesystem.");
        ESP_LOGD(TAG, "SD card error: %s", esp_err_to_name(ret));
        /* code */
        ESP_LOGI(TAG, "SD card initialized failed.");
        
    }
    
    ESP_LOGI(TAG, "SD card initialized.");

    //打印SD卡信息
    sdmmc_card_print_info(stdout, card);

    // return ESP_OK;

}

SdCard::~SdCard()
{
}


/** 读取指定行数据
 * file_path : 文件路径
 * line_num : 行号
 * output : 输出缓冲区
 * output_size : 输出缓冲区大小
 */
esp_err_t SdCard::sdCardReadLine(const char *filename, int line_num, char *output, size_t output_size)
{
    char line[1000]; // 假设每行最大长度为 1000 字符
    int current_line = 0;

    char  file_path[50];
    char data1[100];
    sprintf(file_path, "%s/%s", MOUNT_POINT, filename);

    if (output_size>(sizeof(line)-1))
    {
        return ESP_FAIL;
    }
    // 检查输出缓冲区是否足够大
    if (output_size < 1) {
        ESP_LOGE(TAG, "Output buffer size is too small");
        return ESP_FAIL;
    }

    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    // 逐行读取文件
    while (fgets(line, sizeof(line), f) != NULL) {
        current_line++; 
        // 如果当前行是目标行
        if (current_line == line_num) {
            // 去掉行末的换行符（如果有）
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0'; // 去掉换行符
            } 
            // 将目标行内容复制到输出缓冲区
            strncpy(output, line, output_size - 1);
            output[output_size - 1] = '\0'; // 确保字符串以 null 结尾
            fclose(f); 
            return ESP_OK;
        }
    }

    fclose(f);

    // 如果文件行数不足
    if (current_line < line_num) {
        ESP_LOGE(TAG, "File has only %d lines, requested line %d", current_line, line_num);
        return ESP_FAIL;
    }

    return ESP_FAIL;
}


/**
 * 读取指定文件内容
 * @param filename : 文件路径
 * @param output : 输出缓冲区
 * @param output_size : 输出缓冲区大小
 */
esp_err_t SdCard::sdCardReadFile(const char *filename, char *output, size_t output_size) {


    char  file_path[50];
    char data1[100];
    sprintf(file_path, "%s/%s", MOUNT_POINT, filename);


    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    // 获取文件大小
    fseek(f, 0, SEEK_END); // 移动到文件末尾
    long file_size = ftell(f); // 获取文件大小
    fseek(f, 0, SEEK_SET); // 回到文件开头

    if (file_size < 0) {
        ESP_LOGE(TAG, "Failed to get file size");
        fclose(f);
        return ESP_FAIL;
    }

    // 检查缓冲区是否足够大
    if (output_size < (size_t)(file_size + 1)) { // +1 用于字符串结束符
        ESP_LOGE(TAG, "Output buffer is too small (required: %ld, provided: %d)", file_size + 1, output_size);
        fclose(f);
        return ESP_FAIL;
    }

    // 读取文件内容
    size_t bytes_read = fread(output, 1, file_size, f);
    if (bytes_read != file_size) {
        ESP_LOGE(TAG, "Failed to read file content");
        fclose(f);
        return ESP_FAIL;
    }

    // 添加字符串结束符
    output[file_size] = '\0';

    fclose(f);
    ESP_LOGI(TAG, "Read entire file: %ld bytes", file_size);
    return ESP_OK;
}

 

/**
 * 像指定文件写入数据
 * @param filename : 文件路径
 * @param data : 要写入的数据
 * @param isAppend : 是否为追内容，true:追加内容  false:覆盖内容
 */
esp_err_t SdCard::sdWriteFile(char *filename, char *data,bool isAppend)
{
    esp_err_t ret; 
    char  file_path[50];
    char data1[100];
    sprintf(file_path, "%s/%s", MOUNT_POINT, filename); 
   
    FILE *f =NULL;
    if(isAppend)
    {
        f = fopen(file_path, "a");
    }else{
        f = fopen(file_path, "w");
    }
    
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f , data);
    fclose(f);
    ESP_LOGI(TAG, "File written");
    return ESP_OK;
}