#include "../include/SuperResConfig.h"
#include <onnxruntime_cxx_api.h>

namespace SuperEigen {

bool SuperResConfig::checkGPUAvailable() {
    // 检查CUDA是否可用
    try {
        Ort::SessionOptions session_options;
        std::vector<std::string> providers = Ort::GetAvailableProviders();
        
        for (const auto& provider : providers) {
            if (provider == "CUDAExecutionProvider") {
                return true;
            }
        }
    } catch (...) {
        // 忽略异常
    }
    
    return false;
}

} // namespace SuperEigen 