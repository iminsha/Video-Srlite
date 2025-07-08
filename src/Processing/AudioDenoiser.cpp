#include "AudioDenoiser.h"
#include <iostream>

AudioDenoiser::AudioDenoiser() {
    std::cout << "AudioDenoiser initialized (stub implementation)" << std::endl;
}

AudioDenoiser::~AudioDenoiser() = default;

bool AudioDenoiser::initialize() {
    std::cout << "AudioDenoiser::initialize called" << std::endl;
    return true;  // 简化实现，总是返回成功
}

void AudioDenoiser::processFrame(float* audioData, int frameSize) {
    std::cout << "AudioDenoiser::processFrame called with frame size: " << frameSize << std::endl;
    // 简化实现：不做任何处理，直接返回原始音频数据
} 