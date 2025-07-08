#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../SuperEigen/include/SuperResEngine.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <QTimer>
#include <QElapsedTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QPainter>
#include <QScrollBar>
#include <QDateTime>
#include <QApplication>
#include <QKeyEvent>
#include <QSplitter>
#include <QMimeDatabase>
#include <QUuid>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_isProcessing(false)
    , m_isPaused(false)
    , m_totalTasks(0)
    , m_completedTasks(0)
    , m_workerPool(nullptr)
    , m_srEngine(nullptr)
    , m_engineReady(false)
{
    setWindowTitle("VideoSR-Lite - 智能视频超分辨率工具");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    setMinimumSize(1600, 1000);
    resize(1800, 1200);

    // 创建状态计时器
    m_statusTimer = new QTimer(this);
    m_statusTimer->setSingleShot(true);

    // 初始化工作池（单线程用于图像处理）
    m_workerPool = new WorkerPool(1, this);

    // 异步初始化超分引擎
    initializeSuperResEngine();

    setupUI();
    createActions();
    createMenus();
    createStatusBar();
    connectSignals();
    setupStyleSheet();

    showStatusMessage("✨ VideoSR-Lite 已启动，正在初始化超分辨率引擎...", 5000);

    qDebug() << "MainWindow: 初始化完成";
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow: 开始析构";

    // 停止工作池
    if (m_workerPool) {
        m_workerPool->stop();
    }

    qDebug() << "MainWindow: 析构完成";
}

void MainWindow::setupUI()
{
    // 创建中央部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // 创建主分割器（水平）
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_centralWidget);
    
    QHBoxLayout* centralLayout = new QHBoxLayout(m_centralWidget);
    centralLayout->setContentsMargins(8, 8, 8, 8);
    centralLayout->addWidget(m_mainSplitter);
    
    // 创建左侧分割器（垂直）
    m_leftSplitter = new QSplitter(Qt::Vertical, this);
    m_mainSplitter->addWidget(m_leftSplitter);
    
    // 创建模块化组件
    createModularComponents();
    
    // 创建控制面板
    createControlPanel();
    
    // 设置分割器比例和最小尺寸
    m_mainSplitter->setSizes({500, 1000});  // 左侧500px，右侧1000px
    m_mainSplitter->setStretchFactor(0, 0);  // 左侧固定
    m_mainSplitter->setStretchFactor(1, 1);  // 右侧可拉伸
    
    m_leftSplitter->setSizes({300, 120, 100});  // 文件列表300px，设置120px，控制100px
    m_leftSplitter->setStretchFactor(0, 1);  // 文件列表可拉伸
    m_leftSplitter->setStretchFactor(1, 0);  // 设置面板固定
    m_leftSplitter->setStretchFactor(2, 0);  // 控制面板固定
    
    // 设置最小尺寸
    m_fileListWidget->setMinimumHeight(200);
    m_settingsPanel->setMinimumHeight(100);
    m_controlPanel->setMinimumHeight(80);
}

void MainWindow::createModularComponents()
{
    // 创建文件列表组件
    m_fileListWidget = new FileListWidget(this);
    m_leftSplitter->addWidget(m_fileListWidget);

    // 创建设置面板
    m_settingsPanel = new SettingsPanel(this);
    m_leftSplitter->addWidget(m_settingsPanel);

    // 创建图像预览组件
    m_imagePreviewWidget = new ImagePreviewWidget(this);
    m_mainSplitter->addWidget(m_imagePreviewWidget);
}

