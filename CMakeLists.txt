cmake_minimum_required(VERSION 3.16)
project(VideoSR-Lite VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)
set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui)
find_package(PkgConfig REQUIRED)

# Find FFmpeg
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavcodec
    libavformat
    libavutil
    libswscale
    libswresample
)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Find Threads
find_package(Threads REQUIRED)

# Find ONNX Runtime (optional)
find_path(ONNXRUNTIME_ROOT_PATH
    NAMES include/onnxruntime_cxx_api.h
    PATHS /usr/local /usr /opt/onnxruntime ${CMAKE_CURRENT_SOURCE_DIR}/onnxruntime ${CMAKE_CURRENT_SOURCE_DIR}/third_party/onnxruntime
    DOC "ONNX Runtime root directory"
)

# After initial find_path failure, perform a manual search inside project/onnx/*
if(NOT ONNXRUNTIME_ROOT_PATH)
    file(GLOB ORT_DIR_CANDIDATES
        LIST_DIRECTORIES true
        "${CMAKE_CURRENT_SOURCE_DIR}/onnx/onnxruntime*"
    )
    foreach(dir ${ORT_DIR_CANDIDATES})
        if(EXISTS "${dir}/include/onnxruntime_cxx_api.h")
            set(ONNXRUNTIME_ROOT_PATH ${dir})
            set(ONNXRUNTIME_INCLUDE_DIRS ${dir}/include)
            set(ONNXRUNTIME_LIBRARIES ${dir}/lib/libonnxruntime.so)
            add_definitions(-DHAVE_ONNXRUNTIME)
            message(STATUS "ONNX Runtime found in local onnx directory: ${dir}")
            break()
        endif()
    endforeach()
endif()

if(ONNXRUNTIME_ROOT_PATH)
    # If include/lib not yet set (find_path branch), assign them now
    if(NOT ONNXRUNTIME_INCLUDE_DIRS)
        set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_ROOT_PATH}/include)
    endif()
    if(NOT ONNXRUNTIME_LIBRARIES)
        set(ONNXRUNTIME_LIBRARIES ${ONNXRUNTIME_ROOT_PATH}/lib/libonnxruntime.so)
    endif()
    message(STATUS "ONNX Runtime found: ${ONNXRUNTIME_ROOT_PATH}")
else()
    message(WARNING "ONNX Runtime not found. Building without ONNX Runtime support.")
    set(ONNXRUNTIME_INCLUDE_DIRS "")
    set(ONNXRUNTIME_LIBRARIES "")
endif()

# Find RNNoise (optional)
find_path(RNNOISE_INCLUDE_DIR rnnoise.h
    PATHS /usr/include /usr/local/include
)
find_library(RNNOISE_LIBRARY rnnoise
    PATHS /usr/lib /usr/local/lib
)

if(RNNOISE_INCLUDE_DIR AND RNNOISE_LIBRARY)
    add_definitions(-DHAVE_RNNOISE)
    message(STATUS "RNNoise found: ${RNNOISE_LIBRARY}")
else()
    message(WARNING "RNNoise not found. Building without RNNoise support.")
    set(RNNOISE_LIBRARY "")
endif()

# Enable Qt's MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set UI files directory
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/UI)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Processing
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataStruct
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Decoder/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SuperEigen/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AppController
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioProcessor
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioProc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Encoder
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SyncVA
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PostFilter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/WorkerPool
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
)

if(ONNXRUNTIME_INCLUDE_DIRS)
    include_directories(${ONNXRUNTIME_INCLUDE_DIRS})
endif()

if(RNNOISE_INCLUDE_DIR)
    include_directories(${RNNOISE_INCLUDE_DIR})
endif()

# Build option for GUI
option(BUILD_GUI "Build GUI application" ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Utils/FileUtils.cpp
    src/Utils/LogUtils.cpp
    src/Utils/Logger.cpp
    src/Decoder/src/Decoder.cpp
    src/Decoder/src/VideoDecoder.cpp
    src/Decoder/src/AudioDecoder.cpp
    src/Processing/AudioDenoiser.cpp
    src/Processing/PostProcessor.cpp
    src/Processing/SuperResolution.cpp
    src/AppController/AppController.cpp
    src/AppController/WorkerPool.cpp
    src/AudioProcessor/AudioProcessor.cpp
    src/AudioProc/AudioProc.cpp
    src/Encoder/VideoEncoder.cpp
    src/Encoder/AudioEncoder.cpp
    src/Encoder/Encoder.cpp
    src/Encoder/Muxer.cpp
    src/SyncVA/AVSyncManager.cpp
    src/PostFilter/PostFilterProcessor.cpp
    src/PostFilter/PostFilter.cpp
)

# Add SuperEigen sources if ONNX Runtime is available
if(ONNXRUNTIME_LIBRARIES)
    list(APPEND SOURCES 
        src/SuperEigen/src/SuperResEngine.cpp
        src/SuperEigen/src/ModelSession.cpp
        src/SuperEigen/src/PrePostProcessor.cpp
        src/SuperEigen/src/SuperResConfig.cpp
    )
