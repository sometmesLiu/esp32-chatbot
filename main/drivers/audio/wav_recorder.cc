#include "wav_recorder.h"
#include "format_wav.h"
#include "audio_es7210.h"
#include <esp_timer.h>
#include <esp_check.h>

#define TAG "WavRecorder"

WavRecorder::WavRecorder(std::shared_ptr<FileInterface> fileInterface, std::shared_ptr<AudioInputInterface> audioInput) 
: m_file(fileInterface), m_audio_input(audioInput) {

} 

esp_err_t WavRecorder::record(uint16_t seconds) {

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
    static int16_t i2s_readraw_buff[8192];  // 16384字节
    // 每次读取约 16384 ÷ 64000 ≈ 0.256秒 的音频数据
     

    //使能通道，如果出错直接跳到err标签位置
    // 开启了I2S接收通道，麦克风开始工作
    ESP_GOTO_ON_ERROR(m_audio_input->enable(), err, TAG, "error while starting i2s rx channel");

    while (wav_written < wav_size) {
        // 进度显示控制
        if(wav_written % byte_rate < sizeof(i2s_readraw_buff)) {
            //  每录制约1秒就显示一次进度
            ESP_LOGI(TAG, "Recording: %"PRIu32"/%ds", wav_written/byte_rate + 1, seconds);
            printf(".");
        }
        size_t bytes_read = 0;
        /* Read RAW samples from ES7210 */
        ESP_GOTO_ON_ERROR(
            m_audio_input->read(i2s_readraw_buff, sizeof(i2s_readraw_buff), &bytes_read),
             err, 
             TAG, 
             "error while reading samples from i2s");
        /* Write the samples to the WAV file */
        // ESP_GOTO_ON_FALSE(m_file->write_file(filename,(char *)i2s_readraw_buff,true), ESP_FAIL, err,
        // TAG, "error while writing samples to wav file");

        // 在读取数据后添加调试输出
        int16_t max_value = 0;
        for(int i = 0; i < bytes_read/2; i++) {
            if(abs(i2s_readraw_buff[i]) > max_value) {
                max_value = abs(i2s_readraw_buff[i]);
            }
        }
        ESP_LOGD(TAG, "Max sample value: %d", max_value);

        // 添加平均值计算，帮助判断是否有有效信号
        int32_t sum = 0;
        for(int i = 0; i < bytes_read/2; i++) {
            sum += abs(i2s_readraw_buff[i]);
        }
        int16_t avg_value = sum / (bytes_read/2);
        ESP_LOGD(TAG, "Max: %d, Avg: %d", max_value, avg_value);
        if (m_file->write_file( (char *)i2s_readraw_buff,bytes_read)!=ESP_OK) {
            ESP_LOGE(TAG, "写入音频数据失败");
            goto err;
        }

        wav_written += bytes_read;
    } 

    printf("录制完成\n");
err:
    m_audio_input->disable();
    return ret;

}
 