void MainWindow::createControlPanel()
{
    m_controlPanel = new QWidget(this);
    m_leftSplitter->addWidget(m_controlPanel);
    
    QVBoxLayout* controlLayout = new QVBoxLayout(m_controlPanel);
    controlLayout->setSpacing(12);
    
    // 处理控制组
    m_processingGroupBox = new QGroupBox("🎮 处理控制", m_controlPanel);
    controlLayout->addWidget(m_processingGroupBox);
    
    QVBoxLayout* processingLayout = new QVBoxLayout(m_processingGroupBox);
    processingLayout->setSpacing(8);
    
    // 第一行按钮
    QWidget* buttonRow1 = new QWidget(this);
    QHBoxLayout* buttonLayout1 = new QHBoxLayout(buttonRow1);
    buttonLayout1->setSpacing(8);
    
    m_processCurrentBtn = new QPushButton("🚀 处理当前", buttonRow1);
    m_processCurrentBtn->setToolTip("处理当前选中的图像文件");
    m_processCurrentBtn->setEnabled(false);
    buttonLayout1->addWidget(m_processCurrentBtn);
    
    m_processAllBtn = new QPushButton("⚡ 批量处理", buttonRow1);
    m_processAllBtn->setToolTip("处理列表中的所有图像文件");
    m_processAllBtn->setEnabled(false);
    buttonLayout1->addWidget(m_processAllBtn);
    
    processingLayout->addWidget(buttonRow1);
    
    // 第二行按钮
    QWidget* buttonRow2 = new QWidget(this);
    QHBoxLayout* buttonLayout2 = new QHBoxLayout(buttonRow2);
    buttonLayout2->setSpacing(8);
    
    m_pauseBtn = new QPushButton("⏸️ 暂停", buttonRow2);
    m_pauseBtn->setToolTip("暂停/恢复处理");
    m_pauseBtn->setEnabled(false);
    buttonLayout2->addWidget(m_pauseBtn);
    
    m_stopBtn = new QPushButton("⏹️ 停止", buttonRow2);
    m_stopBtn->setToolTip("停止所有处理任务");
    m_stopBtn->setEnabled(false);
    buttonLayout2->addWidget(m_stopBtn);
    
    processingLayout->addWidget(buttonRow2);
    
    m_clearBtn = new QPushButton("🧹 清空预览", this);
    m_clearBtn->setToolTip("清空当前预览");
    processingLayout->addWidget(m_clearBtn);
    
    // 进度显示
    m_overallProgressBar = new QProgressBar(this);
    m_overallProgressBar->setVisible(false);
    processingLayout->addWidget(m_overallProgressBar);
    
    m_processingStatusLabel = new QLabel("状态：待机", this);
    m_processingStatusLabel->setStyleSheet("color: #666666; font-size: 12px;");
    processingLayout->addWidget(m_processingStatusLabel);
    
    controlLayout->addStretch();
}

