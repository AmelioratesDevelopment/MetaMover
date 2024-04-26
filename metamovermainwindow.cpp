#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <vector>

#include "metamovermainwindow.h"
#include "./ui_metamovermainwindow.h"


MetaMoverMainWindow::MetaMoverMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MetaMoverMainWindow)
    , appConfigManager(AppConfig::get())
{
    lockSlots = true;
    ui->setupUi(this);
    this->setupUiElements();
    this->LoadAppConfig();
    lockSlots = false;
}

MetaMoverMainWindow::~MetaMoverMainWindow()
{
    this->SaveAppConfig();
    delete ui;
}

void MetaMoverMainWindow::setupUiElements()
{
    // Initialize Ui Element Models
    this->setupIfDuplicatesFoundOptions();
    this->setupMediaOutputFolderStructureOptions();
}

void MetaMoverMainWindow::SaveAppConfig()
{
    this->SetSourceDirectory(ui->lineEditSourceDir->text().toStdString());
    this->SetFileMetaInvalidMoveToFolderCheckbox(ui->checkBoxInvalidMetaMove->isChecked());
    this->SetIncludeSubdirectoriesCheckbox(ui->checkBoxIncludeSubDir->isChecked());
    this->SetOutputDirForInvalidMetaFiles(ui->lineEditInvalidMetaDir->text().toStdString());
    this->SetOutputDirectory(ui->lineEditOutputDirectory->text().toStdString());
    this->SetOutputDuplicatesDirectory(ui->lineEditDuplicatesDir->text().toStdString());
    this->SetIfDuplicatesFoundSelection(ui->comboBoxDuplicateSelection->currentText().toStdString());
    this->SetPhotosOutputFolderStructureSelection(ui->comboBoxPhotoOutputFolderStructure->currentText().toStdString());
    appConfigManager.save();
}

void MetaMoverMainWindow::LoadAppConfig()
{
    appConfigManager.load();
    this->SetSourceDirectory(appConfigManager.GetSourceDirectory());    
    this->SetFileMetaInvalidMoveToFolderCheckbox(appConfigManager.GetSourceIfMetaInvalidMoveFlag());
    this->SetIncludeSubdirectoriesCheckbox(appConfigManager.GetSourceIncludeSubDirsFlag());
    this->SetOutputDirForInvalidMetaFiles(appConfigManager.GetSourceIfMetaInvalidMoveDirectory());\
    this->SetOutputDirectory(appConfigManager.GetOutputDirectory());
    this->SetOutputDuplicatesDirectory(appConfigManager.GetOutputDupesDirectory());
    this->SetIfDuplicatesFoundSelection(appConfigManager.GetDuplicatesSelection());
    this->SetPhotosOutputFolderStructureSelection(appConfigManager.GetPhotoOutputFolderStructureSelection());
    this->SetPhotosDuplicateIdentitySetting(appConfigManager.GetPhotosDuplicateIdentitySetting());
    this->SetPhotosReplaceDashesWithUnderscoresCheckbox(appConfigManager.GetPhotosReplaceDashesWithUnderscores());
}

void MetaMoverMainWindow::setupIfDuplicatesFoundOptions()
{
    std::vector<std::string> options = appConfigManager.GetDuplicatesFoundOptions();
    for(std::string& option : options){
        ui->comboBoxDuplicateSelection->addItem(QString::fromStdString(option));
    }
}

void MetaMoverMainWindow::setupMediaOutputFolderStructureOptions()
{
    std::vector<std::string> options = appConfigManager.GetMediaOutputFolderStructureOptions();
    for(std::string& option : options){
        ui->comboBoxPhotoOutputFolderStructure->addItem(QString::fromStdString(option));
    }
}

void MetaMoverMainWindow::SetSourceDirectory(std::string selectedFolder)
{
    if(appConfigManager.GetSourceDirectory() != selectedFolder){
        appConfigManager.SetSourceDirectory(selectedFolder);
    }
    ui->lineEditSourceDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::SetFileMetaInvalidMoveToFolderCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    ui->lineEditInvalidMetaDir->setDisabled(!isChecked);
    ui->pushButtonBrowseInvalidMetaDir->setDisabled(!isChecked);
    if(appConfigManager.GetSourceIfMetaInvalidMoveFlag() != isChecked)
        appConfigManager.SetSourceIfMetaInvalidMoveFlag(isChecked);
    if(ui->checkBoxInvalidMetaMove->checkState() != checkState)
        ui->checkBoxInvalidMetaMove->setCheckState(checkState);
}

void MetaMoverMainWindow::SetIncludeSubdirectoriesCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    if(appConfigManager.GetSourceIncludeSubDirsFlag() != isChecked)
        appConfigManager.SetSourceIncludeSubDirsFlag(isChecked);    
    if(ui->checkBoxIncludeSubDir->checkState() != checkState)
        ui->checkBoxIncludeSubDir->setCheckState(checkState);
}

void MetaMoverMainWindow::SetPhotosReplaceDashesWithUnderscoresCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    if(appConfigManager.GetPhotosReplaceDashesWithUnderscores() != isChecked)
        appConfigManager.SetPhotosReplaceDashesWithUnderscores(isChecked);
    if(ui->checkBoxPhotoReplaceDashesWithUnderScores->checkState() != checkState)
        ui->checkBoxPhotoReplaceDashesWithUnderScores->setCheckState(checkState);
}

