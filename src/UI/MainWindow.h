#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>
#include <QStandardPaths>
#include <QTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QThread>
#include <QCloseEvent>
#include <QMutex>
#include <QMap>
#include <QStringList>
#include <opencv2/opencv.hpp>
#include <memory>

// 引入模块化组件
#include "ImagePreviewWidget.h"
#include "FileListWidget.h"
#include "ImageProcessor.h"
#include "SettingsPanel.h"

// 引入工作池
#include "../WorkerPool/WorkerPool.h"
#include "../SuperEigen/include/SuperResEngine.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QStatusBar;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

// 前向声明
namespace SuperEigen {
class SuperResEngine;
}

// 文件类型枚举
enum class FileType {
    Unknown,
    Image,
    Video
};

// CompareMode枚举在ImagePreviewWidget.h中定义

// 处理参数结构体
struct ProcessingParams {
    int scaleFactor = 2;           // 超分辨率倍数 (2 或 4)
    bool enableAudioDenoise = true; // 是否启用音频降噪
    float denoiseStrength = 0.2f;   // 降噪强度 (0.0 - 1.0)
    float sharpenStrength = 0.5f;   // 锐化强度 (0.0 - 1.0)
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 拖放事件处理
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 菜单操作
    void openFile();
    void openFileList();
    void about();
    void showHelp();
    void exit();
    
    // 文件操作
    void onFileSelected(const QString& filePath);
    void onFileCountChanged(int count);
    
    // 图像处理
    void processCurrentImage();
    void processAllImages();
    void pauseProcessing();
    void resumeProcessing();
    void stopProcessing();
    
    // 图像处理结果处理
    void onImageProcessed(const QString& filePath, const cv::Mat& originalImage,
                         const cv::Mat& enhancedImage, qint64 processingTime);
    void onImageProcessError(const QString& filePath, const QString& error);
    void onProcessingProgress(int completed, int total);

    // 预览操作
    void onPreviewSaved(const QString& path);
    void onImageInfoUpdated(const QString& originalInfo, const QString& enhancedInfo);

    // 设置操作
    void onSettingsChanged(const ProcessingSettings& settings);

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void setupUI();
    void createModularComponents();
    void createControlPanel();
    void setupStyleSheet();
    void connectSignals();

    // UI辅助方法
    void showStatusMessage(const QString& message, int timeout = 3000);
    void updateWindowTitle();
    void updateProcessingStatus();

    // 文件验证
    bool isValidImageFile(const QString& filePath);
    QStringList extractImageFiles(const QMimeData* mimeData);

    // 超分引擎管理
    void initializeSuperResEngine();
    void loadImageForPreview(const QString& filePath);
    void processImageAsync(const QString& filePath);

private:
    // 菜单和动作
    QMenuBar *m_menuBar;
    QMenu *m_fileMenu;
    QMenu *m_viewMenu;
    QMenu *m_processMenu;
    QMenu *m_helpMenu;
    
    QAction *m_openAction;
    QAction *m_openListAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QAction *m_helpAction;
    QAction *m_processCurrentAction;
    QAction *m_processAllAction;
    QAction *m_pauseAction;
    QAction *m_stopAction;
    
    QStatusBar *m_statusBar;
    QProgressBar *m_statusProgressBar;
    QLabel *m_statusLabel;
    
    // 主界面组件
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QSplitter *m_leftSplitter;
    
    // 模块化组件
    FileListWidget *m_fileListWidget;
    ImagePreviewWidget *m_imagePreviewWidget;
    SettingsPanel *m_settingsPanel;

    // 工作池和超分引擎
    WorkerPool *m_workerPool;
    std::unique_ptr<SuperEigen::SuperResEngine> m_srEngine;
    bool m_engineReady;
    
    // 控制面板
    QWidget *m_controlPanel;
    QGroupBox *m_processingGroupBox;
    QPushButton *m_processCurrentBtn;
    QPushButton *m_processAllBtn;
    QPushButton *m_pauseBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_clearBtn;
    QProgressBar *m_overallProgressBar;
    QLabel *m_processingStatusLabel;
    
    // 数据
    QString m_currentFilePath;
    ProcessingSettings m_currentSettings;

    // 图像缓存和处理队列
    QMap<QString, cv::Mat> m_imageCache;  // 图像缓存，用于立即预览
    QStringList m_processingQueue;        // 待处理队列
    QMutex m_cacheMutex;                  // 缓存访问锁

    // 处理状态
    bool m_isProcessing;
    bool m_isPaused;
    int m_totalTasks;
    int m_completedTasks;
    QTimer *m_statusTimer;
};

#endif // MAINWINDOW_H
