#include "SettingsPanel.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include <QSettings>
#include <QDir>
#include <QDebug>

SettingsPanel::SettingsPanel(QWidget *parent)
    : QWidget(parent)
{
    // 设置默认值
    m_currentSettings = ProcessingSettings();
    setupUI();
    connectSignals();
    updateUI();
}

SettingsPanel::~SettingsPanel()
{
}

void SettingsPanel::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);  // 减少间距
    mainLayout->setContentsMargins(6, 6, 6, 6);  // 减少边距
    
    // 处理设置组（简化版）
    m_processingGroupBox = new QGroupBox("🎛️ 处理参数", this);
    mainLayout->addWidget(m_processingGroupBox);
    
    QVBoxLayout* processingLayout = new QVBoxLayout(m_processingGroupBox);
    processingLayout->setSpacing(6);
    
    // 缩放倍数
    QHBoxLayout* scaleLayout = new QHBoxLayout();
    m_scaleFactorLabel = new QLabel("缩放倍数：", this);
    scaleLayout->addWidget(m_scaleFactorLabel);
    
    m_scaleFactorComboBox = new QComboBox(this);
    m_scaleFactorComboBox->addItems({"2x", "4x"});
    m_scaleFactorComboBox->setToolTip("选择超分辨率缩放倍数");
    scaleLayout->addWidget(m_scaleFactorComboBox);
    scaleLayout->addStretch();
    processingLayout->addLayout(scaleLayout);
    
    // 降噪设置
    m_enableDenoisingCheckBox = new QCheckBox("🎯 启用降噪", this);
    processingLayout->addWidget(m_enableDenoisingCheckBox);
    
    // 性能设置组（简化版）
    m_performanceGroupBox = new QGroupBox("⚡ 性能设置", this);
    mainLayout->addWidget(m_performanceGroupBox);
    
    QVBoxLayout* performanceLayout = new QVBoxLayout(m_performanceGroupBox);
    performanceLayout->setSpacing(6);
    
    m_enableGPUCheckBox = new QCheckBox("🚀 启用GPU加速", this);
    performanceLayout->addWidget(m_enableGPUCheckBox);
    
    // 预设管理组（简化版）
    m_presetGroupBox = new QGroupBox("📋 预设管理", this);
    mainLayout->addWidget(m_presetGroupBox);
    
    QVBoxLayout* presetLayout = new QVBoxLayout(m_presetGroupBox);
    presetLayout->setSpacing(6);
    
    m_presetComboBox = new QComboBox(this);
    m_presetComboBox->addItems({"默认设置", "快速处理", "高质量处理", "降噪优先"});
    presetLayout->addWidget(m_presetComboBox);
    
    QHBoxLayout* presetButtonLayout = new QHBoxLayout();
    m_loadPresetBtn = new QPushButton("📥 测试", this);
    m_loadPresetBtn->setMaximumHeight(30);
    presetButtonLayout->addWidget(m_loadPresetBtn);
    
    m_applyBtn = new QPushButton("✅ 应用", this);
    m_applyBtn->setMaximumHeight(30);
    presetButtonLayout->addWidget(m_applyBtn);
    
    presetLayout->addLayout(presetButtonLayout);
    
    mainLayout->addStretch();
    
    // 创建被简化掉的组件（设置为空指针或创建简单版本）
    m_denoiseStrengthLabel = new QLabel(this);
    m_denoiseStrengthSlider = new QSlider(this);
    m_denoiseStrengthSpinBox = new QDoubleSpinBox(this);
    m_sharpenStrengthLabel = new QLabel(this);
    m_sharpenStrengthSlider = new QSlider(this);
    m_sharpenStrengthSpinBox = new QDoubleSpinBox(this);
    m_batchSizeLabel = new QLabel(this);
    m_batchSizeSpinBox = new QSpinBox(this);
    m_outputGroupBox = new QGroupBox(this);
    m_outputPathLabel = new QLabel(this);
    m_outputPathLineEdit = new QLineEdit(this);
    m_browseOutputPathBtn = new QPushButton(this);
    m_outputFormatLabel = new QLabel(this);
    m_outputFormatComboBox = new QComboBox(this);
    m_jpegQualityLabel = new QLabel(this);
    m_jpegQualitySlider = new QSlider(this);
    m_jpegQualitySpinBox = new QSpinBox(this);
    m_preserveOriginalNameCheckBox = new QCheckBox(this);
    m_nameSuffixLabel = new QLabel(this);
    m_nameSuffixLineEdit = new QLineEdit(this);
    m_savePresetBtn = new QPushButton(this);
    m_deletePresetBtn = new QPushButton(this);
    m_buttonWidget = new QWidget(this);
    m_resetBtn = new QPushButton(this);
    
    // 隐藏这些组件
    m_denoiseStrengthLabel->hide();
    m_denoiseStrengthSlider->hide();
    m_denoiseStrengthSpinBox->hide();
    m_sharpenStrengthLabel->hide();
    m_sharpenStrengthSlider->hide();
    m_sharpenStrengthSpinBox->hide();
    m_batchSizeLabel->hide();
    m_batchSizeSpinBox->hide();
    m_outputGroupBox->hide();
    m_outputPathLabel->hide();
    m_outputPathLineEdit->hide();
    m_browseOutputPathBtn->hide();
    m_outputFormatLabel->hide();
    m_outputFormatComboBox->hide();
    m_jpegQualityLabel->hide();
    m_jpegQualitySlider->hide();
    m_jpegQualitySpinBox->hide();
    m_preserveOriginalNameCheckBox->hide();
    m_nameSuffixLabel->hide();
    m_nameSuffixLineEdit->hide();
    m_savePresetBtn->hide();
    m_deletePresetBtn->hide();
    m_buttonWidget->hide();
    m_resetBtn->hide();
}

