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

esp_err_t SdCard::open(const char* filename, const char* mode) {

    char  file_path[50]; 
    sprintf(file_path, "%s/%s", MOUNT_POINT, filename);

    m_file = fopen(file_path, mode);
    return m_file ? ESP_OK : ESP_FAIL;
}

esp_err_t SdCard::close() {
    if (m_file) {
        fclose(m_file);
        m_file = nullptr;
    }
    return ESP_OK;
}


esp_err_t SdCard::read_line(int line_num, char *output, size_t output_size)
{
    if (output_size < 2) {
        ESP_LOGE(TAG, "Output buffer size is too small");
        return ESP_FAIL;
    }

    // 检查文件是否打开
    if (!m_file) {
        ESP_LOGE(TAG, "File not opened");
        return ESP_FAIL;
    }

    // 保存当前文件位置
    long original_pos = ftell(m_file);
    ESP_LOGD(TAG, "Current file position before read: %ld", original_pos);
    
    // 重置到文件开始
    if (fseek(m_file, 0, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Failed to seek to beginning of file");
        return ESP_FAIL;
    }

    int current_line = 0;
    size_t buffer_size = 1024;  // 初始缓冲区大小
    char* line_buffer = (char*)malloc(buffer_size);
    
    if (!line_buffer) {
        ESP_LOGE(TAG, "Failed to allocate memory for line buffer");
        return ESP_ERR_NO_MEM;
    }

    // 逐行读取文件
    while (true) {
        size_t i = 0;
        int ch;
        
        // 逐字符读取直到换行符或文件结束
        while ((ch = fgetc(m_file)) != EOF) {
            // 如果缓冲区不够，扩展它
            if (i >= buffer_size - 1) {
                buffer_size *= 2;
                char* temp = (char*)realloc(line_buffer, buffer_size);
                if (!temp) {
                    free(line_buffer);
                    fseek(m_file, original_pos, SEEK_SET);  // 恢复文件位置
                    return ESP_ERR_NO_MEM;
                }
                line_buffer = temp;
            }
            
            if (ch == '\n') {
                break;  // 遇到换行符结束
            }
            
            line_buffer[i++] = (char)ch;
        }
        
        // 处理文件最后一行没有换行符的情况
        if (i > 0 || ch != EOF) {
            current_line++;
            
            // 如果当前行是目标行
            if (current_line == line_num) {
                line_buffer[i] = '\0';  // 字符串结束符
                
                // 复制到输出缓冲区
                strncpy(output, line_buffer, output_size - 1);
                output[output_size - 1] = '\0';
                
                free(line_buffer);
                fseek(m_file, original_pos, SEEK_SET);  // 恢复文件位置
                return ESP_OK;
            }
        }
        
        // 文件结束
        if (ch == EOF) {
            break;
        }
    }

    free(line_buffer);
    fseek(m_file, original_pos, SEEK_SET);  // 恢复文件位置
    
    // 如果文件行数不足
    ESP_LOGE(TAG, "File has only %d lines, requested line %d", current_line, line_num);
    return ESP_FAIL;
}


/**
 * 读取指定文件内容
 * @param filename : 文件路径
 * @param output : 输出缓冲区
 * @param output_size : 输出缓冲区大小
 */
esp_err_t SdCard::read_file(char *output, size_t output_size) {
 
    if (m_file == nullptr) {
        ESP_LOGE(TAG, "File not opened");
        return ESP_ERR_INVALID_STATE;
    } 
    // 获取当前文件指针位置
    long current_pos = ftell(m_file);
    ESP_LOGD(TAG, "Current file position before read: %ld", current_pos); 
    // 获取文件大小
    if (fseek(m_file, 0, SEEK_END) != 0) {
        ESP_LOGE(TAG, "Failed to seek to end");
        return ESP_FAIL;
    } 
    long file_size = ftell(m_file);
    if (file_size < 0) {
        ESP_LOGE(TAG, "Failed to get file size");
        return ESP_FAIL;
    } 
    // 重置文件指针
    if (fseek(m_file, 0, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Failed to seek to start");
        return ESP_FAIL;
    } 
    // 检查缓冲区大小
    if (output_size < (size_t)(file_size + 1)) {
        ESP_LOGE(TAG, "Buffer too small (need %ld, have %d)", 
                file_size + 1, output_size);
        return ESP_ERR_INVALID_SIZE;
    } 
    // 读取文件内容
    size_t bytes_read = fread(output, 1, file_size, m_file);
    output[bytes_read] = '\0';  // 确保字符串终止
  

    ESP_LOGI(TAG, "Read %zu/%ld bytes: %.*s", 
             bytes_read, file_size, 
             (int)(bytes_read > 50 ? 50 : bytes_read), output);

    if (bytes_read != (size_t)file_size) {
        ESP_LOGW(TAG, "Partial read (expected %ld, got %zu)", 
                file_size, bytes_read);
    }

    return ESP_OK;
}

 

/**
 * 像指定文件写入数据
 * @param filename : 文件路径
 * @param data : 要写入的数据
 * @param isAppend : 是否为追内容，true:追加内容  false:覆盖内容
 */
esp_err_t SdCard::write_file(const char*  data,size_t size)
{ 
    
    return fwrite(data, 1, size, m_file) == size ? ESP_OK : ESP_FAIL;  
}

esp_err_t SdCard::seek(size_t offset,SeekMode mode) {
    if (!m_file) {
        return ESP_ERR_INVALID_STATE; // 文件未打开
    }

    int origin;
    switch (mode) {
        case SeekMode::SET: origin = SEEK_SET; break;
        case SeekMode::CUR: origin = SEEK_CUR; break;
        case SeekMode::END: origin = SEEK_END; break;
        default: return ESP_ERR_INVALID_ARG;
    }

    if (fseek(m_file, offset, origin) != 0) {
        return ESP_FAIL; // 移动失败
    }

    return ESP_OK;
}
