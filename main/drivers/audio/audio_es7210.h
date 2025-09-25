#pragma once
#include <driver/i2s_types.h>
#include <driver/i2s_common.h>
#include <driver/i2s.h>
#include <driver/i2s_tdm.h>
#include "audio_input_interface.h"



/* I2C port and GPIOs */
#define EXAMPLE_I2C_NUM            (I2C_NUM_0)
#define EXAMPLE_I2C_SDA_IO         GPIO_NUM_2
#define EXAMPLE_I2C_SCL_IO         GPIO_NUM_1

/* I2S port and GPIOs */
#define EXAMPLE_I2S_NUM            (I2S_NUM_0)
#define EXAMPLE_I2S_MCK_IO         GPIO_NUM_38
#define EXAMPLE_I2S_BCK_IO         GPIO_NUM_14
#define EXAMPLE_I2S_WS_IO          GPIO_NUM_13
#define EXAMPLE_I2S_DI_IO          GPIO_NUM_12
 
/* I2S configurations */
#define EXAMPLE_I2S_TDM_FORMAT     (ES7210_I2S_FMT_I2S)
#define EXAMPLE_I2S_CHAN_NUM       (2)
#define EXAMPLE_I2S_SAMPLE_RATE    (16000)
#define EXAMPLE_I2S_MCLK_MULTIPLE  (I2S_MCLK_MULTIPLE_256)
#define EXAMPLE_I2S_SAMPLE_BITS    (I2S_DATA_BIT_WIDTH_16BIT)
#define EXAMPLE_I2S_TDM_SLOT_MASK ((i2s_tdm_slot_mask_t)(I2S_TDM_SLOT0 | I2S_TDM_SLOT1))
 
#define EXAMPLE_ES7210_I2C_ADDR    (0x41)
#define EXAMPLE_ES7210_I2C_CLK      (100000)
#define EXAMPLE_ES7210_MIC_BIAS     (ES7210_MIC_BIAS_2V87)
#define EXAMPLE_ES7210_MIC_GAIN     (ES7210_MIC_GAIN_30DB)   
#define EXAMPLE_ES7210_ADC_VOLUME     (0)


#define EXAMPLE_RECORD_TIME_SEC (10)

class AudioEs7210: public AudioInputInterface
{
private:
    i2s_chan_handle_t es7210_i2s_init(void);
    void es7210_code_init(void);
    /* data */
    i2s_chan_handle_t i2s_chan;
    uint32_t sample_rate;
    uint16_t channel_num;
public: 
    esp_err_t disable() override;
    esp_err_t enable() override;
    esp_err_t read(void *dest, size_t size, size_t *bytes_read) override;
    AudioEs7210(uint32_t sample_rate = 16000, uint8_t channel_num = 2);
}; 