void MainWindow::createActions()
{
    // 文件菜单动作
    m_openAction = new QAction("📂 打开文件...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("打开图像文件");
    
    m_openListAction = new QAction("📄 打开文件列表...", this);
    m_openListAction->setShortcut(QKeySequence("Ctrl+L"));
    m_openListAction->setStatusTip("批量添加图像文件");
    
    m_exitAction = new QAction("🚪 退出", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("退出应用程序");
    
    // 处理菜单动作
    m_processCurrentAction = new QAction("🚀 处理当前图像", this);
    m_processCurrentAction->setShortcut(QKeySequence("Ctrl+R"));
    m_processCurrentAction->setStatusTip("处理当前选中的图像");
    m_processCurrentAction->setEnabled(false);
    
    m_processAllAction = new QAction("⚡ 批量处理", this);
    m_processAllAction->setShortcut(QKeySequence("Ctrl+Shift+R"));
    m_processAllAction->setStatusTip("处理所有图像文件");
    m_processAllAction->setEnabled(false);
    
    m_pauseAction = new QAction("⏸️ 暂停处理", this);
    m_pauseAction->setShortcut(QKeySequence("Ctrl+P"));
    m_pauseAction->setStatusTip("暂停或恢复处理");
    m_pauseAction->setEnabled(false);
    
    m_stopAction = new QAction("⏹️ 停止处理", this);
    m_stopAction->setShortcut(QKeySequence("Ctrl+S"));
    m_stopAction->setStatusTip("停止所有处理任务");
    m_stopAction->setEnabled(false);
    
    // 帮助菜单动作
    m_helpAction = new QAction("❓ 使用帮助", this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    m_helpAction->setStatusTip("查看使用帮助");
    
    m_aboutAction = new QAction("ℹ️ 关于", this);
    m_aboutAction->setStatusTip("关于 VideoSR-Lite");
}

void MainWindow::createMenus()
{
    m_menuBar = menuBar();
    
    // 文件菜单
    m_fileMenu = m_menuBar->addMenu("📁 文件(&F)");
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_openListAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // 视图菜单
    m_viewMenu = m_menuBar->addMenu("👁️ 视图(&V)");
    // TODO: 添加视图相关动作
    
    // 处理菜单
    m_processMenu = m_menuBar->addMenu("⚡ 处理(&P)");
    m_processMenu->addAction(m_processCurrentAction);
    m_processMenu->addAction(m_processAllAction);
    m_processMenu->addSeparator();
    m_processMenu->addAction(m_pauseAction);
    m_processMenu->addAction(m_stopAction);
    
    // 帮助菜单
    m_helpMenu = m_menuBar->addMenu("❓ 帮助(&H)");
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createStatusBar()
{
    m_statusBar = statusBar();
    
    m_statusLabel = new QLabel("就绪", this);
    m_statusBar->addWidget(m_statusLabel);
    
    m_statusProgressBar = new QProgressBar(this);
    m_statusProgressBar->setVisible(false);
    m_statusProgressBar->setMaximumWidth(200);
    m_statusBar->addPermanentWidget(m_statusProgressBar);
    
    // 连接状态计时器
    connect(m_statusTimer, &QTimer::timeout, [this]() {
        m_statusLabel->setText("就绪");
    });
}

void MainWindow::connectSignals()
{
    // 菜单信号
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_openListAction, &QAction::triggered, this, &MainWindow::openFileList);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::exit);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::showHelp);
    
    connect(m_processCurrentAction, &QAction::triggered, this, &MainWindow::processCurrentImage);
    connect(m_processAllAction, &QAction::triggered, this, &MainWindow::processAllImages);
    connect(m_pauseAction, &QAction::triggered, this, &MainWindow::pauseProcessing);
    connect(m_stopAction, &QAction::triggered, this, &MainWindow::stopProcessing);
    
    // 文件列表信号
    connect(m_fileListWidget, &FileListWidget::fileSelected, this, &MainWindow::onFileSelected);
    connect(m_fileListWidget, &FileListWidget::fileCountChanged, this, &MainWindow::onFileCountChanged);

    // 工作池信号（如果需要的话可以添加）
    // WorkerPool 使用回调函数，不需要信号连接
    
    // 预览组件信号
    connect(m_imagePreviewWidget, &ImagePreviewWidget::previewSaved, this, &MainWindow::onPreviewSaved);
    connect(m_imagePreviewWidget, &ImagePreviewWidget::imageInfoUpdated, this, &MainWindow::onImageInfoUpdated);
    
    // 设置面板信号
    connect(m_settingsPanel, &SettingsPanel::settingsChanged, this, &MainWindow::onSettingsChanged);
    
    // 控制按钮信号
    connect(m_processCurrentBtn, &QPushButton::clicked, this, &MainWindow::processCurrentImage);
    connect(m_processAllBtn, &QPushButton::clicked, this, &MainWindow::processAllImages);
    connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::pauseProcessing);
    connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::stopProcessing);
    connect(m_clearBtn, &QPushButton::clicked, [this]() {
        m_imagePreviewWidget->clearPreview();
        m_currentFilePath.clear();
        updateWindowTitle();
    });
}

