#include "AVSyncManager.h"
#include "../Utils/Logger.h"
#include <limits>
#include <stdexcept>

AVSyncManager::AVSyncManager() {
    LOG_DEBUG("AVSyncManager initialized");
}

AVSyncManager::~AVSyncManager() {
    clear();
    LOG_DEBUG("AVSyncManager destroyed");
}

void AVSyncManager::pushVideo(const FrameData& frame) {
    std::lock_guard<std::mutex> lock(mutex_);
    videoQueue_.push_back(frame);
    
    LOG_DEBUG("Video frame pushed, timestamp: " + std::to_string(frame.timestamp) + 
              "s, queue size: " + std::to_string(videoQueue_.size()));
}

void AVSyncManager::pushAudio(const AudioFrameData& frame) {
    std::lock_guard<std::mutex> lock(mutex_);
    audioQueue_.push_back(frame);
    
    LOG_DEBUG("Audio frame pushed, timestamp: " + std::to_string(frame.timestamp) + 
              "s, queue size: " + std::to_string(audioQueue_.size()));
}

bool AVSyncManager::hasNext() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !videoQueue_.empty() || !audioQueue_.empty();
}

AVSyncManager::FrameVariant AVSyncManager::popNext() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (videoQueue_.empty() && audioQueue_.empty()) {
        throw std::runtime_error("No frames available in sync manager");
    }
    
    double videoTimestamp = getVideoFrontTimestamp();
    double audioTimestamp = getAudioFrontTimestamp();
    
    // 比较时间戳，选择更早的帧
    if (videoTimestamp <= audioTimestamp) {
        // 视频帧更早（或音频队列为空）
        FrameData frame = videoQueue_.front();
        videoQueue_.pop_front();
        
        LOG_DEBUG("Popped video frame, timestamp: " + std::to_string(frame.timestamp) + 
                  "s, remaining video frames: " + std::to_string(videoQueue_.size()));
        
        return frame;
    } else {
        // 音频帧更早（或视频队列为空）
        AudioFrameData frame = audioQueue_.front();
        audioQueue_.pop_front();
        
        LOG_DEBUG("Popped audio frame, timestamp: " + std::to_string(frame.timestamp) + 
                  "s, remaining audio frames: " + std::to_string(audioQueue_.size()));
        
        return frame;
    }
}

size_t AVSyncManager::getVideoQueueSize() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return videoQueue_.size();
}

size_t AVSyncManager::getAudioQueueSize() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return audioQueue_.size();
}

void AVSyncManager::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t videoCount = videoQueue_.size();
    size_t audioCount = audioQueue_.size();
    
    videoQueue_.clear();
    audioQueue_.clear();
    
    LOG_INFO("Cleared sync manager queues - Video: " + std::to_string(videoCount) + 
             " frames, Audio: " + std::to_string(audioCount) + " frames");
}

double AVSyncManager::getNextTimestamp() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (videoQueue_.empty() && audioQueue_.empty()) {
        return -1.0;
    }
    
    double videoTimestamp = getVideoFrontTimestamp();
    double audioTimestamp = getAudioFrontTimestamp();
    
    return std::min(videoTimestamp, audioTimestamp);
}

bool AVSyncManager::isVideoFrame(const FrameVariant& frame) {
    return std::holds_alternative<FrameData>(frame);
}

bool AVSyncManager::isAudioFrame(const FrameVariant& frame) {
    return std::holds_alternative<AudioFrameData>(frame);
}

const FrameData& AVSyncManager::getVideoFrame(const FrameVariant& frame) {
    return std::get<FrameData>(frame);
}

const AudioFrameData& AVSyncManager::getAudioFrame(const FrameVariant& frame) {
    return std::get<AudioFrameData>(frame);
}

double AVSyncManager::getVideoFrontTimestamp() const {
    if (videoQueue_.empty()) {
        return std::numeric_limits<double>::infinity();
    }
    return videoQueue_.front().timestamp;
}

double AVSyncManager::getAudioFrontTimestamp() const {
    if (audioQueue_.empty()) {
        return std::numeric_limits<double>::infinity();
    }
    return audioQueue_.front().timestamp;
} 