#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>

// 处理参数结构体
struct ProcessingSettings {
    int scaleFactor = 2;               // 超分辨率倍数 (2 或 4)
    bool enableDenoising = true;       // 是否启用降噪
    double denoiseStrength = 0.3;      // 降噪强度 (0.0 - 1.0)
    double sharpenStrength = 0.2;      // 锐化强度 (0.0 - 1.0)
    bool enableGPU = true;             // 是否启用GPU加速
    int batchSize = 1;                 // 批处理大小
    QString outputPath;                // 输出路径
    QString outputFormat = "png";      // 输出格式
    int jpegQuality = 95;              // JPEG质量 (1-100)
    bool preserveOriginalName = true;  // 保持原文件名
    QString nameSuffix = "_enhanced";  // 文件名后缀
};

class SettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPanel(QWidget *parent = nullptr);
    ~SettingsPanel();

    // 设置管理
    ProcessingSettings getSettings() const;
    void setSettings(const ProcessingSettings& settings);
    void resetToDefaults();
    
    // 预设配置
    void loadPreset(const QString& presetName);
    void savePreset(const QString& presetName);
    QStringList getAvailablePresets() const;

public slots:
    void onScaleFactorChanged(int factor);
    void onDenoiseStrengthChanged(double strength);
    void onSharpenStrengthChanged(double strength);
    void onOutputPathChanged();
    void onOutputFormatChanged(const QString& format);
    void onApplyClicked();
    void onResetClicked();
    void onSavePresetClicked();
    void onLoadPresetClicked();

signals:
    void settingsChanged(const ProcessingSettings& settings);
    void presetLoaded(const QString& presetName);
    void presetSaved(const QString& presetName);

private:
    void setupUI();
    void connectSignals();
    void updateUI();
    void updateSliderLabels();
    void validateSettings();

private:
    // 处理设置组
    QGroupBox* m_processingGroupBox;
    QLabel* m_scaleFactorLabel;
    QComboBox* m_scaleFactorComboBox;
    
    QCheckBox* m_enableDenoisingCheckBox;
    QLabel* m_denoiseStrengthLabel;
    QSlider* m_denoiseStrengthSlider;
    QDoubleSpinBox* m_denoiseStrengthSpinBox;
    
    QLabel* m_sharpenStrengthLabel;
    QSlider* m_sharpenStrengthSlider;
    QDoubleSpinBox* m_sharpenStrengthSpinBox;
    
    // 性能设置组
    QGroupBox* m_performanceGroupBox;
    QCheckBox* m_enableGPUCheckBox;
    QLabel* m_batchSizeLabel;
    QSpinBox* m_batchSizeSpinBox;
    
    // 输出设置组
    QGroupBox* m_outputGroupBox;
    QLabel* m_outputPathLabel;
    QLineEdit* m_outputPathLineEdit;
    QPushButton* m_browseOutputPathBtn;
    
    QLabel* m_outputFormatLabel;
    QComboBox* m_outputFormatComboBox;
    
    QLabel* m_jpegQualityLabel;
    QSlider* m_jpegQualitySlider;
    QSpinBox* m_jpegQualitySpinBox;
    
    QCheckBox* m_preserveOriginalNameCheckBox;
    QLabel* m_nameSuffixLabel;
    QLineEdit* m_nameSuffixLineEdit;
    
    // 预设管理组
    QGroupBox* m_presetGroupBox;
    QComboBox* m_presetComboBox;
    QPushButton* m_savePresetBtn;
    QPushButton* m_loadPresetBtn;
    QPushButton* m_deletePresetBtn;
    
    // 操作按钮
    QWidget* m_buttonWidget;
    QPushButton* m_applyBtn;
    QPushButton* m_resetBtn;
    
    // 数据
    ProcessingSettings m_currentSettings;
    QStringList m_availablePresets;
};

#endif // SETTINGSPANEL_H