void MainWindow::setupStyleSheet()
{
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 8px;
            margin-top: 8px;
            padding-top: 8px;
            background-color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 8px 0 8px;
            color: #1976d2;
        }
        
        QPushButton {
            background-color: #1976d2;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: bold;
            min-height: 20px;
        }
        
        QPushButton:hover {
            background-color: #1565c0;
        }
        
        QPushButton:pressed {
            background-color: #0d47a1;
        }
        
        QPushButton:disabled {
            background-color: #cccccc;
            color: #666666;
        }
        
        QProgressBar {
            border: 2px solid #cccccc;
            border-radius: 6px;
            text-align: center;
            background-color: #f0f0f0;
        }
        
        QProgressBar::chunk {
            background-color: #4caf50;
            border-radius: 4px;
        }
        
        QSplitter::handle {
            background-color: #e0e0e0;
        }
        
        QSplitter::handle:horizontal {
            width: 3px;
        }
        
        QSplitter::handle:vertical {
            height: 3px;
        }
    )";
    
    setStyleSheet(styleSheet);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QStringList imageFiles = extractImageFiles(event->mimeData());
        if (!imageFiles.isEmpty()) {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList imageFiles = extractImageFiles(event->mimeData());
    if (!imageFiles.isEmpty()) {
        m_fileListWidget->addFiles(imageFiles);
        showStatusMessage(QString("成功添加 %1 个文件").arg(imageFiles.size()));
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        if (m_fileListWidget->getFileCount() > 0) {
            m_fileListWidget->removeCurrentFile();
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_isProcessing) {
        int ret = QMessageBox::question(this, "确认退出", 
                                       "处理任务正在进行中，确定要退出吗？\n退出将停止所有处理任务。",
                                       QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
        
        // 停止处理
        stopProcessing();
    }
    
    event->accept();
}

void MainWindow::openFile()
{
    QString lastDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString filter = "图像文件 (*.jpg *.jpeg *.png *.bmp *.tiff *.tif *.webp);;所有文件 (*.*)";
    
    QString filePath = QFileDialog::getOpenFileName(this, "选择图像文件", lastDir, filter);
    
    if (!filePath.isEmpty()) {
        m_fileListWidget->addFile(filePath);
        showStatusMessage("文件已添加到列表");
    }
}

void MainWindow::openFileList()
{
    QString lastDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString filter = "图像文件 (*.jpg *.jpeg *.png *.bmp *.tiff *.tif *.webp);;所有文件 (*.*)";
    
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "选择图像文件", lastDir, filter);
    
    if (!filePaths.isEmpty()) {
        m_fileListWidget->addFiles(filePaths);
        showStatusMessage(QString("成功添加 %1 个文件").arg(filePaths.size()));
    }
}

void MainWindow::about()
{
    QString aboutText = R"(
<h2>🎯 VideoSR-Lite</h2>
<p><b>智能视频超分辨率处理工具</b></p>
<p>版本: 2.0.0</p>
<p>构建日期: %1</p>

<h3>✨ 主要特性:</h3>
<ul>
<li>🚀 高性能超分辨率处理</li>
<li>🎨 现代化用户界面</li>
<li>⚡ 多线程异步处理</li>
<li>🔍 实时预览对比</li>
<li>📁 批量文件处理</li>
<li>🎛️ 丰富的参数设置</li>
</ul>

<h3>🛠️ 技术栈:</h3>
<p>Qt 5.15+ • OpenCV 4.x • ONNX Runtime • Modern C++</p>

<p>© 2024 VideoSR-Lite Team. All rights reserved.</p>
    )";
    
    QMessageBox::about(this, "关于 VideoSR-Lite", aboutText.arg(__DATE__));
}

void MainWindow::showHelp()
{
    QString helpText = R"(
<h2>📖 VideoSR-Lite 使用帮助</h2>

<h3>🚀 快速开始:</h3>
<ol>
<li><b>添加文件:</b> 点击"添加文件"按钮或直接拖拽图像文件到窗口中</li>
<li><b>选择文件:</b> 在左侧文件列表中选择要处理的图像</li>
<li><b>调整设置:</b> 在设置面板中调整处理参数</li>
<li><b>开始处理:</b> 点击"处理当前"或"批量处理"按钮</li>
<li><b>查看结果:</b> 在右侧预览区域查看处理效果</li>
</ol>

<h3>⌨️ 快捷键:</h3>
<ul>
<li><b>Ctrl+O:</b> 打开文件</li>
<li><b>Ctrl+L:</b> 打开文件列表</li>
<li><b>Ctrl+R:</b> 处理当前图像</li>
<li><b>Ctrl+Shift+R:</b> 批量处理</li>
<li><b>Ctrl+P:</b> 暂停/恢复处理</li>
<li><b>Delete:</b> 删除选中文件</li>
</ul>

<h3>💡 使用技巧:</h3>
<ul>
<li>支持拖拽添加文件，可以同时拖入多个文件</li>
<li>原图会立即显示，无需等待处理完成</li>
<li>可以在处理过程中暂停和恢复</li>
<li>使用预设可以快速切换不同的处理参数</li>
<li>支持多种图像格式: JPG, PNG, BMP, TIFF, WebP</li>
</ul>

<p>如需更多帮助，请访问项目主页或联系技术支持。</p>
    )";
    
    QMessageBox::information(this, "使用帮助", helpText);
}

void MainWindow::exit()
{
    close();
}

void MainWindow::onFileSelected(const QString& filePath)
{
    m_currentFilePath = filePath;

    // 立即加载图像用于预览
    loadImageForPreview(filePath);

    // 更新窗口标题
    updateWindowTitle();

    // 更新按钮状态
    m_processCurrentBtn->setEnabled(true);
    m_processCurrentAction->setEnabled(true);

    showStatusMessage(QString("正在加载文件: %1").arg(QFileInfo(filePath).fileName()));
}

void MainWindow::onFileCountChanged(int count)
{
    m_processAllBtn->setEnabled(count > 0);
    m_processAllAction->setEnabled(count > 0);
    
    showStatusMessage(QString("文件列表: %1 个文件").arg(count));
}