void SettingsPanel::connectSignals()
{
    // 处理参数信号
    connect(m_scaleFactorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsPanel::onScaleFactorChanged);
    
    // 预设管理信号
    connect(m_loadPresetBtn, &QPushButton::clicked, this, &SettingsPanel::onLoadPresetClicked);
    
    // 操作按钮信号
    connect(m_applyBtn, &QPushButton::clicked, this, &SettingsPanel::onApplyClicked);
    
    // 注释掉被简化的组件的信号连接
    /*
    connect(m_denoiseStrengthSlider, &QSlider::valueChanged, [this](int value) {
        double strength = value / 100.0;
        m_denoiseStrengthSpinBox->setValue(strength);
        onDenoiseStrengthChanged(strength);
    });
    
    connect(m_denoiseStrengthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) {
        m_denoiseStrengthSlider->setValue(int(value * 100));
        onDenoiseStrengthChanged(value);
    });
    
    connect(m_sharpenStrengthSlider, &QSlider::valueChanged, [this](int value) {
        double strength = value / 100.0;
        m_sharpenStrengthSpinBox->setValue(strength);
        onSharpenStrengthChanged(strength);
    });
    
    connect(m_sharpenStrengthSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) {
        m_sharpenStrengthSlider->setValue(int(value * 100));
        onSharpenStrengthChanged(value);
    });
    
    // JPEG质量信号
    connect(m_jpegQualitySlider, &QSlider::valueChanged, [this](int value) {
        m_jpegQualitySpinBox->setValue(value);
        updateSliderLabels();
    });
    
    connect(m_jpegQualitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
        m_jpegQualitySlider->setValue(value);
        updateSliderLabels();
    });
    
    // 输出设置信号
    connect(m_browseOutputPathBtn, &QPushButton::clicked, this, &SettingsPanel::onOutputPathChanged);
    connect(m_outputFormatComboBox, &QComboBox::currentTextChanged, this, &SettingsPanel::onOutputFormatChanged);
    
    connect(m_savePresetBtn, &QPushButton::clicked, this, &SettingsPanel::onSavePresetClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &SettingsPanel::onResetClicked);
    */
}

ProcessingSettings SettingsPanel::getSettings() const
{
    ProcessingSettings settings;
    
    settings.scaleFactor = (m_scaleFactorComboBox->currentIndex() == 0) ? 2 : 4;
    settings.enableDenoising = m_enableDenoisingCheckBox->isChecked();
    settings.enableGPU = m_enableGPUCheckBox->isChecked();
    
    // 使用默认值代替被隐藏的组件
    settings.denoiseStrength = 0.3;
    settings.sharpenStrength = 0.2;
    settings.batchSize = 1;
    settings.outputPath = "";
    settings.outputFormat = "png";
    settings.jpegQuality = 95;
    settings.preserveOriginalName = true;
    settings.nameSuffix = "_enhanced";
    
    return settings;
}

