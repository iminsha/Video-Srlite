/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAddFiles;
    QAction *actionClearList;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionFitWindow;
    QAction *actionActualSize;
    QAction *actionSideBySide;
    QAction *actionSliderCompare;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *leftPanel;
    QVBoxLayout *leftLayout;
    QGroupBox *fileGroupBox;
    QVBoxLayout *fileLayout;
    QHBoxLayout *fileButtonsLayout;
    QPushButton *addFileBtn;
    QPushButton *removeFileBtn;
    QListWidget *fileListWidget;
    QGroupBox *settingsGroupBox;
    QVBoxLayout *settingsLayout;
    QLabel *resolutionLabel;
    QRadioButton *x2RadioBtn;
    QRadioButton *x4RadioBtn;
    QCheckBox *audioDenoiseCheckBox;
    QLabel *denoiseStrengthLabel;
    QHBoxLayout *denoiseLayout;
    QSlider *denoiseSlider;
    QLabel *denoiseValueLabel;
    QLabel *sharpenLabel;
    QHBoxLayout *sharpenLayout;
    QSlider *sharpenSlider;
    QLabel *sharpenValueLabel;
    QGroupBox *controlGroupBox;
    QVBoxLayout *controlLayout;
    QPushButton *startBtn;
    QPushButton *pauseBtn;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    QSpacerItem *verticalSpacer;
    QWidget *rightPanel;
    QVBoxLayout *rightLayout;
    QGroupBox *previewGroupBox;
    QVBoxLayout *previewLayout;
    QWidget *previewToolbar;
    QHBoxLayout *previewToolbarLayout;
    QPushButton *loadPreviewBtn;
    QLabel *zoomLabel;
    QComboBox *zoomComboBox;
    QLabel *compareLabel;
    QComboBox *compareModeComboBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *savePreviewBtn;
    QStackedWidget *previewStackedWidget;
    QWidget *sideBySidePage;
    QHBoxLayout *sideBySideLayout;
    QWidget *originalWidget;
    QVBoxLayout *originalLayout;
    QLabel *originalTitleLabel;
    QScrollArea *originalScrollArea;
    QWidget *originalScrollAreaWidgetContents;
    QVBoxLayout *originalContentLayout;
    QLabel *originalImageLabel;
    QWidget *enhancedWidget;
    QVBoxLayout *enhancedLayout;
    QLabel *enhancedTitleLabel;
    QScrollArea *enhancedScrollArea;
    QWidget *enhancedScrollAreaWidgetContents;
    QVBoxLayout *enhancedContentLayout;
    QLabel *enhancedImageLabel;
    QWidget *sliderComparePage;
    QVBoxLayout *sliderCompareLayout;
    QLabel *sliderCompareLabel;
    QSlider *compareSlider;
    QWidget *imageInfoWidget;
    QHBoxLayout *imageInfoLayout;
    QLabel *originalInfoLabel;
    QLabel *enhancedInfoLabel;
    QLabel *processingTimeLabel;
    QSpacerItem *infoSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1800, 1000);
        actionAddFiles = new QAction(MainWindow);
        actionAddFiles->setObjectName(QString::fromUtf8("actionAddFiles"));
        actionClearList = new QAction(MainWindow);
        actionClearList->setObjectName(QString::fromUtf8("actionClearList"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionFitWindow = new QAction(MainWindow);
        actionFitWindow->setObjectName(QString::fromUtf8("actionFitWindow"));
        actionActualSize = new QAction(MainWindow);
        actionActualSize->setObjectName(QString::fromUtf8("actionActualSize"));
        actionSideBySide = new QAction(MainWindow);
        actionSideBySide->setObjectName(QString::fromUtf8("actionSideBySide"));
        actionSliderCompare = new QAction(MainWindow);
        actionSliderCompare->setObjectName(QString::fromUtf8("actionSliderCompare"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        leftPanel = new QWidget(centralwidget);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftPanel->setMinimumSize(QSize(360, 0));
        leftPanel->setMaximumSize(QSize(380, 16777215));
        leftLayout = new QVBoxLayout(leftPanel);
        leftLayout->setSpacing(12);
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));
        leftLayout->setContentsMargins(12, 12, 12, 12);
        fileGroupBox = new QGroupBox(leftPanel);
        fileGroupBox->setObjectName(QString::fromUtf8("fileGroupBox"));
        fileLayout = new QVBoxLayout(fileGroupBox);
        fileLayout->setSpacing(8);
        fileLayout->setObjectName(QString::fromUtf8("fileLayout"));
        fileButtonsLayout = new QHBoxLayout();
        fileButtonsLayout->setSpacing(8);
        fileButtonsLayout->setObjectName(QString::fromUtf8("fileButtonsLayout"));
        addFileBtn = new QPushButton(fileGroupBox);
        addFileBtn->setObjectName(QString::fromUtf8("addFileBtn"));

        fileButtonsLayout->addWidget(addFileBtn);

        removeFileBtn = new QPushButton(fileGroupBox);
        removeFileBtn->setObjectName(QString::fromUtf8("removeFileBtn"));

        fileButtonsLayout->addWidget(removeFileBtn);


        fileLayout->addLayout(fileButtonsLayout);

        fileListWidget = new QListWidget(fileGroupBox);
        fileListWidget->setObjectName(QString::fromUtf8("fileListWidget"));
        fileListWidget->setMinimumSize(QSize(0, 220));
        fileListWidget->setAlternatingRowColors(true);

        fileLayout->addWidget(fileListWidget);


        leftLayout->addWidget(fileGroupBox);

        settingsGroupBox = new QGroupBox(leftPanel);
        settingsGroupBox->setObjectName(QString::fromUtf8("settingsGroupBox"));
        settingsLayout = new QVBoxLayout(settingsGroupBox);
        settingsLayout->setSpacing(8);
        settingsLayout->setObjectName(QString::fromUtf8("settingsLayout"));
        resolutionLabel = new QLabel(settingsGroupBox);
        resolutionLabel->setObjectName(QString::fromUtf8("resolutionLabel"));

        settingsLayout->addWidget(resolutionLabel);

        x2RadioBtn = new QRadioButton(settingsGroupBox);
        x2RadioBtn->setObjectName(QString::fromUtf8("x2RadioBtn"));
        x2RadioBtn->setChecked(true);

        settingsLayout->addWidget(x2RadioBtn);

        x4RadioBtn = new QRadioButton(settingsGroupBox);
        x4RadioBtn->setObjectName(QString::fromUtf8("x4RadioBtn"));

        settingsLayout->addWidget(x4RadioBtn);

        audioDenoiseCheckBox = new QCheckBox(settingsGroupBox);
        audioDenoiseCheckBox->setObjectName(QString::fromUtf8("audioDenoiseCheckBox"));
        audioDenoiseCheckBox->setChecked(true);

        settingsLayout->addWidget(audioDenoiseCheckBox);

        denoiseStrengthLabel = new QLabel(settingsGroupBox);
        denoiseStrengthLabel->setObjectName(QString::fromUtf8("denoiseStrengthLabel"));

        settingsLayout->addWidget(denoiseStrengthLabel);

        denoiseLayout = new QHBoxLayout();
        denoiseLayout->setSpacing(8);
        denoiseLayout->setObjectName(QString::fromUtf8("denoiseLayout"));
        denoiseSlider = new QSlider(settingsGroupBox);
        denoiseSlider->setObjectName(QString::fromUtf8("denoiseSlider"));
        denoiseSlider->setMinimum(0);
        denoiseSlider->setMaximum(100);
        denoiseSlider->setValue(20);
        denoiseSlider->setOrientation(Qt::Horizontal);

        denoiseLayout->addWidget(denoiseSlider);

        denoiseValueLabel = new QLabel(settingsGroupBox);
        denoiseValueLabel->setObjectName(QString::fromUtf8("denoiseValueLabel"));
        denoiseValueLabel->setMinimumSize(QSize(40, 0));

        denoiseLayout->addWidget(denoiseValueLabel);


        settingsLayout->addLayout(denoiseLayout);

        sharpenLabel = new QLabel(settingsGroupBox);
        sharpenLabel->setObjectName(QString::fromUtf8("sharpenLabel"));

        settingsLayout->addWidget(sharpenLabel);

        sharpenLayout = new QHBoxLayout();
        sharpenLayout->setSpacing(8);
        sharpenLayout->setObjectName(QString::fromUtf8("sharpenLayout"));
        sharpenSlider = new QSlider(settingsGroupBox);
        sharpenSlider->setObjectName(QString::fromUtf8("sharpenSlider"));
        sharpenSlider->setMinimum(0);
        sharpenSlider->setMaximum(100);
        sharpenSlider->setValue(50);
        sharpenSlider->setOrientation(Qt::Horizontal);

        sharpenLayout->addWidget(sharpenSlider);

        sharpenValueLabel = new QLabel(settingsGroupBox);
        sharpenValueLabel->setObjectName(QString::fromUtf8("sharpenValueLabel"));
        sharpenValueLabel->setMinimumSize(QSize(40, 0));

        sharpenLayout->addWidget(sharpenValueLabel);


        settingsLayout->addLayout(sharpenLayout);


        leftLayout->addWidget(settingsGroupBox);

        controlGroupBox = new QGroupBox(leftPanel);
        controlGroupBox->setObjectName(QString::fromUtf8("controlGroupBox"));
        controlLayout = new QVBoxLayout(controlGroupBox);
        controlLayout->setSpacing(8);
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));
        startBtn = new QPushButton(controlGroupBox);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setMinimumSize(QSize(0, 45));

        controlLayout->addWidget(startBtn);

        pauseBtn = new QPushButton(controlGroupBox);
        pauseBtn->setObjectName(QString::fromUtf8("pauseBtn"));
        pauseBtn->setEnabled(false);

        controlLayout->addWidget(pauseBtn);

        progressBar = new QProgressBar(controlGroupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        controlLayout->addWidget(progressBar);

        statusLabel = new QLabel(controlGroupBox);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setAlignment(Qt::AlignCenter);

        controlLayout->addWidget(statusLabel);


        leftLayout->addWidget(controlGroupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        leftLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(leftPanel);

        rightPanel = new QWidget(centralwidget);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setSpacing(12);
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        rightLayout->setContentsMargins(12, 12, 12, 12);
        previewGroupBox = new QGroupBox(rightPanel);
        previewGroupBox->setObjectName(QString::fromUtf8("previewGroupBox"));
        previewLayout = new QVBoxLayout(previewGroupBox);
        previewLayout->setSpacing(12);
        previewLayout->setObjectName(QString::fromUtf8("previewLayout"));
        previewToolbar = new QWidget(previewGroupBox);
        previewToolbar->setObjectName(QString::fromUtf8("previewToolbar"));
        previewToolbarLayout = new QHBoxLayout(previewToolbar);
        previewToolbarLayout->setSpacing(12);
        previewToolbarLayout->setObjectName(QString::fromUtf8("previewToolbarLayout"));
        previewToolbarLayout->setContentsMargins(0, 0, 0, 0);
        loadPreviewBtn = new QPushButton(previewToolbar);
        loadPreviewBtn->setObjectName(QString::fromUtf8("loadPreviewBtn"));

        previewToolbarLayout->addWidget(loadPreviewBtn);

        zoomLabel = new QLabel(previewToolbar);
        zoomLabel->setObjectName(QString::fromUtf8("zoomLabel"));

        previewToolbarLayout->addWidget(zoomLabel);

        zoomComboBox = new QComboBox(previewToolbar);
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->addItem(QString());
        zoomComboBox->setObjectName(QString::fromUtf8("zoomComboBox"));

        previewToolbarLayout->addWidget(zoomComboBox);

        compareLabel = new QLabel(previewToolbar);
        compareLabel->setObjectName(QString::fromUtf8("compareLabel"));

        previewToolbarLayout->addWidget(compareLabel);

        compareModeComboBox = new QComboBox(previewToolbar);
        compareModeComboBox->addItem(QString());
        compareModeComboBox->addItem(QString());
        compareModeComboBox->addItem(QString());
        compareModeComboBox->addItem(QString());
        compareModeComboBox->setObjectName(QString::fromUtf8("compareModeComboBox"));

        previewToolbarLayout->addWidget(compareModeComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        previewToolbarLayout->addItem(horizontalSpacer);

        savePreviewBtn = new QPushButton(previewToolbar);
        savePreviewBtn->setObjectName(QString::fromUtf8("savePreviewBtn"));

        previewToolbarLayout->addWidget(savePreviewBtn);


        previewLayout->addWidget(previewToolbar);

        previewStackedWidget = new QStackedWidget(previewGroupBox);
        previewStackedWidget->setObjectName(QString::fromUtf8("previewStackedWidget"));
        sideBySidePage = new QWidget();
        sideBySidePage->setObjectName(QString::fromUtf8("sideBySidePage"));
        sideBySideLayout = new QHBoxLayout(sideBySidePage);
        sideBySideLayout->setSpacing(12);
        sideBySideLayout->setObjectName(QString::fromUtf8("sideBySideLayout"));
        originalWidget = new QWidget(sideBySidePage);
        originalWidget->setObjectName(QString::fromUtf8("originalWidget"));
        originalLayout = new QVBoxLayout(originalWidget);
        originalLayout->setSpacing(8);
        originalLayout->setObjectName(QString::fromUtf8("originalLayout"));
        originalLayout->setContentsMargins(0, 0, 0, 0);
        originalTitleLabel = new QLabel(originalWidget);
        originalTitleLabel->setObjectName(QString::fromUtf8("originalTitleLabel"));
        originalTitleLabel->setAlignment(Qt::AlignCenter);

        originalLayout->addWidget(originalTitleLabel);

        originalScrollArea = new QScrollArea(originalWidget);
        originalScrollArea->setObjectName(QString::fromUtf8("originalScrollArea"));
        originalScrollArea->setMinimumSize(QSize(680, 680));
        originalScrollArea->setWidgetResizable(true);
        originalScrollAreaWidgetContents = new QWidget();
        originalScrollAreaWidgetContents->setObjectName(QString::fromUtf8("originalScrollAreaWidgetContents"));
        originalScrollAreaWidgetContents->setGeometry(QRect(0, 0, 678, 678));
        originalContentLayout = new QVBoxLayout(originalScrollAreaWidgetContents);
        originalContentLayout->setObjectName(QString::fromUtf8("originalContentLayout"));
        originalImageLabel = new QLabel(originalScrollAreaWidgetContents);
        originalImageLabel->setObjectName(QString::fromUtf8("originalImageLabel"));
        originalImageLabel->setMinimumSize(QSize(660, 660));
        originalImageLabel->setAlignment(Qt::AlignCenter);
        originalImageLabel->setScaledContents(false);

        originalContentLayout->addWidget(originalImageLabel);

        originalScrollArea->setWidget(originalScrollAreaWidgetContents);

        originalLayout->addWidget(originalScrollArea);


        sideBySideLayout->addWidget(originalWidget);

        enhancedWidget = new QWidget(sideBySidePage);
        enhancedWidget->setObjectName(QString::fromUtf8("enhancedWidget"));
        enhancedLayout = new QVBoxLayout(enhancedWidget);
        enhancedLayout->setSpacing(8);
        enhancedLayout->setObjectName(QString::fromUtf8("enhancedLayout"));
        enhancedLayout->setContentsMargins(0, 0, 0, 0);
        enhancedTitleLabel = new QLabel(enhancedWidget);
        enhancedTitleLabel->setObjectName(QString::fromUtf8("enhancedTitleLabel"));
        enhancedTitleLabel->setAlignment(Qt::AlignCenter);

        enhancedLayout->addWidget(enhancedTitleLabel);

        enhancedScrollArea = new QScrollArea(enhancedWidget);
        enhancedScrollArea->setObjectName(QString::fromUtf8("enhancedScrollArea"));
        enhancedScrollArea->setMinimumSize(QSize(680, 680));
        enhancedScrollArea->setWidgetResizable(true);
        enhancedScrollAreaWidgetContents = new QWidget();
        enhancedScrollAreaWidgetContents->setObjectName(QString::fromUtf8("enhancedScrollAreaWidgetContents"));
        enhancedScrollAreaWidgetContents->setGeometry(QRect(0, 0, 678, 678));
        enhancedContentLayout = new QVBoxLayout(enhancedScrollAreaWidgetContents);
        enhancedContentLayout->setObjectName(QString::fromUtf8("enhancedContentLayout"));
        enhancedImageLabel = new QLabel(enhancedScrollAreaWidgetContents);
        enhancedImageLabel->setObjectName(QString::fromUtf8("enhancedImageLabel"));
        enhancedImageLabel->setMinimumSize(QSize(660, 660));
        enhancedImageLabel->setAlignment(Qt::AlignCenter);
        enhancedImageLabel->setScaledContents(false);

        enhancedContentLayout->addWidget(enhancedImageLabel);

        enhancedScrollArea->setWidget(enhancedScrollAreaWidgetContents);

        enhancedLayout->addWidget(enhancedScrollArea);


        sideBySideLayout->addWidget(enhancedWidget);

        previewStackedWidget->addWidget(sideBySidePage);
        sliderComparePage = new QWidget();
        sliderComparePage->setObjectName(QString::fromUtf8("sliderComparePage"));
        sliderCompareLayout = new QVBoxLayout(sliderComparePage);
        sliderCompareLayout->setObjectName(QString::fromUtf8("sliderCompareLayout"));
        sliderCompareLabel = new QLabel(sliderComparePage);
        sliderCompareLabel->setObjectName(QString::fromUtf8("sliderCompareLabel"));
        sliderCompareLabel->setMinimumSize(QSize(1360, 680));
        sliderCompareLabel->setAlignment(Qt::AlignCenter);

        sliderCompareLayout->addWidget(sliderCompareLabel);

        compareSlider = new QSlider(sliderComparePage);
        compareSlider->setObjectName(QString::fromUtf8("compareSlider"));
        compareSlider->setOrientation(Qt::Horizontal);
        compareSlider->setMinimum(0);
        compareSlider->setMaximum(100);
        compareSlider->setValue(50);

        sliderCompareLayout->addWidget(compareSlider);

        previewStackedWidget->addWidget(sliderComparePage);

        previewLayout->addWidget(previewStackedWidget);

        imageInfoWidget = new QWidget(previewGroupBox);
        imageInfoWidget->setObjectName(QString::fromUtf8("imageInfoWidget"));
        imageInfoWidget->setMaximumSize(QSize(16777215, 60));
        imageInfoLayout = new QHBoxLayout(imageInfoWidget);
        imageInfoLayout->setSpacing(20);
        imageInfoLayout->setObjectName(QString::fromUtf8("imageInfoLayout"));
        imageInfoLayout->setContentsMargins(0, 0, 0, 0);
        originalInfoLabel = new QLabel(imageInfoWidget);
        originalInfoLabel->setObjectName(QString::fromUtf8("originalInfoLabel"));

        imageInfoLayout->addWidget(originalInfoLabel);

        enhancedInfoLabel = new QLabel(imageInfoWidget);
        enhancedInfoLabel->setObjectName(QString::fromUtf8("enhancedInfoLabel"));

        imageInfoLayout->addWidget(enhancedInfoLabel);

        processingTimeLabel = new QLabel(imageInfoWidget);
        processingTimeLabel->setObjectName(QString::fromUtf8("processingTimeLabel"));

        imageInfoLayout->addWidget(processingTimeLabel);

        infoSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        imageInfoLayout->addItem(infoSpacer);


        previewLayout->addWidget(imageInfoWidget);


        rightLayout->addWidget(previewGroupBox);


        horizontalLayout->addWidget(rightPanel);

        horizontalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1800, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionAddFiles);
        menuFile->addAction(actionClearList);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionFitWindow);
        menuView->addAction(actionActualSize);
        menuView->addSeparator();
        menuView->addAction(actionSideBySide);
        menuView->addAction(actionSliderCompare);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionHelp);

        retranslateUi(MainWindow);

        previewStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VideoSR-Lite - AI\350\247\206\351\242\221&\345\233\276\347\211\207\347\224\273\350\264\250\345\242\236\345\274\272\345\267\245\345\205\267", nullptr));
        MainWindow->setStyleSheet(QApplication::translate("MainWindow", "/* \344\270\273\347\252\227\345\217\243\346\240\267\345\274\217 */\n"
"QMainWindow {\n"
"    background-color: #f0f2f5;\n"
"}\n"
"\n"
"/* \345\267\246\344\276\247\351\235\242\346\235\277\346\240\267\345\274\217 */\n"
"#leftPanel {\n"
"    background-color: #ffffff;\n"
"    border-right: 1px solid #e8eaed;\n"
"}\n"
"\n"
"/* \345\210\206\347\273\204\346\241\206\346\240\267\345\274\217 */\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    color: #333333;\n"
"    border: 2px solid #e8eaed;\n"
"    border-radius: 8px;\n"
"    margin-top: 10px;\n"
"    padding-top: 10px;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 8px 0 8px;\n"
"    color: #1976d2;\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"/* \346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QPushButton {\n"
"    background-color: #1976d2;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 6px;\n"
"    padding: 8px"
                        " 16px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    min-height: 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #1565c0;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #0d47a1;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #cccccc;\n"
"    color: #666666;\n"
"}\n"
"\n"
"/* \344\270\273\350\246\201\346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"#startBtn {\n"
"    background-color: #4caf50;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    min-height: 25px;\n"
"}\n"
"\n"
"#startBtn:hover {\n"
"    background-color: #45a049;\n"
"}\n"
"\n"
"/* \346\254\241\350\246\201\346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"#removeFileBtn, #pauseBtn {\n"
"    background-color: #ff5722;\n"
"}\n"
"\n"
"#removeFileBtn:hover, #pauseBtn:hover {\n"
"    background-color: #e64a19;\n"
"}\n"
"\n"
"/* \346\226\207\344\273\266\345\210\227\350\241\250\346\240\267\345\274\217 */\n"
"QListWidget {\n"
"    border: 1px solid #e0e0e0;\n"
"    borde"
                        "r-radius: 6px;\n"
"    background-color: #fafbfc;\n"
"    alternate-background-color: #f5f5f5;\n"
"    selection-background-color: #e3f2fd;\n"
"}\n"
"\n"
"QListWidget::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #eeeeee;\n"
"}\n"
"\n"
"QListWidget::item:selected {\n"
"    background-color: #e3f2fd;\n"
"    color: #1976d2;\n"
"}\n"
"\n"
"QListWidget::item:hover {\n"
"    background-color: #f5f5f5;\n"
"}\n"
"\n"
"/* \346\273\221\345\235\227\346\240\267\345\274\217 */\n"
"QSlider::groove:horizontal {\n"
"    border: 1px solid #bbb;\n"
"    background: #e0e0e0;\n"
"    height: 6px;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"    background: #1976d2;\n"
"    border: 1px solid #1976d2;\n"
"    width: 16px;\n"
"    margin: -5px 0;\n"
"    border-radius: 8px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"    background: #1565c0;\n"
"}\n"
"\n"
"/* \345\215\225\351\200\211\346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QRadioButton {\n"
"    color: #33333"
                        "3;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"QRadioButton::indicator:unchecked {\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 8px;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QRadioButton::indicator:checked {\n"
"    border: 2px solid #1976d2;\n"
"    border-radius: 8px;\n"
"    background-color: #1976d2;\n"
"}\n"
"\n"
"/* \345\244\215\351\200\211\346\241\206\346\240\267\345\274\217 */\n"
"QCheckBox {\n"
"    color: #333333;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked {\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 3px;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"    border: 2px solid #1976d2;\n"
"    border-radius: 3px;\n"
"    background-color: #1976d2;\n"
"}\n"
"\n"
"/* \347\273\204\345\220\210\346\241\206\346\240\267\345\274\217 */\n"
"QComboBox {\n"
" "
                        "   border: 1px solid #cccccc;\n"
"    border-radius: 4px;\n"
"    padding: 4px 8px;\n"
"    background-color: white;\n"
"    min-width: 80px;\n"
"}\n"
"\n"
"QComboBox:hover {\n"
"    border: 1px solid #1976d2;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 20px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    width: 12px;\n"
"    height: 12px;\n"
"}\n"
"\n"
"/* \351\242\204\350\247\210\345\214\272\345\237\237\346\240\267\345\274\217 */\n"
"#previewGroupBox {\n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"#previewToolbar {\n"
"    background-color: #f8f9fa;\n"
"    border: 1px solid #e9ecef;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"/* \351\242\204\350\247\210\346\240\207\347\255\276\346\240\267\345\274\217 */\n"
"#originalTitleLabel, #enhancedTitleLabel {\n"
"    background-color: #e3f2fd;\n"
"    border: 1px solid #bbdefb;\n"
"    border-radius: 4px;\n"
"    padding: 6px;\n"
"    font-weight: bold;\n"
"    color: #1976d2;\n"
"}\n"
"\n"
"/* \346\273\232\345\212"
                        "\250\345\214\272\345\237\237\346\240\267\345\274\217 */\n"
"QScrollArea {\n"
"    border: 2px solid #e0e0e0;\n"
"    border-radius: 8px;\n"
"    background-color: #fafafa;\n"
"}\n"
"\n"
"/* \351\242\204\350\247\210\345\233\276\347\211\207\346\240\207\347\255\276\346\240\267\345\274\217 */\n"
"#originalImageLabel, #enhancedImageLabel {\n"
"    border: 1px dashed #cccccc;\n"
"    border-radius: 4px;\n"
"    background-color: #ffffff;\n"
"    color: #666666;\n"
"}\n"
"\n"
"/* \350\277\233\345\272\246\346\235\241\346\240\267\345\274\217 */\n"
"QProgressBar {\n"
"    border: 1px solid #cccccc;\n"
"    border-radius: 4px;\n"
"    background-color: #f0f0f0;\n"
"    text-align: center;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #4caf50;\n"
"    border-radius: 3px;\n"
"}\n"
"\n"
"/* \347\212\266\346\200\201\346\240\217\346\240\267\345\274\217 */\n"
"QStatusBar {\n"
"    background-color: #f8f9fa;\n"
"    border-top: 1px solid #e9ecef;\n"
"    color: #495057;\n"
"}", nullptr));
        actionAddFiles->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\346\226\207\344\273\266...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionAddFiles->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionClearList->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\345\210\227\350\241\250", nullptr));
#ifndef QT_NO_SHORTCUT
        actionClearList->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216...", nullptr));
        actionHelp->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\345\270\256\345\212\251", nullptr));
#ifndef QT_NO_SHORTCUT
        actionHelp->setShortcut(QApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        actionFitWindow->setText(QApplication::translate("MainWindow", "\351\200\202\345\220\210\347\252\227\345\217\243", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFitWindow->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", nullptr));
#endif // QT_NO_SHORTCUT
        actionActualSize->setText(QApplication::translate("MainWindow", "\345\256\236\351\231\205\345\244\247\345\260\217", nullptr));
#ifndef QT_NO_SHORTCUT
        actionActualSize->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", nullptr));
#endif // QT_NO_SHORTCUT
        actionSideBySide->setText(QApplication::translate("MainWindow", "\345\271\266\346\216\222\345\257\271\346\257\224", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSideBySide->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", nullptr));
#endif // QT_NO_SHORTCUT
        actionSliderCompare->setText(QApplication::translate("MainWindow", "\346\273\221\345\212\250\345\257\271\346\257\224", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSliderCompare->setShortcut(QApplication::translate("MainWindow", "Ctrl+3", nullptr));
#endif // QT_NO_SHORTCUT
        fileGroupBox->setTitle(QApplication::translate("MainWindow", "\360\237\223\201 \345\252\222\344\275\223\346\226\207\344\273\266\345\210\227\350\241\250", nullptr));
        addFileBtn->setText(QApplication::translate("MainWindow", "\342\236\225 \346\267\273\345\212\240\346\226\207\344\273\266", nullptr));
        removeFileBtn->setText(QApplication::translate("MainWindow", "\360\237\227\221\357\270\217 \347\247\273\351\231\244", nullptr));
        settingsGroupBox->setTitle(QApplication::translate("MainWindow", "\342\232\231\357\270\217 \345\244\204\347\220\206\350\256\276\347\275\256", nullptr));
        resolutionLabel->setText(QApplication::translate("MainWindow", "\360\237\224\215 \350\266\205\345\210\206\350\276\250\347\216\207\345\200\215\346\225\260\357\274\232", nullptr));
        x2RadioBtn->setText(QApplication::translate("MainWindow", "\303\2272 \345\200\215 (\346\216\250\350\215\220\357\274\214\351\200\237\345\272\246\345\277\253)", nullptr));
        x4RadioBtn->setText(QApplication::translate("MainWindow", "\303\2274 \345\200\215 (\346\225\210\346\236\234\344\275\263\357\274\214\351\200\237\345\272\246\346\205\242)", nullptr));
        audioDenoiseCheckBox->setText(QApplication::translate("MainWindow", "\360\237\216\265 \351\237\263\351\242\221\351\231\215\345\231\252", nullptr));
        denoiseStrengthLabel->setText(QApplication::translate("MainWindow", "\360\237\224\212 \351\231\215\345\231\252\345\274\272\345\272\246\357\274\232", nullptr));
        denoiseValueLabel->setText(QApplication::translate("MainWindow", "20%", nullptr));
        sharpenLabel->setText(QApplication::translate("MainWindow", "\342\234\250 \351\224\220\345\214\226\345\274\272\345\272\246\357\274\232", nullptr));
        sharpenValueLabel->setText(QApplication::translate("MainWindow", "50%", nullptr));
        controlGroupBox->setTitle(QApplication::translate("MainWindow", "\360\237\216\254 \345\244\204\347\220\206\346\216\247\345\210\266", nullptr));
        startBtn->setText(QApplication::translate("MainWindow", "\360\237\232\200 \345\274\200\345\247\213\345\244\204\347\220\206", nullptr));
        pauseBtn->setText(QApplication::translate("MainWindow", "\342\217\270\357\270\217 \346\232\202\345\201\234", nullptr));
        statusLabel->setText(QApplication::translate("MainWindow", "\345\260\261\347\273\252", nullptr));
        previewGroupBox->setTitle(QApplication::translate("MainWindow", "\360\237\224\215 \351\242\204\350\247\210\345\257\271\346\257\224 - \351\200\211\346\213\251\346\226\207\344\273\266\345\215\263\345\217\257\350\207\252\345\212\250\351\242\204\350\247\210", nullptr));
        loadPreviewBtn->setText(QApplication::translate("MainWindow", "\360\237\224\204 \345\210\267\346\226\260\351\242\204\350\247\210", nullptr));
        zoomLabel->setText(QApplication::translate("MainWindow", "\360\237\224\215 \347\274\251\346\224\276\357\274\232", nullptr));
        zoomComboBox->setItemText(0, QApplication::translate("MainWindow", "25%", nullptr));
        zoomComboBox->setItemText(1, QApplication::translate("MainWindow", "50%", nullptr));
        zoomComboBox->setItemText(2, QApplication::translate("MainWindow", "75%", nullptr));
        zoomComboBox->setItemText(3, QApplication::translate("MainWindow", "100%", nullptr));
        zoomComboBox->setItemText(4, QApplication::translate("MainWindow", "150%", nullptr));
        zoomComboBox->setItemText(5, QApplication::translate("MainWindow", "200%", nullptr));
        zoomComboBox->setItemText(6, QApplication::translate("MainWindow", "300%", nullptr));
        zoomComboBox->setItemText(7, QApplication::translate("MainWindow", "\351\200\202\345\220\210\347\252\227\345\217\243", nullptr));
        zoomComboBox->setItemText(8, QApplication::translate("MainWindow", "\345\256\236\351\231\205\345\244\247\345\260\217", nullptr));

        compareLabel->setText(QApplication::translate("MainWindow", "\360\237\223\213 \345\257\271\346\257\224\346\250\241\345\274\217\357\274\232", nullptr));
        compareModeComboBox->setItemText(0, QApplication::translate("MainWindow", "\345\271\266\346\216\222\345\257\271\346\257\224", nullptr));
        compareModeComboBox->setItemText(1, QApplication::translate("MainWindow", "\346\273\221\345\212\250\345\257\271\346\257\224", nullptr));
        compareModeComboBox->setItemText(2, QApplication::translate("MainWindow", "\345\210\207\346\215\242\345\257\271\346\257\224", nullptr));
        compareModeComboBox->setItemText(3, QApplication::translate("MainWindow", "\345\220\214\346\255\245\347\274\251\346\224\276", nullptr));

        savePreviewBtn->setText(QApplication::translate("MainWindow", "\360\237\222\276 \344\277\235\345\255\230\351\242\204\350\247\210", nullptr));
        originalTitleLabel->setText(QApplication::translate("MainWindow", "\360\237\223\267 \345\216\237\345\247\213\347\224\273\351\235\242", nullptr));
        originalImageLabel->setText(QApplication::translate("MainWindow", "\360\237\226\274\357\270\217 \346\232\202\346\227\240\351\242\204\350\247\210\n"
"\350\257\267\351\200\211\346\213\251\344\270\200\344\270\252\345\233\276\347\211\207\346\210\226\350\247\206\351\242\221\346\226\207\344\273\266", nullptr));
        enhancedTitleLabel->setText(QApplication::translate("MainWindow", "\342\234\250 \350\266\205\345\210\206\350\276\250\347\216\207\345\220\216", nullptr));
        enhancedImageLabel->setText(QApplication::translate("MainWindow", "\342\232\241 \347\255\211\345\276\205\345\244\204\347\220\206\n"
"\350\266\205\345\210\206\350\276\250\347\216\207\351\242\204\350\247\210\345\260\206\345\234\250\346\255\244\346\230\276\347\244\272", nullptr));
        sliderCompareLabel->setText(QApplication::translate("MainWindow", "\346\273\221\345\212\250\345\257\271\346\257\224\346\250\241\345\274\217 - \345\274\200\345\217\221\344\270\255", nullptr));
        originalInfoLabel->setText(QApplication::translate("MainWindow", "\345\216\237\345\233\276\344\277\241\346\201\257\357\274\232\346\232\202\346\227\240", nullptr));
        originalInfoLabel->setStyleSheet(QApplication::translate("MainWindow", "color: #666666; font-size: 12px;", nullptr));
        enhancedInfoLabel->setText(QApplication::translate("MainWindow", "\345\242\236\345\274\272\345\233\276\344\277\241\346\201\257\357\274\232\346\232\202\346\227\240", nullptr));
        enhancedInfoLabel->setStyleSheet(QApplication::translate("MainWindow", "color: #666666; font-size: 12px;", nullptr));
        processingTimeLabel->setText(QApplication::translate("MainWindow", "\345\244\204\347\220\206\346\227\266\351\227\264\357\274\232--", nullptr));
        processingTimeLabel->setStyleSheet(QApplication::translate("MainWindow", "color: #666666; font-size: 12px;", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuView->setTitle(QApplication::translate("MainWindow", "\350\247\206\345\233\276", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
