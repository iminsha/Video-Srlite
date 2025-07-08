#ifndef IMAGEPREVIEWWIDGET_H
#define IMAGEPREVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPixmap>
#include <QSize>
#include <QScrollBar>
#include <opencv2/opencv.hpp>

// 对比模式枚举
enum class CompareMode {
    SideBySide = 0,    // 并排对比
    SliderCompare = 1, // 滑动对比
    ToggleCompare = 2, // 切换对比
    SyncZoom = 3       // 同步缩放
};

class ImagePreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePreviewWidget(QWidget *parent = nullptr);
    ~ImagePreviewWidget();

    // 设置图像
    void setOriginalImage(const cv::Mat& image);
    void setEnhancedImage(const cv::Mat& image);
    void clearPreview();
    
    // 获取当前图像
    cv::Mat getCurrentOriginalImage() const { return m_originalImage; }
    cv::Mat getCurrentEnhancedImage() const { return m_enhancedImage; }
    
    // 缩放控制
    void setZoomFactor(double factor);
    void setZoomMode(const QString& mode);
    
    // 对比模式
    void setCompareMode(CompareMode mode);
    CompareMode getCompareMode() const { return m_compareMode; }

public slots:
    void onZoomChanged(const QString& zoomText);
    void onCompareModeChanged(int index);
    void refreshPreview();
    void savePreview();

signals:
    void previewSaved(const QString& path);
    void imageInfoUpdated(const QString& originalInfo, const QString& enhancedInfo);

private:
    void setupUI();
    void createSideBySidePage();
    void createSliderComparePage();
    void connectSignals();
    void connectScrollBars();
    void updateDisplay();
    
    // 显示模式
    QSize calculateDisplaySize(const cv::Mat& referenceImage);
    void displaySideBySideComparison();
    void displaySliderComparison();
    void displayToggleComparison();
    void updatePreviewTitles();
    void updateImageInfo();
    
    // 图像处理辅助方法
    QPixmap matToQPixmap(const cv::Mat& mat);
    cv::Mat createComparisonImage(const cv::Mat& original, const cv::Mat& enhanced);

private:
    // UI组件
    QGroupBox* m_previewGroupBox;
    QWidget* m_toolbarWidget;
    QComboBox* m_zoomComboBox;
    QComboBox* m_compareModeComboBox;
    QPushButton* m_refreshBtn;
    QPushButton* m_saveBtn;
    
    QStackedWidget* m_stackedWidget;
    
    // 并排对比页面
    QWidget* m_sideBySidePage;
    QScrollArea* m_originalScrollArea;
    QScrollArea* m_enhancedScrollArea;
    QLabel* m_originalImageLabel;
    QLabel* m_enhancedImageLabel;
    QLabel* m_originalTitleLabel;
    QLabel* m_enhancedTitleLabel;
    
    // 滑动对比页面
    QWidget* m_sliderComparePage;
    QLabel* m_sliderCompareLabel;
    
    // 信息显示
    QWidget* m_infoWidget;
    QLabel* m_originalInfoLabel;
    QLabel* m_enhancedInfoLabel;
    QLabel* m_processingTimeLabel;
    
    // 数据
    cv::Mat m_originalImage;
    cv::Mat m_enhancedImage;
    double m_zoomFactor;
    CompareMode m_compareMode;
};

#endif // IMAGEPREVIEWWIDGET_H 