void SettingsPanel::setSettings(const ProcessingSettings& settings)
{
    m_currentSettings = settings;
    
    m_scaleFactorComboBox->setCurrentIndex(settings.scaleFactor == 2 ? 0 : 1);
    m_enableDenoisingCheckBox->setChecked(settings.enableDenoising);
    m_enableGPUCheckBox->setChecked(settings.enableGPU);
    
    // 注释掉被隐藏组件的设置
    /*
    m_denoiseStrengthSpinBox->setValue(settings.denoiseStrength);
    m_sharpenStrengthSpinBox->setValue(settings.sharpenStrength);
    m_batchSizeSpinBox->setValue(settings.batchSize);
    m_outputPathLineEdit->setText(settings.outputPath);
    m_outputFormatComboBox->setCurrentText(settings.outputFormat);
    m_jpegQualitySpinBox->setValue(settings.jpegQuality);
    m_preserveOriginalNameCheckBox->setChecked(settings.preserveOriginalName);
    m_nameSuffixLineEdit->setText(settings.nameSuffix);
    */
    
    updateUI();
}

void SettingsPanel::resetToDefaults()
{
    ProcessingSettings defaultSettings;
    setSettings(defaultSettings);
    emit settingsChanged(defaultSettings);
}

void SettingsPanel::updateUI()
{
    // 简化版的UI更新，只更新可见组件
    // updateSliderLabels();  // 注释掉，因为组件已被隐藏
    
    // 注释掉被隐藏组件的UI更新
    /*
    // 根据降噪开关状态启用/禁用相关控件
    bool denoisingEnabled = m_enableDenoisingCheckBox->isChecked();
    m_denoiseStrengthLabel->setEnabled(denoisingEnabled);
    m_denoiseStrengthSlider->setEnabled(denoisingEnabled);
    m_denoiseStrengthSpinBox->setEnabled(denoisingEnabled);
    
    // 根据输出格式启用/禁用JPEG质量设置
    bool isJpegFormat = m_outputFormatComboBox->currentText().contains("jp", Qt::CaseInsensitive);
    m_jpegQualityLabel->setEnabled(isJpegFormat);
    m_jpegQualitySlider->setEnabled(isJpegFormat);
    m_jpegQualitySpinBox->setEnabled(isJpegFormat);
    */
}

void SettingsPanel::updateSliderLabels()
{
    // 简化版，不更新隐藏的标签
    /*
    m_denoiseStrengthLabel->setText(QString("降噪强度：%1").arg(m_denoiseStrengthSpinBox->value(), 0, 'f', 2));
    m_sharpenStrengthLabel->setText(QString("锐化强度：%1").arg(m_sharpenStrengthSpinBox->value(), 0, 'f', 2));
    m_jpegQualityLabel->setText(QString("JPEG质量：%1").arg(m_jpegQualitySpinBox->value()));
    */
}

void SettingsPanel::onScaleFactorChanged(int factor)
{
    updateUI();
    emit settingsChanged(getSettings());
}

void SettingsPanel::onDenoiseStrengthChanged(double strength)
{
    updateSliderLabels();
    emit settingsChanged(getSettings());
}

void SettingsPanel::onSharpenStrengthChanged(double strength)
{
    updateSliderLabels();
    emit settingsChanged(getSettings());
}

void SettingsPanel::onOutputPathChanged()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择输出目录", 
                                                   QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if (!dir.isEmpty()) {
        m_outputPathLineEdit->setText(dir);
        emit settingsChanged(getSettings());
    }
}

void SettingsPanel::onOutputFormatChanged(const QString& format)
{
    updateUI();
    emit settingsChanged(getSettings());
}

void SettingsPanel::onApplyClicked()
{
    validateSettings();
    emit settingsChanged(getSettings());
    QMessageBox::information(this, "设置已应用", "当前设置已成功应用");
}

void SettingsPanel::onResetClicked()
{
    int ret = QMessageBox::question(this, "确认重置", 
                                   "确定要重置所有设置为默认值吗？",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        resetToDefaults();
    }
}

