#include "ImagePreviewWidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

ImagePreviewWidget::ImagePreviewWidget(QWidget *parent)
    : QWidget(parent)
    , m_zoomFactor(1.0)
    , m_compareMode(CompareMode::SideBySide)
{
    setupUI();
    connectSignals();
}

ImagePreviewWidget::~ImagePreviewWidget()
{
}

void ImagePreviewWidget::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    
    // 预览分组框
    m_previewGroupBox = new QGroupBox("🔍 图像预览对比", this);
    mainLayout->addWidget(m_previewGroupBox);
    
    QVBoxLayout* previewLayout = new QVBoxLayout(m_previewGroupBox);
    previewLayout->setSpacing(12);
    
    // 工具栏
    m_toolbarWidget = new QWidget(this);
    m_toolbarWidget->setObjectName("previewToolbar");
    previewLayout->addWidget(m_toolbarWidget);
    
    QHBoxLayout* toolbarLayout = new QHBoxLayout(m_toolbarWidget);
    toolbarLayout->setSpacing(12);
    
    // 刷新按钮
    m_refreshBtn = new QPushButton("🔄 刷新", this);
    toolbarLayout->addWidget(m_refreshBtn);
    
    // 缩放控制
    toolbarLayout->addWidget(new QLabel("🔍 缩放："));
    m_zoomComboBox = new QComboBox(this);
    m_zoomComboBox->addItems({"25%", "50%", "75%", "100%", "150%", "200%", "300%", "适合窗口", "实际大小"});
    m_zoomComboBox->setCurrentText("100%");
    toolbarLayout->addWidget(m_zoomComboBox);
    
    // 对比模式
    toolbarLayout->addWidget(new QLabel("📋 对比："));
    m_compareModeComboBox = new QComboBox(this);
    m_compareModeComboBox->addItems({"并排对比", "滑动对比", "切换对比", "同步缩放"});
    toolbarLayout->addWidget(m_compareModeComboBox);
    
    toolbarLayout->addStretch();
    
    // 保存按钮
    m_saveBtn = new QPushButton("💾 保存", this);
    toolbarLayout->addWidget(m_saveBtn);
    
    // 堆叠组件
    m_stackedWidget = new QStackedWidget(this);
    previewLayout->addWidget(m_stackedWidget);
    
    // 创建并排对比页面
    createSideBySidePage();
    
    // 创建滑动对比页面
    createSliderComparePage();
    
    // 信息显示区域
    m_infoWidget = new QWidget(this);
    m_infoWidget->setMaximumHeight(60);
    previewLayout->addWidget(m_infoWidget);
    
    QHBoxLayout* infoLayout = new QHBoxLayout(m_infoWidget);
    infoLayout->setSpacing(20);
    
    m_originalInfoLabel = new QLabel("原图信息：暂无", this);
    m_originalInfoLabel->setStyleSheet("color: #666666; font-size: 12px;");
    infoLayout->addWidget(m_originalInfoLabel);
    
    m_enhancedInfoLabel = new QLabel("增强图信息：暂无", this);
    m_enhancedInfoLabel->setStyleSheet("color: #666666; font-size: 12px;");
    infoLayout->addWidget(m_enhancedInfoLabel);
    
    m_processingTimeLabel = new QLabel("处理时间：--", this);
    m_processingTimeLabel->setStyleSheet("color: #666666; font-size: 12px;");
    infoLayout->addWidget(m_processingTimeLabel);
    
    infoLayout->addStretch();
    
    // 设置默认状态
    m_stackedWidget->setCurrentIndex(0);
    clearPreview();
}

