set(UI_SOURCES
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    ImagePreviewWidget.cpp
    ImagePreviewWidget.h
    FileListWidget.cpp
    FileListWidget.h
    ImageProcessor.cpp
    ImageProcessor.h
    SettingsPanel.cpp
    SettingsPanel.h
    VideoPreviewWidget.cpp
    VideoPreviewWidget.h
    SettingsPanel.cpp
    SettingsPanel.h
)

# Qt MOC for headers with Q_OBJECT
qt5_wrap_cpp(UI_MOC_SOURCES
    MainWindow.h
    ImagePreviewWidget.h
    FileListWidget.h
    ImageProcessor.h
    SettingsPanel.h
    VideoPreviewWidget.h
)

# Qt UIC for .ui files
qt5_wrap_ui(UI_UIC_SOURCES
    MainWindow.ui
)

# Create UI library
add_library(UI ${UI_SOURCES} ${UI_MOC_SOURCES} ${UI_UIC_SOURCES})

# Link dependencies
target_link_libraries(UI
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(UI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OpenCV_INCLUDE_DIRS}
) 