void MetaMoverMainWindow::SetOutputDirForInvalidMetaFiles(std::string selectedFolder)
{
    if(appConfigManager.GetSourceIfMetaInvalidMoveDirectory() != selectedFolder){
        appConfigManager.SetSourceIfMetaInvalidMoveDirectory(selectedFolder);
    }
    ui->lineEditInvalidMetaDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::SetOutputDuplicatesDirectory(std::string selectedFolder)
{
    if(appConfigManager.GetOutputDupesDirectory() != selectedFolder){
        appConfigManager.SetOutputDupesDirectory(selectedFolder);
    }
    ui->lineEditDuplicatesDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::SetOutputDirectory(std::string selectedFolder)
{
    if(appConfigManager.GetOutputDirectory() != selectedFolder){
        appConfigManager.SetOutputDirectory(selectedFolder);
    }
    ui->lineEditOutputDirectory->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::SetIfDuplicatesFoundSelection(std::string optionSelected)
{
    bool moveToFolderSelected = (optionSelected != "Move To Folder");
    ui->labelDuplicatesDir->setDisabled(moveToFolderSelected);
    ui->lineEditDuplicatesDir->setDisabled(moveToFolderSelected);
    ui->pushButtonDuplicatesDirBrowse->setDisabled(moveToFolderSelected);
    if(appConfigManager.GetDuplicatesSelection() != optionSelected){
        appConfigManager.SetDuplicatesSelection(optionSelected);
    }
    if(ui->comboBoxDuplicateSelection->currentText() != QString::fromStdString(optionSelected)){
        ui->comboBoxDuplicateSelection->setCurrentText(QString::fromStdString(optionSelected));
    }
}

void MetaMoverMainWindow::SetPhotosOutputFolderStructureSelection(std::string optionSelected)
{
    if(appConfigManager.GetPhotoOutputFolderStructureSelection() != optionSelected){
        appConfigManager.SetPhotoOutputFolderStructureSelection(optionSelected);
    }
    if(ui->comboBoxPhotoOutputFolderStructure->currentText() != QString::fromStdString(optionSelected)){
        ui->comboBoxPhotoOutputFolderStructure->setCurrentText(QString::fromStdString(optionSelected));
    }
}

void MetaMoverMainWindow::SetPhotosDuplicateIdentitySetting(std::string optionSelected)
{
    if(optionSelected == ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString()){
        ui->radioButtonPhotosDupeSettingFileNameMatch->setChecked(true);
        appConfigManager.SetPhotosDuplicateIdentitySetting(optionSelected);
    }else if(optionSelected == ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->text().toStdString()){
        ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->setChecked(true);
        appConfigManager.SetPhotosDuplicateIdentitySetting(optionSelected);
    }else{
        ui->radioButtonPhotosDupeSettingFileNameMatch->setChecked(true);
        appConfigManager.SetPhotosDuplicateIdentitySetting(
            ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString());
    }
}

std::string MetaMoverMainWindow::launchDirectoryBrowser(std::string dialogTitle, std::string failMsg)
{
    QString selectedFolder = QFileDialog::getExistingDirectory(0,
                                                               QString::fromStdString(dialogTitle),
                                                               QDir::homePath());
    if(selectedFolder.isEmpty()) {return "";} //No folder selected
    if(!QDir(selectedFolder).exists()){
        QMessageBox::critical(this,
                              "Error",
                              QString::fromStdString(failMsg),
                              QMessageBox::Ok);
        return "";
    }
    return selectedFolder.toStdString();
}

void MetaMoverMainWindow::on_pushButtonBrowseSource_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Source Directory",
                                                           "Invalid Source Directory Selection");
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->SetSourceDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonBrowseOutput_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Output Directory",
                                                           "Invalid Output Directory Selection");
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->SetOutputDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonBrowseInvalidMetaDir_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Meta Directory",
                                                           "Invalid Meta Directory Selection");
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->SetOutputDirForInvalidMetaFiles(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonDuplicatesDirBrowse_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Duplicates Directory",
                                                           "Invalid Duplicates Directory Selection");
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->SetOutputDuplicatesDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_checkBoxInvalidMetaMove_clicked()
{
    if(lockSlots) {return;}
    this->SetFileMetaInvalidMoveToFolderCheckbox(ui->checkBoxInvalidMetaMove->isChecked());
}

void MetaMoverMainWindow::on_checkBoxIncludeSubDir_clicked()
{
    if(lockSlots) {return;}
    this->SetIncludeSubdirectoriesCheckbox(ui->checkBoxIncludeSubDir->isChecked());
}

void MetaMoverMainWindow::on_comboBoxDuplicateSelection_currentTextChanged(const QString &currentSelection)
{
    if(lockSlots) {return;}
    SetIfDuplicatesFoundSelection(currentSelection.toStdString());
}

void MetaMoverMainWindow::on_comboBoxPhotoOutputFolderStructure_currentTextChanged(const QString &currentSelection)
{
    if(lockSlots) {return;}
    SetPhotosOutputFolderStructureSelection(currentSelection.toStdString());
}

void MetaMoverMainWindow::on_radioButtonPhotosDupeSettingFileNameMatch_clicked()
{
    SetPhotosDuplicateIdentitySetting(
        ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString());
}

void MetaMoverMainWindow::on_radioButtonPhotosDupeSettingEXIFAndFileNameMatch_clicked()
{
    SetPhotosDuplicateIdentitySetting(
        ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->text().toStdString());
}

void MetaMoverMainWindow::on_checkBoxPhotoReplaceDashesWithUnderScores_clicked()
{
    SetPhotosReplaceDashesWithUnderscoresCheckbox(ui->checkBoxPhotoReplaceDashesWithUnderScores->isChecked());
}

