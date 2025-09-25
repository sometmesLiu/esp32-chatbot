#pragma once
#include "file_interface.h"
#include "driver/i2s_std.h"
#include <memory>  
#include "audio_input_interface.h"

class WavRecorder {
public:
    explicit WavRecorder(std::shared_ptr<FileInterface> fileInterface, std::shared_ptr<AudioInputInterface> audioInput);

    esp_err_t record(uint16_t seconds);
    
private:
    std::shared_ptr<FileInterface> m_file; // 用智能指针持有对象
    std::shared_ptr<AudioInputInterface> m_audio_input;
};
