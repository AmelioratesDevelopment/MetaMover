#ifndef METAMOVERMAINWINDOW_H
#define METAMOVERMAINWINDOW_H

/***********************************************************************
 * File Name: MetaMoverMainWindow.h
 * Author(s): Blake Azuela
 * Date Created: 2024-05-06
 * Description: Header file for the MetaMoverMainWindow class, which manages
 *              the user interface for the MetaMover application. This class
 *              includes setups for UI components, event handling for user
 *              interactions, and configurations for file and directory
 *              management options within the UI.
 * License: MIT License
 ***********************************************************************/

#include <QMainWindow>
#include <QDir>
#include <QTimer>
#include <QMetaType>
#include <vector>
#include "scanner.h"
#include "transfermanager.h"
#include "appconfigmanager.h"

// Type alias
using PhotoFileHandlerVector = std::vector<std::unique_ptr<PhotoFileHandler>>;

Q_DECLARE_METATYPE(PhotoFileHandlerVector*)

QT_BEGIN_NAMESPACE
namespace Ui {
class MetaMoverMainWindow;
}
QT_END_NAMESPACE

class MetaMoverMainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Class Variables
    Ui::MetaMoverMainWindow *ui;
    AppConfigManager appConfigManager;
    Scanner *appScanner;
    TransferManager *transferManager;
    bool lockSlots;
    bool softlyStopPollingTimer; //used to softly stop timer
    int timerSoftStopCycleCount;
    int currentTimerSoftStopCycleCount;
    QTimer *pollingTimer;

    // Class Functions
    void setupUiElements();
    void enableScanControls(bool enabled);
    void enableTransferControls(bool enabled);
    void setupIfDuplicatesFoundOptions();
    void setupMediaOutputFolderStructureOptions();
    void resetScanResults();
    void prepForTransfer();
    void transferCanceled();
    void loadAppConfig();
    void saveAppConfig();
    void updateFileCounts();
    void startPollingTimer();  // Add startTimer function declaration
    void stopPollingTimer();
    std::string launchDirectoryBrowser(std::string dialogTitle,
                                       std::string failMsg,
                                       std::string startingDir = QDir::homePath().toStdString());

    // Setters
    void setSourceDirectory(std::string selectedFolder);
    void setFileMetaInvalidMoveToFolderCheckbox(bool isChecked);
    void setIncludeSubdirectoriesCheckbox(bool isChecked);
    void setPhotosDuplicateIdentitySetting(std::string optionSelected);
    void setPhotosReplaceDashesWithUnderscoresCheckbox(bool isChecked);
    void setOutputDirectory(std::string selectedFolder);
    void setOutputDirForInvalidMetaFiles(std::string selectedFolder);
    void setOutputDuplicatesDirectory(std::string selectedFolder);
    void setIfDuplicatesFoundSelection(std::string optionSelected);
    void setPhotosOutputFolderStructureSelection(std::string optionSelected);

signals:
    void startScan(const std::string& directoryPath, bool includeSubdirectories);
    void startTransfer(std::vector<std::unique_ptr<PhotoFileHandler>> *photoFileHandlers,
                       std::vector<std::unique_ptr<PhotoFileHandler>> *invalidPhotoFileHandlers,
                       bool moveFiles = false);

public:
    explicit MetaMoverMainWindow(Scanner* scanner, TransferManager *transferManager, QWidget *parent = nullptr);
    ~MetaMoverMainWindow();

private slots:
    //void updateFileCountUI();
    //void updateTransferProgress();
    void showScanResults();
    void onTransferFinished();
    void pollingTimerTick();

    //ui triggers
    void on_pushButtonBrowseSource_clicked();
    void on_pushButtonBrowseOutput_clicked();
    void on_checkBoxInvalidMetaMove_clicked();
    void on_checkBoxIncludeSubDir_clicked();
    void on_pushButtonBrowseInvalidMetaDir_clicked();
    void on_comboBoxDuplicateSelection_currentTextChanged(const QString &currentSelection);
    void on_pushButtonDuplicatesDirBrowse_clicked();
    void on_comboBoxPhotoOutputFolderStructure_currentTextChanged(const QString &currentSelection);
    void on_radioButtonPhotosDupeSettingFileNameMatch_clicked();
    void on_radioButtonPhotosDupeSettingEXIFAndFileNameMatch_clicked();
    void on_checkBoxPhotoReplaceDashesWithUnderScores_clicked();
    void on_pushButtonScan_clicked();

    void on_pushButtonPhotoCopy_clicked();
    void on_pushButtonPhotoMove_clicked();
    void on_pushButtonCancel_clicked();
};
#endif // METAMOVERMAINWINDOW_H
