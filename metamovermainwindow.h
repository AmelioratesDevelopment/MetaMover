#ifndef METAMOVERMAINWINDOW_H
#define METAMOVERMAINWINDOW_H

#include <QMainWindow>
#include "appconfigmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MetaMoverMainWindow;
}
QT_END_NAMESPACE

class MetaMoverMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetaMoverMainWindow(QWidget *parent = nullptr);
    ~MetaMoverMainWindow();

    void setupUiElements();
    void setupIfDuplicatesFoundOptions();
    void setupMediaOutputFolderStructureOptions();
    void LoadAppConfig();
    void SaveAppConfig();
    void SetSourceDirectory(std::string selectedFolder);
    void SetFileMetaInvalidMoveToFolderCheckbox(bool isChecked);
    void SetIncludeSubdirectoriesCheckbox(bool isChecked);
    void SetPhotosDuplicateIdentitySetting(std::string optionSelected);
    void SetPhotosReplaceDashesWithUnderscoresCheckbox(bool isChecked);
    void SetOutputDirectory(std::string selectedFolder);
    void SetOutputDirForInvalidMetaFiles(std::string selectedFolder);
    void SetOutputDuplicatesDirectory(std::string selectedFolder);
    void SetIfDuplicatesFoundSelection(std::string optionSelected);
    void SetPhotosOutputFolderStructureSelection(std::string optionSelected);

    std::string launchDirectoryBrowser(std::string dialogTitle, std::string failMsg);

private slots:
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

private:
    bool lockSlots;
    Ui::MetaMoverMainWindow *ui;
    AppConfigManager appConfigManager;
};
#endif // METAMOVERMAINWINDOW_H