void SettingsPanel::onSavePresetClicked()
{
    bool ok;
    QString presetName = QInputDialog::getText(this, "保存预设", "预设名称：", 
                                              QLineEdit::Normal, "", &ok);
    if (ok && !presetName.isEmpty()) {
        savePreset(presetName);
    }
}

void SettingsPanel::onLoadPresetClicked()
{
    QString presetName = m_presetComboBox->currentText();
    if (!presetName.isEmpty()) {
        loadPreset(presetName);
    }
}

void SettingsPanel::validateSettings()
{
    // 验证输出路径
    QString outputPath = m_outputPathLineEdit->text();
    if (!outputPath.isEmpty() && !QDir(outputPath).exists()) {
        QMessageBox::warning(this, "路径不存在", 
                            "指定的输出路径不存在，将使用原文件目录。");
        m_outputPathLineEdit->clear();
    }
    
    // 验证文件名后缀
    QString suffix = m_nameSuffixLineEdit->text();
    if (suffix.isEmpty()) {
        m_nameSuffixLineEdit->setText("_enhanced");
    }
}

void SettingsPanel::loadPreset(const QString& presetName)
{
    ProcessingSettings settings;
    
    if (presetName == "快速处理") {
        settings.scaleFactor = 2;
        settings.enableDenoising = false;
        settings.denoiseStrength = 0.1;
        settings.sharpenStrength = 0.1;
        settings.enableGPU = true;
        settings.batchSize = 4;
    } else if (presetName == "高质量处理") {
        settings.scaleFactor = 4;
        settings.enableDenoising = true;
        settings.denoiseStrength = 0.4;
        settings.sharpenStrength = 0.3;
        settings.enableGPU = true;
        settings.batchSize = 1;
    } else if (presetName == "降噪优先") {
        settings.scaleFactor = 2;
        settings.enableDenoising = true;
        settings.denoiseStrength = 0.6;
        settings.sharpenStrength = 0.1;
        settings.enableGPU = true;
        settings.batchSize = 2;
    } else if (presetName == "细节增强") {
        settings.scaleFactor = 2;
        settings.enableDenoising = true;
        settings.denoiseStrength = 0.2;
        settings.sharpenStrength = 0.5;
        settings.enableGPU = true;
        settings.batchSize = 2;
    }
    // 默认设置已经在构造函数中设置
    
    setSettings(settings);
    emit settingsChanged(settings);
    emit presetLoaded(presetName);
    
    qDebug() << "加载预设:" << presetName;
}

void SettingsPanel::savePreset(const QString& presetName)
{
    // TODO: 实现预设保存到配置文件
    QSettings settings("VideoSR-Lite", "Presets");
    ProcessingSettings currentSettings = getSettings();
    
    settings.beginGroup(presetName);
    settings.setValue("scaleFactor", currentSettings.scaleFactor);
    settings.setValue("enableDenoising", currentSettings.enableDenoising);
    settings.setValue("denoiseStrength", currentSettings.denoiseStrength);
    settings.setValue("sharpenStrength", currentSettings.sharpenStrength);
    settings.setValue("enableGPU", currentSettings.enableGPU);
    settings.setValue("batchSize", currentSettings.batchSize);
    settings.setValue("outputPath", currentSettings.outputPath);
    settings.setValue("outputFormat", currentSettings.outputFormat);
    settings.setValue("jpegQuality", currentSettings.jpegQuality);
    settings.setValue("preserveOriginalName", currentSettings.preserveOriginalName);
    settings.setValue("nameSuffix", currentSettings.nameSuffix);
    settings.endGroup();
    
    // 添加到下拉列表（如果还没有）
    if (m_presetComboBox->findText(presetName) == -1) {
        m_presetComboBox->addItem(presetName);
    }
    
    emit presetSaved(presetName);
    QMessageBox::information(this, "预设已保存", QString("预设 '%1' 已成功保存").arg(presetName));
    
    qDebug() << "保存预设:" << presetName;
}

QStringList SettingsPanel::getAvailablePresets() const
{
    QStringList presets;
    for (int i = 0; i < m_presetComboBox->count(); ++i) {
        presets.append(m_presetComboBox->itemText(i));
    }
    return presets;
}
