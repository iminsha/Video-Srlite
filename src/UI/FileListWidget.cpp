#include "FileListWidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QMimeDatabase>
#include <QApplication>
#include <QDebug>
#include <QDateTime>

// FileListItem 实现
FileListItem::FileListItem(const FileItemData& data, QListWidget *parent)
    : QListWidgetItem(parent)
    , m_fileData(data)
{
    updateDisplay();
}

void FileListItem::setFileData(const FileItemData& data)
{
    m_fileData = data;
    updateDisplay();
}

void FileListItem::setProcessed(bool processed)
{
    m_fileData.isProcessed = processed;
    updateDisplay();
}

void FileListItem::updateDisplay()
{
    QString displayText;
    
    if (m_fileData.isProcessed) {
        displayText = "✅ " + m_fileData.fileName;
    } else {
        displayText = "📄 " + m_fileData.fileName;
    }
    
    displayText += QString(" (%1)").arg(m_fileData.sizeText);
    
    setText(displayText);
    
    // 设置工具提示
    QString tooltip = QString("文件路径: %1\n文件大小: %2\n修改时间: %3")
                         .arg(m_fileData.filePath)
                         .arg(m_fileData.sizeText)
                         .arg(m_fileData.fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    
    if (m_fileData.isProcessed) {
        tooltip += "\n状态: 已处理";
    } else {
        tooltip += "\n状态: 待处理";
    }
    
    setToolTip(tooltip);
    
    // 设置图标颜色
    if (m_fileData.isProcessed) {
        setForeground(QColor("#4caf50")); // 绿色
    } else {
        setForeground(QColor("#666666")); // 灰色
    }
}

// DragDropListWidget 实现
DragDropListWidget::DragDropListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DropOnly);
    setDefaultDropAction(Qt::CopyAction);
}

void DragDropListWidget::dragEnterEvent(QDragEnterEvent *event)
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

void DragDropListWidget::dragMoveEvent(QDragMoveEvent *event)
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

