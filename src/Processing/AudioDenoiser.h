#ifndef AUDIODENOISER_H
#define AUDIODENOISER_H

class AudioDenoiser {
public:
    AudioDenoiser();
    ~AudioDenoiser();
    
    bool initialize();
    void processFrame(float* audioData, int frameSize);

private:
    // 简化实现，不需要成员变量
};

#endif // AUDIODENOISER_H 