#pragma once
#include <vector>
#include <cstdint>
#include <string>

struct AudioFrameData {
    std::vector<uint8_t> data;

    int sampleRate = 0; // 采样率（Hz）
    int channels = 0; // 声道数
    int nbSamples = 0; // 解码后的样本数
    double duration = 0.0; // 该帧持续时间（秒）

    int64_t pts = 0; // 时间戳（FFmpeg）
    double timestamp = 0.0;

    std::string format;       // s16le / fltp / aac
    bool encoded = true;
};
