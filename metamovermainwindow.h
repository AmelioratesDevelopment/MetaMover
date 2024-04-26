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
    void loadAppConfig();
    void saveAppConfig();
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