void DragDropListWidget::dropEvent(QDropEvent *event)
{
    QStringList imageFiles = extractImageFiles(event->mimeData());
    if (!imageFiles.isEmpty()) {
        emit filesDropped(imageFiles);
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

bool DragDropListWidget::isValidImageFile(const QString& filePath)
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

QStringList DragDropListWidget::extractImageFiles(const QMimeData* mimeData)
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

// FileListWidget 实现
FileListWidget::FileListWidget(QWidget *parent)
    : QWidget(parent)
    , m_totalFiles(0)
    , m_processedFiles(0)
{
    m_supportedFormats = getSupportedImageFormats();
    setupUI();
    connectSignals();
}

FileListWidget::~FileListWidget()
{
}

void FileListWidget::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    
    // 文件列表分组框
    m_fileGroupBox = new QGroupBox("📁 图像文件列表", this);
    mainLayout->addWidget(m_fileGroupBox);
    
    QVBoxLayout* fileLayout = new QVBoxLayout(m_fileGroupBox);
    fileLayout->setSpacing(12);
    
    // 工具栏
    m_toolbarWidget = new QWidget(this);
    m_toolbarWidget->setObjectName("fileToolbar");
    fileLayout->addWidget(m_toolbarWidget);
    
    QHBoxLayout* toolbarLayout = new QHBoxLayout(m_toolbarWidget);
    toolbarLayout->setSpacing(12);
    
    m_addFilesBtn = new QPushButton("➕ 添加文件", this);
    m_addFilesBtn->setToolTip("添加图像文件到处理队列\n支持格式: JPG, PNG, BMP, TIFF, WebP");
    toolbarLayout->addWidget(m_addFilesBtn);
    
    m_removeFileBtn = new QPushButton("➖ 移除选中", this);
    m_removeFileBtn->setToolTip("移除当前选中的文件");
    m_removeFileBtn->setEnabled(false);
    toolbarLayout->addWidget(m_removeFileBtn);
    
    m_clearAllBtn = new QPushButton("🗑️ 清空列表", this);
    m_clearAllBtn->setToolTip("清空所有文件");
    m_clearAllBtn->setEnabled(false);
    toolbarLayout->addWidget(m_clearAllBtn);
    
    toolbarLayout->addStretch();
    
    // 文件列表
    m_fileListWidget = new DragDropListWidget(this);
    m_fileListWidget->setMinimumHeight(300);
    m_fileListWidget->setAlternatingRowColors(true);
    m_fileListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    fileLayout->addWidget(m_fileListWidget);
    
    // 状态栏
    m_statusWidget = new QWidget(this);
    m_statusWidget->setMaximumHeight(50);
    fileLayout->addWidget(m_statusWidget);
    
    QHBoxLayout* statusLayout = new QHBoxLayout(m_statusWidget);
    statusLayout->setSpacing(20);
    
    m_fileCountLabel = new QLabel("文件数量: 0", this);
    m_fileCountLabel->setStyleSheet("color: #666666; font-size: 12px;");
    statusLayout->addWidget(m_fileCountLabel);
    
    m_processedCountLabel = new QLabel("已处理: 0", this);
    m_processedCountLabel->setStyleSheet("color: #666666; font-size: 12px;");
    statusLayout->addWidget(m_processedCountLabel);
    
    statusLayout->addStretch();
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMinimumWidth(200);
    m_progressBar->setVisible(false);
    statusLayout->addWidget(m_progressBar);
}

void FileListWidget::connectSignals()
{
    connect(m_addFilesBtn, &QPushButton::clicked, this, &FileListWidget::onAddFilesClicked);
    connect(m_removeFileBtn, &QPushButton::clicked, this, &FileListWidget::onRemoveFileClicked);
    connect(m_clearAllBtn, &QPushButton::clicked, this, &FileListWidget::onClearAllClicked);
    
    connect(m_fileListWidget, &QListWidget::itemSelectionChanged, this, &FileListWidget::onFileSelectionChanged);
    connect(m_fileListWidget, &DragDropListWidget::filesDropped, this, &FileListWidget::onFilesDropped);
}

void FileListWidget::addFile(const QString& filePath)
{
    // 检查是否已存在
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        FileListItem* item = dynamic_cast<FileListItem*>(m_fileListWidget->item(i));
        if (item && item->getFileData().filePath == filePath) {
            qDebug() << "文件已存在:" << filePath;
            return;
        }
    }
    
    // 创建文件项
    FileItemData fileData(filePath);
    if (!fileData.fileInfo.exists()) {
        QMessageBox::warning(this, "警告", "文件不存在：" + filePath);
        return;
    }
    
    FileListItem* item = new FileListItem(fileData, m_fileListWidget);
    m_fileListWidget->addItem(item);
    
    m_totalFiles++;
    updateFileCount();
    updateButtons();
    
    qDebug() << "添加文件:" << filePath;
}

void FileListWidget::addFiles(const QStringList& filePaths)
{
    for (const QString& filePath : filePaths) {
        addFile(filePath);
    }
}

void FileListWidget::removeCurrentFile()
{
    QListWidgetItem* currentItem = m_fileListWidget->currentItem();
    if (!currentItem) {
        return;
    }
    
    FileListItem* fileItem = dynamic_cast<FileListItem*>(currentItem);
    if (fileItem && fileItem->isProcessed()) {
        m_processedFiles--;
    }
    
    delete currentItem;
    m_totalFiles--;
    
    updateFileCount();
    updateButtons();
    
    qDebug() << "移除当前文件";
}

void FileListWidget::removeAllFiles()
{
    if (m_fileListWidget->count() == 0) {
        return;
    }
    
    int ret = QMessageBox::question(this, "确认清空", 
                                   "确定要清空所有文件吗？\n这将移除列表中的所有文件。",
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_fileListWidget->clear();
        m_totalFiles = 0;
        m_processedFiles = 0;
        
        updateFileCount();
        updateButtons();
        
        qDebug() << "清空所有文件";
    }
}

