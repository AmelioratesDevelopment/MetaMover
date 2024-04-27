#ifndef METAMOVERMAINWINDOW_H
#define METAMOVERMAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include "scanner.h"
#include "appconfigmanager.h"

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
    bool lockSlots;

    // Class Functions
    void setupUiElements();
    void setupIfDuplicatesFoundOptions();
    void setupMediaOutputFolderStructureOptions();
    void loadAppConfig();
    void saveAppConfig();
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

public:
    explicit MetaMoverMainWindow(Scanner* scanner, QWidget *parent = nullptr);
    ~MetaMoverMainWindow();

private slots:
    void updateFileCountUI(int filesFound);
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

};
#endif // METAMOVERMAINWINDOW_H
