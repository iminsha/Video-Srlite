#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QFileInfo>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>

// 文件项数据结构
struct FileItemData {
    QString filePath;
    QString fileName;
    QFileInfo fileInfo;
    qint64 fileSize;
    QString sizeText;
    bool isProcessed;
    
    FileItemData() : fileSize(0), isProcessed(false) {}
    FileItemData(const QString& path) : filePath(path), isProcessed(false) {
        fileInfo = QFileInfo(path);
        fileName = fileInfo.fileName();
        fileSize = fileInfo.size();
        sizeText = formatFileSize(fileSize);
    }
    
    static QString formatFileSize(qint64 bytes) {
        if (bytes < 1024) return QString("%1 B").arg(bytes);
        if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
        if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 1);
    }
};

// 自定义文件列表项
class FileListItem : public QListWidgetItem
{
public:
    explicit FileListItem(const FileItemData& data, QListWidget *parent = nullptr);
    
    FileItemData getFileData() const { return m_fileData; }
    void setFileData(const FileItemData& data);
    void setProcessed(bool processed);
    bool isProcessed() const { return m_fileData.isProcessed; }
    
private:
    void updateDisplay();
    
private:
    FileItemData m_fileData;
};

// 拖拽支持的列表控件
class DragDropListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit DragDropListWidget(QWidget *parent = nullptr);

signals:
    void filesDropped(const QStringList& filePaths);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    bool isValidImageFile(const QString& filePath);
    QStringList extractImageFiles(const QMimeData* mimeData);
};

class FileListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget *parent = nullptr);
    ~FileListWidget();

    // 文件管理
    void addFile(const QString& filePath);
    void addFiles(const QStringList& filePaths);
    void removeCurrentFile();
    void removeAllFiles();
    QString getCurrentFilePath() const;
    QStringList getAllFilePaths() const;
    int getFileCount() const;
    
    // 处理状态
    void markFileAsProcessed(const QString& filePath);
    void resetAllProcessedStatus();

public slots:
    void onAddFilesClicked();
    void onRemoveFileClicked();
    void onClearAllClicked();
    void onFileSelectionChanged();
    void onFilesDropped(const QStringList& filePaths);

signals:
    void fileSelected(const QString& filePath);
    void fileCountChanged(int count);
    void processingProgress(int processed, int total);

private:
    void setupUI();
    void connectSignals();
    void updateFileCount();
    void updateButtons();
    QStringList getSupportedImageFormats();

private:
    // UI组件
    QGroupBox* m_fileGroupBox;
    QWidget* m_toolbarWidget;
    QPushButton* m_addFilesBtn;
    QPushButton* m_removeFileBtn;
    QPushButton* m_clearAllBtn;
    
    DragDropListWidget* m_fileListWidget;
    
    QWidget* m_statusWidget;
    QLabel* m_fileCountLabel;
    QLabel* m_processedCountLabel;
    QProgressBar* m_progressBar;
    
    // 数据
    int m_totalFiles;
    int m_processedFiles;
    QStringList m_supportedFormats;
};

#endif // FILELISTWIDGET_H 