QString FileListWidget::getCurrentFilePath() const
{
    QListWidgetItem* currentItem = m_fileListWidget->currentItem();
    if (!currentItem) {
        return QString();
    }
    
    FileListItem* fileItem = dynamic_cast<FileListItem*>(currentItem);
    if (fileItem) {
        return fileItem->getFileData().filePath;
    }
    
    return QString();
}

QStringList FileListWidget::getAllFilePaths() const
{
    QStringList filePaths;
    
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        FileListItem* item = dynamic_cast<FileListItem*>(m_fileListWidget->item(i));
        if (item) {
            filePaths.append(item->getFileData().filePath);
        }
    }
    
    return filePaths;
}

int FileListWidget::getFileCount() const
{
    return m_totalFiles;
}

void FileListWidget::markFileAsProcessed(const QString& filePath)
{
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        FileListItem* item = dynamic_cast<FileListItem*>(m_fileListWidget->item(i));
        if (item && item->getFileData().filePath == filePath) {
            if (!item->isProcessed()) {
                item->setProcessed(true);
                m_processedFiles++;
                updateFileCount();
                emit processingProgress(m_processedFiles, m_totalFiles);
            }
            break;
        }
    }
}

void FileListWidget::resetAllProcessedStatus()
{
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        FileListItem* item = dynamic_cast<FileListItem*>(m_fileListWidget->item(i));
        if (item) {
            item->setProcessed(false);
        }
    }
    
    m_processedFiles = 0;
    updateFileCount();
    emit processingProgress(0, m_totalFiles);
}

void FileListWidget::onAddFilesClicked()
{
    QString lastDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    
    QString filter = "图像文件 (";
    for (const QString& format : m_supportedFormats) {
        filter += "*." + format + " ";
    }
    filter = filter.trimmed() + ")";
    
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "选择图像文件", lastDir, filter);
    
    if (!filePaths.isEmpty()) {
        addFiles(filePaths);
    }
}

void FileListWidget::onRemoveFileClicked()
{
    removeCurrentFile();
}

void FileListWidget::onClearAllClicked()
{
    removeAllFiles();
}

void FileListWidget::onFileSelectionChanged()
{
    QListWidgetItem* currentItem = m_fileListWidget->currentItem();
    bool hasSelection = (currentItem != nullptr);
    
    m_removeFileBtn->setEnabled(hasSelection);
    
    if (hasSelection) {
        FileListItem* fileItem = dynamic_cast<FileListItem*>(currentItem);
        if (fileItem) {
            emit fileSelected(fileItem->getFileData().filePath);
        }
    }
}

void FileListWidget::onFilesDropped(const QStringList& filePaths)
{
    if (!filePaths.isEmpty()) {
        addFiles(filePaths);
        QMessageBox::information(this, "文件添加", QString("成功添加 %1 个文件").arg(filePaths.size()));
    }
}

void FileListWidget::updateFileCount()
{
    m_fileCountLabel->setText(QString("文件数量: %1").arg(m_totalFiles));
    m_processedCountLabel->setText(QString("已处理: %1").arg(m_processedFiles));
    
    // 更新进度条
    if (m_totalFiles > 0) {
        m_progressBar->setVisible(true);
        m_progressBar->setMaximum(m_totalFiles);
        m_progressBar->setValue(m_processedFiles);
        
        double percentage = (double)m_processedFiles / m_totalFiles * 100.0;
        m_progressBar->setFormat(QString("进度: %1%").arg(percentage, 0, 'f', 1));
    } else {
        m_progressBar->setVisible(false);
    }
    
    emit fileCountChanged(m_totalFiles);
}

void FileListWidget::updateButtons()
{
    bool hasFiles = (m_totalFiles > 0);
    m_clearAllBtn->setEnabled(hasFiles);
}

QStringList FileListWidget::getSupportedImageFormats()
{
    return {"jpg", "jpeg", "png", "bmp", "tiff", "tif", "webp"};
}

#include "FileListWidget.moc" 