void MainWindow::processCurrentImage()
{
    if (m_currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个图像文件！");
        return;
    }

    if (!m_engineReady) {
        QMessageBox::warning(this, "警告", "超分辨率引擎尚未就绪，请稍后再试！");
        return;
    }

    // 异步处理当前图像
    processImageAsync(m_currentFilePath);
    m_isProcessing = true;
    updateProcessingStatus();
    showStatusMessage("🚀 开始处理当前图像...");
}

void MainWindow::processAllImages()
{
    QStringList filePaths = m_fileListWidget->getAllFilePaths();

    if (filePaths.isEmpty()) {
        QMessageBox::warning(this, "警告", "文件列表为空，请先添加图像文件！");
        return;
    }

    if (!m_engineReady) {
        QMessageBox::warning(this, "警告", "超分辨率引擎尚未就绪，请稍后再试！");
        return;
    }

    // 设置处理队列
    m_processingQueue = filePaths;
    m_totalTasks = filePaths.size();
    m_completedTasks = 0;
    m_isProcessing = true;

    // 开始批量处理
    for (const QString& filePath : filePaths) {
        processImageAsync(filePath);
    }

    updateProcessingStatus();
    showStatusMessage(QString("⚡ 开始批量处理 %1 个文件...").arg(filePaths.size()));
}

void MainWindow::pauseProcessing()
{
    if (!m_isProcessing) {
        return;
    }

    if (m_isPaused) {
        // 恢复处理（简化版本，实际上WorkerPool会继续处理队列中的任务）
        m_isPaused = false;
        m_pauseBtn->setText("⏸️ 暂停");
        m_pauseAction->setText("⏸️ 暂停处理");
        showStatusMessage("▶️ 处理已恢复");
    } else {
        // 暂停处理（简化版本，不会停止正在进行的任务，但会标记为暂停状态）
        m_isPaused = true;
        m_pauseBtn->setText("▶️ 恢复");
        m_pauseAction->setText("▶️ 恢复处理");
        showStatusMessage("⏸️ 处理已暂停");
    }

    updateProcessingStatus();
}

void MainWindow::resumeProcessing()
{
    if (m_isPaused) {
        pauseProcessing(); // 切换暂停状态
    }
}

void MainWindow::stopProcessing()
{
    if (!m_isProcessing) {
        return;
    }

    // 清空处理队列
    m_processingQueue.clear();

    m_isProcessing = false;
    m_isPaused = false;
    m_totalTasks = 0;
    m_completedTasks = 0;

    updateProcessingStatus();
    showStatusMessage("⏹️ 处理已停止");
}





void MainWindow::onPreviewSaved(const QString& path)
{
    showStatusMessage(QString("💾 预览已保存到: %1").arg(path));
}

void MainWindow::onImageInfoUpdated(const QString& originalInfo, const QString& enhancedInfo)
{
    // 可以在这里更新状态栏的详细信息
    Q_UNUSED(originalInfo)
    Q_UNUSED(enhancedInfo)
}

void MainWindow::onSettingsChanged(const ProcessingSettings& settings)
{
    m_currentSettings = settings;
    qDebug() << "MainWindow: 设置已更新，缩放倍数:" << settings.scaleFactor;
}

bool MainWindow::isValidImageFile(const QString& filePath)
{
    QMimeDatabase mimeDb;
    QMimeType mimeType = mimeDb.mimeTypeForFile(filePath);
    
    return mimeType.name().startsWith("image/") && 
           (mimeType.name().contains("jpeg") || 
            mimeType.name().contains("png") || 
            mimeType.name().contains("bmp") || 
            mimeType.name().contains("tiff") ||
            mimeType.name().contains("webp"));
}

QStringList MainWindow::extractImageFiles(const QMimeData* mimeData)
{
    QStringList imageFiles;
    
    if (mimeData->hasUrls()) {
        for (const QUrl& url : mimeData->urls()) {
            if (url.isLocalFile()) {
                QString filePath = url.toLocalFile();
                if (isValidImageFile(filePath)) {
                    imageFiles.append(filePath);
                }
            }
        }
    }
    
    return imageFiles;
}