void ImagePreviewWidget::createSideBySidePage()
{
    m_sideBySidePage = new QWidget(this);
    m_stackedWidget->addWidget(m_sideBySidePage);
    
    QHBoxLayout* layout = new QHBoxLayout(m_sideBySidePage);
    layout->setSpacing(12);
    
    // 原图区域
    QWidget* originalWidget = new QWidget(this);
    layout->addWidget(originalWidget);
    
    QVBoxLayout* originalLayout = new QVBoxLayout(originalWidget);
    originalLayout->setSpacing(8);
    
    m_originalTitleLabel = new QLabel("📷 原始画面", this);
    m_originalTitleLabel->setAlignment(Qt::AlignCenter);
    m_originalTitleLabel->setObjectName("originalTitleLabel");
    originalLayout->addWidget(m_originalTitleLabel);
    
    m_originalScrollArea = new QScrollArea(this);
    m_originalScrollArea->setMinimumSize(680, 680);
    m_originalScrollArea->setWidgetResizable(true);
    originalLayout->addWidget(m_originalScrollArea);
    
    m_originalImageLabel = new QLabel(this);
    m_originalImageLabel->setMinimumSize(660, 660);
    m_originalImageLabel->setAlignment(Qt::AlignCenter);
    m_originalImageLabel->setObjectName("originalImageLabel");
    m_originalImageLabel->setText("🖼️ 暂无预览\n请选择一个图片文件");
    m_originalScrollArea->setWidget(m_originalImageLabel);
    
    // 增强图区域
    QWidget* enhancedWidget = new QWidget(this);
    layout->addWidget(enhancedWidget);
    
    QVBoxLayout* enhancedLayout = new QVBoxLayout(enhancedWidget);
    enhancedLayout->setSpacing(8);
    
    m_enhancedTitleLabel = new QLabel("✨ 超分辨率后", this);
    m_enhancedTitleLabel->setAlignment(Qt::AlignCenter);
    m_enhancedTitleLabel->setObjectName("enhancedTitleLabel");
    enhancedLayout->addWidget(m_enhancedTitleLabel);
    
    m_enhancedScrollArea = new QScrollArea(this);
    m_enhancedScrollArea->setMinimumSize(680, 680);
    m_enhancedScrollArea->setWidgetResizable(true);
    enhancedLayout->addWidget(m_enhancedScrollArea);
    
    m_enhancedImageLabel = new QLabel(this);
    m_enhancedImageLabel->setMinimumSize(660, 660);
    m_enhancedImageLabel->setAlignment(Qt::AlignCenter);
    m_enhancedImageLabel->setObjectName("enhancedImageLabel");
    m_enhancedImageLabel->setText("⚡ 等待处理\n超分辨率预览将在此显示");
    m_enhancedScrollArea->setWidget(m_enhancedImageLabel);
    
    // 同步滚动
    connectScrollBars();
}

void ImagePreviewWidget::createSliderComparePage()
{
    m_sliderComparePage = new QWidget(this);
    m_stackedWidget->addWidget(m_sliderComparePage);
    
    QVBoxLayout* layout = new QVBoxLayout(m_sliderComparePage);
    
    m_sliderCompareLabel = new QLabel("🚧 滑动对比模式正在开发中\n将在后续版本中提供", this);
    m_sliderCompareLabel->setMinimumSize(1360, 680);
    m_sliderCompareLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_sliderCompareLabel);
}

void ImagePreviewWidget::connectSignals()
{
    connect(m_refreshBtn, &QPushButton::clicked, this, &ImagePreviewWidget::refreshPreview);
    connect(m_saveBtn, &QPushButton::clicked, this, &ImagePreviewWidget::savePreview);
    connect(m_zoomComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &ImagePreviewWidget::onZoomChanged);
    connect(m_compareModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ImagePreviewWidget::onCompareModeChanged);
}

void ImagePreviewWidget::connectScrollBars()
{
    if (!m_originalScrollArea || !m_enhancedScrollArea) return;
    
    // 同步两个滚动区域的滚动条
    QScrollBar* originalHBar = m_originalScrollArea->horizontalScrollBar();
    QScrollBar* originalVBar = m_originalScrollArea->verticalScrollBar();
    QScrollBar* enhancedHBar = m_enhancedScrollArea->horizontalScrollBar();
    QScrollBar* enhancedVBar = m_enhancedScrollArea->verticalScrollBar();
    
    // 水平同步
    connect(originalHBar, &QScrollBar::valueChanged, enhancedHBar, &QScrollBar::setValue);
    connect(enhancedHBar, &QScrollBar::valueChanged, originalHBar, &QScrollBar::setValue);
    
    // 垂直同步
    connect(originalVBar, &QScrollBar::valueChanged, enhancedVBar, &QScrollBar::setValue);
    connect(enhancedVBar, &QScrollBar::valueChanged, originalVBar, &QScrollBar::setValue);
}