endif()

# Add GUI sources if enabled
if(BUILD_GUI)
    list(APPEND SOURCES
        src/UI/MainWindow.cpp
        src/UI/FileListWidget.cpp
        src/UI/ImagePreviewWidget.cpp
        src/UI/SettingsPanel.cpp
        src/UI/ImageProcessor.cpp
        src/WorkerPool/WorkerPool.cpp
    )
endif()

# Header files
set(HEADERS
    src/Processing/SuperResolution.h
    src/Processing/AudioDenoiser.h
    src/Processing/PostProcessor.h
    src/Utils/FileUtils.h
    src/Utils/LogUtils.h
    src/Utils/Logger.h
    src/Decoder/include/Decoder.h
    src/Decoder/include/VideoDecoder.h
    src/Decoder/include/AudioDecoder.h
    src/AppController/AppController.h
    src/AppController/WorkerPool.h
    src/AudioProcessor/AudioProcessor.h
    src/AudioProc/AudioProc.h
    src/Encoder/VideoEncoder.h
    src/Encoder/AudioEncoder.h
    src/Encoder/Encoder.h
    src/Encoder/Muxer.h
    src/SyncVA/AVSyncManager.h
    src/PostFilter/PostFilterProcessor.h
    src/PostFilter/PostFilter.h
    src/DataStruct/FrameData.h
    src/DataStruct/AudioFrameData.h
)

# Add SuperEigen headers if ONNX Runtime is available
if(ONNXRUNTIME_LIBRARIES)
    list(APPEND HEADERS
        src/SuperEigen/include/SuperResEngine.h
        src/SuperEigen/include/ModelSession.h
        src/SuperEigen/include/PrePostProcessor.h
        src/SuperEigen/include/SuperResConfig.h
    )
endif()

# Add GUI headers if enabled
if(BUILD_GUI)
    list(APPEND HEADERS
        src/UI/MainWindow.h
        src/UI/FileListWidget.h
        src/UI/ImagePreviewWidget.h
        src/UI/SettingsPanel.h
        src/UI/ImageProcessor.h
        src/WorkerPool/WorkerPool.h
    )
endif()

# UI files
set(UI_FILES
    src/UI/MainWindow.ui
)

# Create executable
if(BUILD_GUI)
    add_executable(VideoSRLiteGUI ${SOURCES} ${HEADERS} ${UI_FILES})
    set(TARGET_NAME VideoSRLiteGUI)
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    set(TARGET_NAME ${PROJECT_NAME})
endif()

# Link libraries
set(COMMON_LIBS
    PkgConfig::FFMPEG
    ${OpenCV_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(BUILD_GUI)
    list(APPEND COMMON_LIBS
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
    )
endif()

target_link_libraries(${TARGET_NAME} ${COMMON_LIBS})

# Link optional libraries
if(ONNXRUNTIME_LIBRARIES)
    target_link_libraries(${TARGET_NAME} ${ONNXRUNTIME_LIBRARIES})
endif()

if(RNNOISE_LIBRARY)
    target_link_libraries(${TARGET_NAME} ${RNNOISE_LIBRARY})
endif()

# Set output directory
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION bin
)

# Print configuration summary
message(STATUS "")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt5 Version: ${Qt5_VERSION}")
message(STATUS "  OpenCV Version: ${OpenCV_VERSION}")
if(ONNXRUNTIME_ROOT_PATH)
    message(STATUS "  ONNX Runtime: ${ONNXRUNTIME_ROOT_PATH}")
else()
    message(STATUS "  ONNX Runtime: Not found (optional)")
endif()
if(RNNOISE_LIBRARY)
    message(STATUS "  RNNoise: ${RNNOISE_LIBRARY}")
else()
    message(STATUS "  RNNoise: Not found (optional)")
endif()
message(STATUS "")

# Test executables are now handled within individual modules

# Build standalone super-resolution tool
if(ONNXRUNTIME_LIBRARIES)
    add_executable(run_sr_image 
        src/tools/run_sr_image.cpp 
        src/Processing/SuperResolution.cpp
        src/SuperEigen/src/SuperResEngine.cpp
        src/SuperEigen/src/ModelSession.cpp
        src/SuperEigen/src/PrePostProcessor.cpp
        src/SuperEigen/src/SuperResConfig.cpp
        src/Utils/Logger.cpp
        src/Utils/LogUtils.cpp
    )
    target_include_directories(run_sr_image PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SuperEigen/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
    )
    target_link_libraries(run_sr_image
        ${OpenCV_LIBS}
        ${ONNXRUNTIME_LIBRARIES}
    )
    set_target_properties(run_sr_image PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()

if(NOT SOURCES_ADDED_DECODER)
    list(APPEND SOURCES src/Decoder/src/Decoder.cpp)
    set(SOURCES_ADDED_DECODER ON)
endif()

# All sources handled in main CMakeLists.txt