void MainWindow::updateProcessingStatus()
{
    if (m_isProcessing) {
        if (m_isPaused) {
            m_processingStatusLabel->setText("状态：已暂停");
            m_processingStatusLabel->setStyleSheet("color: #ff9800; font-size: 12px;");
        } else {
            m_processingStatusLabel->setText("状态：处理中...");
            m_processingStatusLabel->setStyleSheet("color: #4caf50; font-size: 12px;");
        }
        
        m_pauseBtn->setEnabled(true);
        m_stopBtn->setEnabled(true);
        m_pauseAction->setEnabled(true);
        m_stopAction->setEnabled(true);
        
        m_processCurrentBtn->setEnabled(false);
        m_processAllBtn->setEnabled(false);
        m_processCurrentAction->setEnabled(false);
        m_processAllAction->setEnabled(false);
        
    } else {
        m_processingStatusLabel->setText("状态：待机");
        m_processingStatusLabel->setStyleSheet("color: #666666; font-size: 12px;");
        
        m_pauseBtn->setEnabled(false);
        m_stopBtn->setEnabled(false);
        m_pauseAction->setEnabled(false);
        m_stopAction->setEnabled(false);
        
        m_processCurrentBtn->setEnabled(!m_currentFilePath.isEmpty());
        m_processAllBtn->setEnabled(m_fileListWidget->getFileCount() > 0);
        m_processCurrentAction->setEnabled(!m_currentFilePath.isEmpty());
        m_processAllAction->setEnabled(m_fileListWidget->getFileCount() > 0);
        
        m_overallProgressBar->setVisible(false);
        m_statusProgressBar->setVisible(false);
        
        // 重置按钮文本
        m_pauseBtn->setText("⏸️ 暂停");
        m_pauseAction->setText("⏸️ 暂停处理");
    }
}

void MainWindow::showStatusMessage(const QString& message, int timeout)
{
    m_statusLabel->setText(message);
    if (timeout > 0) {
        m_statusTimer->start(timeout);
    }
}

void MainWindow::updateWindowTitle()
{
    QString title = "VideoSR-Lite - 智能视频超分辨率工具";

    if (!m_currentFilePath.isEmpty()) {
        title += QString(" - %1").arg(QFileInfo(m_currentFilePath).fileName());
    }

    setWindowTitle(title);
}

// 超分引擎初始化
void MainWindow::initializeSuperResEngine()
{
    m_workerPool->submitTask([this]() {
        try {
            qDebug() << "开始初始化超分辨率引擎...";
            m_srEngine = std::make_unique<SuperEigen::SuperResEngine>();

            // 构建正确的模型路径
            QString appDir = QApplication::applicationDirPath();
            QString modelPath = appDir + "/../../onnx/RealESRGAN_x2plus.fp16.onnx";

            qDebug() << "尝试加载模型:" << modelPath;

            // 尝试初始化引擎
            if (m_srEngine->initialize(modelPath.toStdString())) {
                m_engineReady = true;
                qDebug() << "超分辨率引擎初始化成功";

                // 在主线程更新UI
                QMetaObject::invokeMethod(this, [this]() {
                    showStatusMessage("✅ 超分辨率引擎初始化成功", 3000);
                }, Qt::QueuedConnection);
            } else {
                qWarning() << "超分辨率引擎初始化失败";

                QMetaObject::invokeMethod(this, [this]() {
                    showStatusMessage("⚠️ 超分辨率引擎初始化失败，将使用预览模式", 5000);
                }, Qt::QueuedConnection);
            }
        } catch (const std::exception& e) {
            qWarning() << "超分辨率引擎初始化异常:" << e.what();

            QMetaObject::invokeMethod(this, [this, error = QString(e.what())]() {
                showStatusMessage(QString("❌ 引擎初始化异常: %1").arg(error), 5000);
            }, Qt::QueuedConnection);
        }
    });
}

// 加载图像用于立即预览
void MainWindow::loadImageForPreview(const QString& filePath)
{
    // 检查缓存
    {
        QMutexLocker locker(&m_cacheMutex);
        if (m_imageCache.contains(filePath)) {
            // 立即显示缓存的图像
            m_imagePreviewWidget->setOriginalImage(m_imageCache[filePath]);
            return;
        }
    }

    // 异步加载图像
    m_workerPool->submitTask([this, filePath]() {
        try {
            cv::Mat image = cv::imread(filePath.toStdString());
            if (image.empty()) {
                qWarning() << "无法加载图像:" << filePath;
                return;
            }

            // 缓存图像
            {
                QMutexLocker locker(&m_cacheMutex);
                m_imageCache[filePath] = image.clone();
            }

            // 在主线程更新预览
            QMetaObject::invokeMethod(this, [this, filePath, image]() {
                m_imagePreviewWidget->setOriginalImage(image);
                showStatusMessage(QString("✅ 图像加载完成: %1").arg(QFileInfo(filePath).fileName()), 2000);
            }, Qt::QueuedConnection);

        } catch (const std::exception& e) {
            qWarning() << "加载图像异常:" << e.what();

            QMetaObject::invokeMethod(this, [this, error = QString(e.what())]() {
                showStatusMessage(QString("❌ 图像加载失败: %1").arg(error), 3000);
            }, Qt::QueuedConnection);
        }
    });
}