void ImagePreviewWidget::setOriginalImage(const cv::Mat& image)
{
    m_originalImage = image.clone();
    
    // 立即显示原图，不等待超分辨率处理
    if (!m_originalImage.empty()) {
        updateDisplay();
        updateImageInfo();
    }
}

void ImagePreviewWidget::setEnhancedImage(const cv::Mat& image)
{
    m_enhancedImage = image.clone();
    
    // 更新显示
    if (!m_enhancedImage.empty()) {
        updateDisplay();
        updateImageInfo();
    }
}

void ImagePreviewWidget::clearPreview()
{
    m_originalImage = cv::Mat();
    m_enhancedImage = cv::Mat();
    
    m_originalImageLabel->setText("🖼️ 暂无预览\n请选择一个图片文件");
    m_enhancedImageLabel->setText("⚡ 等待处理\n超分辨率预览将在此显示");
    
    m_originalInfoLabel->setText("原图信息：暂无");
    m_enhancedInfoLabel->setText("增强图信息：暂无");
    m_processingTimeLabel->setText("处理时间：--");
    
    updatePreviewTitles();
}

void ImagePreviewWidget::setZoomFactor(double factor)
{
    m_zoomFactor = factor;
    updateDisplay();
}

void ImagePreviewWidget::setZoomMode(const QString& mode)
{
    m_zoomComboBox->setCurrentText(mode);
    onZoomChanged(mode);
}

void ImagePreviewWidget::setCompareMode(CompareMode mode)
{
    m_compareMode = mode;
    m_compareModeComboBox->setCurrentIndex(static_cast<int>(mode));
    m_stackedWidget->setCurrentIndex(mode < CompareMode::ToggleCompare ? 0 : 1);
    updateDisplay();
}

void ImagePreviewWidget::onZoomChanged(const QString& zoomText)
{
    if (zoomText == "适合窗口") {
        m_zoomFactor = -1; // 特殊值表示适合窗口
    } else if (zoomText == "实际大小") {
        m_zoomFactor = 0; // 特殊值表示实际大小
    } else {
        QString cleanText = zoomText;
        cleanText.remove('%');
        bool ok;
        int zoomValue = cleanText.toInt(&ok);
        
        if (ok) {
            m_zoomFactor = zoomValue / 100.0;
        } else {
            m_zoomFactor = 1.0;
        }
    }
    
    updateDisplay();
}

void ImagePreviewWidget::onCompareModeChanged(int index)
{
    setCompareMode(static_cast<CompareMode>(index));
}

void ImagePreviewWidget::refreshPreview()
{
    updateDisplay();
}

void ImagePreviewWidget::savePreview()
{
    if (m_originalImage.empty() && m_enhancedImage.empty()) {
        QMessageBox::warning(this, "警告", "没有可保存的预览图像！");
        return;
    }
    
    QString saveDir = QFileDialog::getExistingDirectory(this, "选择保存目录", 
                                                       QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if (saveDir.isEmpty()) {
        return;
    }
    
    try {
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        
        if (!m_originalImage.empty()) {
            QString originalPath = saveDir + "/original_" + timestamp + ".png";
            cv::imwrite(originalPath.toStdString(), m_originalImage);
        }
        
        if (!m_enhancedImage.empty()) {
            QString enhancedPath = saveDir + "/enhanced_" + timestamp + ".png";
            cv::imwrite(enhancedPath.toStdString(), m_enhancedImage);
        }
        
        if (!m_originalImage.empty() && !m_enhancedImage.empty()) {
            QString comparisonPath = saveDir + "/comparison_" + timestamp + ".png";
            cv::Mat comparison = createComparisonImage(m_originalImage, m_enhancedImage);
            cv::imwrite(comparisonPath.toStdString(), comparison);
        }
        
        emit previewSaved(saveDir);
        QMessageBox::information(this, "保存成功", "预览图像已保存到：\n" + saveDir);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "保存失败", QString("保存失败：%1").arg(e.what()));
    }
}

