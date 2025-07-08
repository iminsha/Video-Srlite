# ---------------- Core library ----------------
add_library(VideoSRLiteCore
    # Decoder
    Decoder/src/Decoder.cpp
    Decoder/src/VideoDecoder.cpp
    Decoder/src/AudioDecoder.cpp
    
    # SuperEigen (SuperResolution)
    SuperEigen/src/SuperResEngine.cpp
    SuperEigen/src/ModelSession.cpp
    SuperEigen/src/PrePostProcessor.cpp
    SuperEigen/src/SuperResConfig.cpp
    
    # Processing
    Processing/SuperResolution.cpp
    Processing/PostProcessor.cpp
    Processing/AudioDenoiser.cpp
    
    # AppController
    AppController/AppController.cpp
    AppController/WorkerPool.cpp
    
    # PostFilter
    PostFilter/PostFilterProcessor.cpp
    PostFilter/PostFilter.cpp
    
    # AudioProcessor
    AudioProcessor/AudioProcessor.cpp
    
    # AudioProc
    AudioProc/AudioProc.cpp
    
    # Encoder
    Encoder/VideoEncoder.cpp
    Encoder/AudioEncoder.cpp
    Encoder/Encoder.cpp
    Encoder/Muxer.cpp
    
    # SyncVA
    SyncVA/AVSyncManager.cpp
    
    # Utils
    Utils/Logger.cpp
    Utils/LogUtils.cpp
    Utils/FileUtils.cpp
)

target_include_directories(VideoSRLiteCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SuperEigen/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Decoder/include
    ${CMAKE_CURRENT_SOURCE_DIR}/DataStruct
    ${OpenCV_INCLUDE_DIRS}
)

# Append ONNXRuntime include dir if found
if(ONNXRUNTIME_INCLUDE_DIR AND NOT ONNXRUNTIME_INCLUDE_DIR STREQUAL "ONNXRUNTIME_INCLUDE_DIR-NOTFOUND")
    target_include_directories(VideoSRLiteCore PUBLIC ${ONNXRUNTIME_INCLUDE_DIR})
endif()

# Append OpenGL & extra libs
find_package(OpenGL REQUIRED)

if(RNNOISE_FOUND)
    set(EXTRA_AUDIO_LIB RNNOISE_LIBRARIES)
endif()

target_link_libraries(VideoSRLiteCore PUBLIC
    PkgConfig::FFMPEG
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENGL_LIBRARIES}
    ${EXTRA_AUDIO_LIB}
)

# Update link libraries section to conditionally link ONNXRuntime
if(ONNXRUNTIME_LIB AND NOT ONNXRUNTIME_LIB STREQUAL "ONNXRUNTIME_LIB-NOTFOUND")
    target_link_libraries(VideoSRLiteCore PUBLIC ${ONNXRUNTIME_LIB})
endif()

# ---------------- Tools ----------------
# Build standalone super-resolution tool if ONNX Runtime is available
if(ONNXRUNTIME_LIBRARIES OR ONNXRUNTIME_LIB)
    add_executable(run_sr_image tools/run_sr_image.cpp)
    target_link_libraries(run_sr_image PRIVATE VideoSRLiteCore)
    set_target_properties(run_sr_image PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif() 