// 异步处理图像
void MainWindow::processImageAsync(const QString& filePath)
{
    if (!m_engineReady) {
        showStatusMessage("⚠️ 超分辨率引擎未就绪", 3000);
        return;
    }

    m_workerPool->submitTask([this, filePath]() {
        try {
            QElapsedTimer timer;
            timer.start();

            // 获取原始图像
            cv::Mat originalImage;
            {
                QMutexLocker locker(&m_cacheMutex);
                if (m_imageCache.contains(filePath)) {
                    originalImage = m_imageCache[filePath].clone();
                } else {
                    originalImage = cv::imread(filePath.toStdString());
                    if (!originalImage.empty()) {
                        m_imageCache[filePath] = originalImage.clone();
                    }
                }
            }

            if (originalImage.empty()) {
                throw std::runtime_error("无法读取图像文件");
            }

            // 执行超分辨率处理
            cv::Mat enhancedImage = m_srEngine->Process(originalImage);
            if (enhancedImage.empty()) {
                throw std::runtime_error("超分辨率处理失败");
            }

            qint64 processingTime = timer.elapsed();

            // 在主线程更新结果
            QMetaObject::invokeMethod(this, [this, filePath, originalImage, enhancedImage, processingTime]() {
                onImageProcessed(filePath, originalImage, enhancedImage, processingTime);
            }, Qt::QueuedConnection);

        } catch (const std::exception& e) {
            QString error = e.what();
            QMetaObject::invokeMethod(this, [this, filePath, error]() {
                onImageProcessError(filePath, error);
            }, Qt::QueuedConnection);
        }
    });
}

// 处理结果回调
void MainWindow::onImageProcessed(const QString& filePath, const cv::Mat& originalImage,
                                 const cv::Mat& enhancedImage, qint64 processingTime)
{
    // 更新预览（如果是当前选中的文件）
    if (filePath == m_currentFilePath) {
        m_imagePreviewWidget->setOriginalImage(originalImage);
        m_imagePreviewWidget->setEnhancedImage(enhancedImage);
    }

    // 更新进度
    m_completedTasks++;
    onProcessingProgress(m_completedTasks, m_totalTasks);

    QString fileName = QFileInfo(filePath).fileName();
    showStatusMessage(QString("✅ 处理完成: %1 (用时 %2ms)").arg(fileName).arg(processingTime), 2000);

    // 检查是否所有任务都完成
    if (m_completedTasks >= m_totalTasks) {
        m_isProcessing = false;
        updateProcessingStatus();
        showStatusMessage("🎉 所有图像处理完成！", 3000);
    }
}

void MainWindow::onImageProcessError(const QString& filePath, const QString& error)
{
    QString fileName = QFileInfo(filePath).fileName();
    qWarning() << "图像处理失败:" << fileName << error;

    // 更新进度（即使失败也要计入）
    m_completedTasks++;
    onProcessingProgress(m_completedTasks, m_totalTasks);

    showStatusMessage(QString("❌ 处理失败: %1 - %2").arg(fileName).arg(error), 5000);

    // 检查是否所有任务都完成
    if (m_completedTasks >= m_totalTasks) {
        m_isProcessing = false;
        updateProcessingStatus();
        showStatusMessage("⚠️ 批量处理完成（部分失败）", 3000);
    }
}

void MainWindow::onProcessingProgress(int completed, int total)
{
    if (total > 0) {
        int percentage = (completed * 100) / total;
        m_overallProgressBar->setValue(percentage);
        m_statusProgressBar->setValue(percentage);

        QString progressText = QString("处理进度: %1/%2 (%3%)")
                              .arg(completed).arg(total).arg(percentage);
        m_processingStatusLabel->setText(progressText);
    }
}