void ImagePreviewWidget::updateDisplay()
{
    switch (m_compareMode) {
        case CompareMode::SideBySide:
        case CompareMode::SyncZoom:
            displaySideBySideComparison();
            break;
        case CompareMode::SliderCompare:
            displaySliderComparison();
            break;
        case CompareMode::ToggleCompare:
            displayToggleComparison();
            break;
    }
    
    updatePreviewTitles();
}

QSize ImagePreviewWidget::calculateDisplaySize(const cv::Mat& referenceImage)
{
    if (referenceImage.empty()) {
        return QSize(660, 660);
    }
    
    if (m_zoomFactor == -1) { // 适合窗口
        QSize availableSize(660, 660);
        double scaleX = double(availableSize.width()) / referenceImage.cols;
        double scaleY = double(availableSize.height()) / referenceImage.rows;
        double scale = std::min(scaleX, scaleY);
        
        return QSize(int(referenceImage.cols * scale), int(referenceImage.rows * scale));
    } else if (m_zoomFactor == 0) { // 实际大小
        return QSize(referenceImage.cols, referenceImage.rows);
    } else { // 指定缩放比例
        return QSize(int(referenceImage.cols * m_zoomFactor), int(referenceImage.rows * m_zoomFactor));
    }
}

void ImagePreviewWidget::displaySideBySideComparison()
{
    // 显示原图
    if (!m_originalImage.empty()) {
        QSize displaySize = calculateDisplaySize(m_originalImage);
        QPixmap originalPixmap = matToQPixmap(m_originalImage);
        originalPixmap = originalPixmap.scaled(displaySize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_originalImageLabel->setPixmap(originalPixmap);
        m_originalImageLabel->resize(originalPixmap.size());
    } else {
        m_originalImageLabel->setText("🖼️ 暂无预览\n请选择一个图片文件");
        m_originalImageLabel->resize(660, 660);
    }
    
    // 显示增强图
    if (!m_enhancedImage.empty()) {
        QSize displaySize = calculateDisplaySize(m_enhancedImage);
        QPixmap enhancedPixmap = matToQPixmap(m_enhancedImage);
        enhancedPixmap = enhancedPixmap.scaled(displaySize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_enhancedImageLabel->setPixmap(enhancedPixmap);
        m_enhancedImageLabel->resize(enhancedPixmap.size());
    } else {
        m_enhancedImageLabel->setText("⚡ 等待处理\n超分辨率预览将在此显示");
        m_enhancedImageLabel->resize(660, 660);
    }
}

void ImagePreviewWidget::displaySliderComparison()
{
    m_sliderCompareLabel->setText("🚧 滑动对比模式正在开发中\n将在后续版本中提供");
}

void ImagePreviewWidget::displayToggleComparison()
{
    // TODO: 实现切换对比功能
    displaySideBySideComparison();
}

void ImagePreviewWidget::updatePreviewTitles()
{
    QString originalTitle = "📷 原始画面";
    QString enhancedTitle = "✨ 超分辨率后";
    
    if (!m_originalImage.empty()) {
        originalTitle += QString(" (%1×%2)").arg(m_originalImage.cols).arg(m_originalImage.rows);
    }
    
    if (!m_enhancedImage.empty()) {
        int scaleFactor = m_enhancedImage.empty() || m_originalImage.empty() ? 0 : m_enhancedImage.cols / m_originalImage.cols;
        enhancedTitle += QString(" (%1×%2").arg(m_enhancedImage.cols).arg(m_enhancedImage.rows);
        if (scaleFactor > 1) {
            enhancedTitle += QString(", ×%1").arg(scaleFactor);
        }
        enhancedTitle += ")";
    }
    
    if (m_zoomFactor > 0 && m_zoomFactor != 1.0) {
        QString zoomInfo = QString(" [显示: %1%]").arg(int(m_zoomFactor * 100));
        originalTitle += zoomInfo;
        enhancedTitle += zoomInfo;
    } else if (m_zoomFactor == -1) {
        QString zoomInfo = " [适合窗口]";
        originalTitle += zoomInfo;
        enhancedTitle += zoomInfo;
    } else if (m_zoomFactor == 0) {
        QString zoomInfo = " [实际大小]";
        originalTitle += zoomInfo;
        enhancedTitle += zoomInfo;
    }
    
    m_originalTitleLabel->setText(originalTitle);
    m_enhancedTitleLabel->setText(enhancedTitle);
}

void ImagePreviewWidget::updateImageInfo()
{
    // 更新原图信息
    if (!m_originalImage.empty()) {
        QString info = QString("原图信息：%1×%2, %3通道, %4")
                          .arg(m_originalImage.cols)
                          .arg(m_originalImage.rows)
                          .arg(m_originalImage.channels())
                          .arg(m_originalImage.depth() == CV_8U ? "8位" : "其他位深");
        
        double imageSizeMB = (m_originalImage.cols * m_originalImage.rows * m_originalImage.channels()) / (1024.0 * 1024.0);
        info += QString(", ~%1MB").arg(imageSizeMB, 0, 'f', 2);
        
        m_originalInfoLabel->setText(info);
    } else {
        m_originalInfoLabel->setText("原图信息：暂无");
    }
    
    // 更新增强图信息
    if (!m_enhancedImage.empty()) {
        QString info = QString("增强图信息：%1×%2, %3通道, %4")
                          .arg(m_enhancedImage.cols)
                          .arg(m_enhancedImage.rows)
                          .arg(m_enhancedImage.channels())
                          .arg(m_enhancedImage.depth() == CV_8U ? "8位" : "其他位深");
        
        double imageSizeMB = (m_enhancedImage.cols * m_enhancedImage.rows * m_enhancedImage.channels()) / (1024.0 * 1024.0);
        info += QString(", ~%1MB").arg(imageSizeMB, 0, 'f', 2);
        
        m_enhancedInfoLabel->setText(info);
    } else {
        m_enhancedInfoLabel->setText("增强图信息：暂无");
    }
    
    emit imageInfoUpdated(m_originalInfoLabel->text(), m_enhancedInfoLabel->text());
}

QPixmap ImagePreviewWidget::matToQPixmap(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QPixmap();
    }
    
    cv::Mat rgb;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGB);
    } else {
        mat.copyTo(rgb);
    }
    
    QImage qimg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    return QPixmap::fromImage(qimg);
}

cv::Mat ImagePreviewWidget::createComparisonImage(const cv::Mat& original, const cv::Mat& enhanced)
{
    if (original.empty() || enhanced.empty()) {
        return cv::Mat();
    }
    
    // 将原图缩放到与增强图相同的尺寸（用于公平对比）
    cv::Mat originalResized;
    cv::resize(original, originalResized, cv::Size(enhanced.cols, enhanced.rows), 0, 0, cv::INTER_CUBIC);
    
    // 创建并排对比图
    cv::Mat comparisonImage(enhanced.rows, enhanced.cols * 2 + 20, enhanced.type(), cv::Scalar(255, 255, 255));
    
    // 复制原图到左半部分
    originalResized.copyTo(comparisonImage(cv::Rect(0, 0, enhanced.cols, enhanced.rows)));
    
    // 复制增强图到右半部分
    enhanced.copyTo(comparisonImage(cv::Rect(enhanced.cols + 20, 0, enhanced.cols, enhanced.rows)));
    
    // 添加文本标注
    cv::putText(comparisonImage, "Original", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(comparisonImage, "Enhanced", cv::Point(enhanced.cols + 30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    
    return comparisonImage;
} 