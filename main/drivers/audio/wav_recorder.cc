#include "wav_recorder.h"
#include "format_wav.h"
#include "audio_es7210.h"
#include <esp_timer.h>
#include <esp_check.h>

#define TAG "WavRecorder"

WavRecorder::WavRecorder(std::shared_ptr<FileInterface> fileInterface) 
: m_file(fileInterface) {

} 

esp_err_t WavRecorder::record(i2s_chan_handle_t i2s_chan, uint16_t seconds) {

    esp_err_t ret = ESP_OK; 
    // 计算音频数据大小
    uint32_t byte_rate = EXAMPLE_I2S_SAMPLE_RATE * EXAMPLE_I2S_CHAN_NUM * EXAMPLE_I2S_SAMPLE_BITS / 8;
    uint32_t wav_size = seconds * byte_rate;
    
    // 创建初始WAV头部（数据大小暂时设为0）
    wav_header_t wav_header = WAV_HEADER_PCM_DEFAULT(wav_size, EXAMPLE_I2S_SAMPLE_BITS, 
                                                EXAMPLE_I2S_SAMPLE_RATE, 
                                                EXAMPLE_I2S_CHAN_NUM);
 
    // 1. 写入WAV头部
    if (m_file->write_file((char *)&wav_header,sizeof(wav_header))!=ESP_OK) {
        ESP_LOGE(TAG, "写入WAV头部失败");
        return ESP_FAIL;
    }
    
    // 2. 实时录音并分段写入数据
    /* Start recording */
    size_t wav_written = 0;
    static int16_t i2s_readraw_buff[8192];
     

    //使能通道，如果出错直接跳到err标签位置
    ESP_GOTO_ON_ERROR(i2s_channel_enable(i2s_chan), err, TAG, "error while starting i2s rx channel");

    while (wav_written < wav_size) {
        if(wav_written % byte_rate < sizeof(i2s_readraw_buff)) {
            ESP_LOGI(TAG, "Recording: %"PRIu32"/%ds", wav_written/byte_rate + 1, EXAMPLE_RECORD_TIME_SEC);
            printf(".");
        }
        size_t bytes_read = 0;
        /* Read RAW samples from ES7210 */
        ESP_GOTO_ON_ERROR(i2s_channel_read(i2s_chan, i2s_readraw_buff, sizeof(i2s_readraw_buff), &bytes_read,
                          pdMS_TO_TICKS(1000)), err, TAG, "error while reading samples from i2s");
        /* Write the samples to the WAV file */
        // ESP_GOTO_ON_FALSE(m_file->write_file(filename,(char *)i2s_readraw_buff,true), ESP_FAIL, err,
        // TAG, "error while writing samples to wav file");


        if (m_file->write_file( (char *)i2s_readraw_buff,bytes_read)!=ESP_OK) {
            ESP_LOGE(TAG, "写入音频数据失败");
            goto err;
        }

        wav_written += bytes_read;
    } 

    printf("录制完成\n");
err:
    i2s_channel_disable(i2s_chan);
    return ret;

}
 