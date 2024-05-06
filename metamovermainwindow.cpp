#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <vector>

#include "metamovermainwindow.h"
#include "./ui_metamovermainwindow.h"

MetaMoverMainWindow::MetaMoverMainWindow(Scanner* scanner, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MetaMoverMainWindow),
    appConfigManager(AppConfig::get()),
    appScanner(scanner)
{
    lockSlots = true;
    ui->setupUi(this);
    this->setupUiElements();
    this->loadAppConfig();
    lockSlots = false;
}

MetaMoverMainWindow::~MetaMoverMainWindow()
{
    this->saveAppConfig();
    delete ui;
}

void MetaMoverMainWindow::setupUiElements()
{
    connect(this, &MetaMoverMainWindow::startScan, appScanner, &Scanner::scan);
    connect(appScanner, &Scanner::filesFoundUpdated, this, &MetaMoverMainWindow::updateFileCountUI, Qt::QueuedConnection);
    connect(appScanner, &Scanner::scanCompleted, this, &MetaMoverMainWindow::showScanResults);
    // Initialize Ui Element Models
    this->setupIfDuplicatesFoundOptions();
    this->setupMediaOutputFolderStructureOptions();
    this->toggleTransferControls(false); //Disable controls that require scan results
}

void MetaMoverMainWindow::saveAppConfig()
{
    this->setSourceDirectory(ui->lineEditSourceDir->text().toStdString());
    this->setFileMetaInvalidMoveToFolderCheckbox(ui->checkBoxInvalidMetaMove->isChecked());
    this->setIncludeSubdirectoriesCheckbox(ui->checkBoxIncludeSubDir->isChecked());
    this->setOutputDirForInvalidMetaFiles(ui->lineEditInvalidMetaDir->text().toStdString());
    this->setOutputDirectory(ui->lineEditOutputDirectory->text().toStdString());
    this->setOutputDuplicatesDirectory(ui->lineEditDuplicatesDir->text().toStdString());
    this->setIfDuplicatesFoundSelection(ui->comboBoxDuplicateSelection->currentText().toStdString());
    this->setPhotosOutputFolderStructureSelection(ui->comboBoxPhotoOutputFolderStructure->currentText().toStdString());
    appConfigManager.save();
}

void MetaMoverMainWindow::loadAppConfig()
{
    appConfigManager.load();
    this->setSourceDirectory(appConfigManager.config.getSourceDirectory());
    this->setFileMetaInvalidMoveToFolderCheckbox(appConfigManager.config.getMoveInvalidFileMeta());
    this->setIncludeSubdirectoriesCheckbox(appConfigManager.config.getIncludeSubDirectories());
    this->setOutputDirForInvalidMetaFiles(appConfigManager.config.getInvalidFileMetaDirectory());
    this->setOutputDirectory(appConfigManager.config.getOutputDirectory());
    this->setOutputDuplicatesDirectory(appConfigManager.config.getDuplicatesDirectory());
    this->setIfDuplicatesFoundSelection(appConfigManager.config.getDuplicatesFoundSelection());
    this->setPhotosOutputFolderStructureSelection(appConfigManager.config.getPhotosOutputFolderStructureSelection());
    this->setPhotosDuplicateIdentitySetting(appConfigManager.config.getPhotosDuplicateIdentitySetting());
    this->setPhotosReplaceDashesWithUnderscoresCheckbox(appConfigManager.config.getPhotosReplaceDashesWithUnderscores());
}

void MetaMoverMainWindow::setupIfDuplicatesFoundOptions()
{
    std::vector<std::string> options = appConfigManager.config.getDuplicatesFoundOptions();
    for(std::string& option : options){
        ui->comboBoxDuplicateSelection->addItem(QString::fromStdString(option));
    }
}

void MetaMoverMainWindow::setupMediaOutputFolderStructureOptions()
{
    std::vector<std::string> options = appConfigManager.config.getMediaOutputFolderStructureOptions();
    for(std::string& option : options){
        ui->comboBoxPhotoOutputFolderStructure->addItem(QString::fromStdString(option));
    }
}

void MetaMoverMainWindow::resetScanResults()
{
    ui->lineEditFilesFound->setText(QString::number(0));
    ui->lineEditPhotoFilesFound->setText(QString::number(0));
    ui->lineEditPhotoHadEXIFData->setText(QString::number(0));
    ui->lineEditPhotoHasEXIFDataNoDate->setText(QString::number(0));
    ui->lineEditPhotoHasEXIFDataWDate->setText(QString::number(0));
    ui->lineEditVideoFilesFound->setText(QString::number(0));
    ui->lineEditVideoHadEXIFData->setText(QString::number(0));
    ui->lineEditVideoHasEXIFDataNoDate->setText(QString::number(0));
    ui->lineEditVideoHasEXIFDataWDate->setText(QString::number(0));
}

void MetaMoverMainWindow::toggleScanControls(bool enabled){
    ui->lineEditSourceDir->setDisabled(!enabled);
    ui->checkBoxIncludeSubDir->setDisabled(!enabled);
    ui->checkBoxInvalidMetaMove->setDisabled(!enabled);
    ui->lineEditInvalidMetaDir->setDisabled(!enabled);
    ui->lineEditOutputDirectory->setDisabled(!enabled);
    ui->comboBoxDuplicateSelection->setDisabled(!enabled);
    ui->lineEditDuplicatesDir->setDisabled(!enabled);
    ui->pushButtonBrowseSource->setDisabled(!enabled);
    ui->pushButtonBrowseInvalidMetaDir->setDisabled(!enabled);
    ui->pushButtonBrowseOutput->setDisabled(!enabled);
    ui->pushButtonDuplicatesDirBrowse->setDisabled(!enabled);
    ui->pushButtonScan->setDisabled(!enabled);
}

void MetaMoverMainWindow::toggleTransferControls(bool enabled)
{
    ui->pushButtonCopyAll->setDisabled(!enabled);
    ui->pushButtonMoveAll->setDisabled(!enabled);
    ui->pushButtonPhotoCopy->setDisabled(!enabled);
    ui->pushButtonPhotoMove->setDisabled(!enabled);
    ui->pushButtonVideoCopy->setDisabled(!enabled);
    ui->pushButtonVideoMove->setDisabled(!enabled);
    ui->groupBoxVideoFileTypesToMoveOrCopy->setDisabled(!enabled);
    ui->groupBoxVideoCameraTypesToMoveOrCopy->setDisabled(!enabled);
    ui->groupBoxPhotoFileTypesToMoveOrCopy->setDisabled(!enabled);
    ui->groupBoxPhotoCameraTypesToMoveOrCopy->setDisabled(!enabled);
}

void MetaMoverMainWindow::setSourceDirectory(std::string selectedFolder)
{
    if(appConfigManager.config.getSourceDirectory() != selectedFolder){
        appConfigManager.config.setSourceDirectory(selectedFolder);
    }
    ui->lineEditSourceDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::setFileMetaInvalidMoveToFolderCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    ui->lineEditInvalidMetaDir->setDisabled(!isChecked);
    ui->pushButtonBrowseInvalidMetaDir->setDisabled(!isChecked);
    if(appConfigManager.config.getMoveInvalidFileMeta() != isChecked)
        appConfigManager.config.setMoveInvalidFileMeta(isChecked);
    if(ui->checkBoxInvalidMetaMove->checkState() != checkState)
        ui->checkBoxInvalidMetaMove->setCheckState(checkState);
}

void MetaMoverMainWindow::setIncludeSubdirectoriesCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    if(appConfigManager.config.getIncludeSubDirectories() != isChecked)
        appConfigManager.config.setIncludeSubDirectories(isChecked);
    if(ui->checkBoxIncludeSubDir->checkState() != checkState)
        ui->checkBoxIncludeSubDir->setCheckState(checkState);
}

void MetaMoverMainWindow::setPhotosReplaceDashesWithUnderscoresCheckbox(bool isChecked)
{
    Qt::CheckState checkState = isChecked ? Qt::Checked : Qt::Unchecked;
    if(appConfigManager.config.getPhotosReplaceDashesWithUnderscores() != isChecked)
        appConfigManager.config.setPhotosReplaceDashesWithUnderscores(isChecked);
    if(ui->checkBoxPhotoReplaceDashesWithUnderScores->checkState() != checkState)
        ui->checkBoxPhotoReplaceDashesWithUnderScores->setCheckState(checkState);
}

void MetaMoverMainWindow::setOutputDirForInvalidMetaFiles(std::string selectedFolder)
{
    if(appConfigManager.config.getInvalidFileMetaDirectory() != selectedFolder){
        appConfigManager.config.setInvalidFileMetaDirectory(selectedFolder);
    }
    ui->lineEditInvalidMetaDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::setOutputDuplicatesDirectory(std::string selectedFolder)
{
    if(appConfigManager.config.getDuplicatesDirectory() != selectedFolder){
        appConfigManager.config.setDuplicatesDirectory(selectedFolder);
    }
    ui->lineEditDuplicatesDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::setOutputDirectory(std::string selectedFolder)
{
    if(appConfigManager.config.getOutputDirectory() != selectedFolder){
        appConfigManager.config.setOutputDirectory(selectedFolder);
    }
    ui->lineEditOutputDirectory->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::setIfDuplicatesFoundSelection(std::string optionSelected)
{
    bool moveToFolderSelected = (optionSelected != "Move To Folder");
    ui->labelDuplicatesDir->setDisabled(moveToFolderSelected);
    ui->lineEditDuplicatesDir->setDisabled(moveToFolderSelected);
    ui->pushButtonDuplicatesDirBrowse->setDisabled(moveToFolderSelected);
    if(appConfigManager.config.getDuplicatesFoundSelection() != optionSelected){
        appConfigManager.config.setDuplicatesFoundSelection(optionSelected);
    }
    if(ui->comboBoxDuplicateSelection->currentText() != QString::fromStdString(optionSelected)){
        ui->comboBoxDuplicateSelection->setCurrentText(QString::fromStdString(optionSelected));
    }
}

void MetaMoverMainWindow::setPhotosOutputFolderStructureSelection(std::string optionSelected)
{
    if(appConfigManager.config.getPhotosOutputFolderStructureSelection() != optionSelected){
        appConfigManager.config.setPhotosOutputFolderStructureSelection(optionSelected);
    }
    if(ui->comboBoxPhotoOutputFolderStructure->currentText() != QString::fromStdString(optionSelected)){
        ui->comboBoxPhotoOutputFolderStructure->setCurrentText(QString::fromStdString(optionSelected));
    }
}

void MetaMoverMainWindow::setPhotosDuplicateIdentitySetting(std::string optionSelected)
{
    if(optionSelected == ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString()){
        ui->radioButtonPhotosDupeSettingFileNameMatch->setChecked(true);
        appConfigManager.config.setPhotosDuplicateIdentitySetting(optionSelected);
    }else if(optionSelected == ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->text().toStdString()){
        ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->setChecked(true);
        appConfigManager.config.setPhotosDuplicateIdentitySetting(optionSelected);
    }else{
        ui->radioButtonPhotosDupeSettingFileNameMatch->setChecked(true);
        std::string newSelection = ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString();
        appConfigManager.config.setPhotosDuplicateIdentitySetting(newSelection);
    }
}

std::string MetaMoverMainWindow::launchDirectoryBrowser(std::string dialogTitle,
                                                        std::string failMsg,
                                                        std::string startingDir)
{
    if(!QDir(QString::fromStdString(startingDir)).exists()){
        startingDir = QDir::homePath().toStdString();
    }
    QString selectedFolder = QFileDialog::getExistingDirectory(0,
                                                               QString::fromStdString(dialogTitle),
                                                               QString::fromStdString(startingDir));
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


// observer functions
void MetaMoverMainWindow::updateFileCountUI() {
    QMetaObject::invokeMethod(this, [this]() {
            updateFileCounts();
        }, Qt::QueuedConnection);
}

void MetaMoverMainWindow::showScanResults() {
    toggleTransferControls(true);
    toggleScanControls(true);
    updateFileCounts();
}

void MetaMoverMainWindow::updateFileCounts(){
    ui->lineEditFilesFound->setText(QString::number(appScanner->getTotalFilesFound()));
    ui->lineEditPhotoFilesFound->setText(QString::number(appScanner->getTotalPhotoFilesFound()));
    ui->lineEditPhotoHadEXIFData->setText(QString::number(appScanner->getPhotoFilesFoundContainingEXIFData()));
    ui->lineEditPhotoHasEXIFDataWDate->setText(QString::number(appScanner->getPhotoFilesFoundContainingValidCreationDate()));
    ui->lineEditPhotoHasEXIFDataNoDate->setText(QString::number(appScanner->getPhotoFilesFoundContainingEXIFWODate()));
}


// ui slots
void MetaMoverMainWindow::on_pushButtonBrowseSource_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Source Directory",
                                                           "Invalid Source Directory Selection",
                                                           appConfigManager.config.getSourceDirectory());
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->setSourceDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonBrowseOutput_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Output Directory",
                                                           "Invalid Output Directory Selection",
                                                           appConfigManager.config.getOutputDirectory());
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->setOutputDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonBrowseInvalidMetaDir_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Meta Directory",
                                                           "Invalid Meta Directory Selection",
                                                           appConfigManager.config.getInvalidFileMetaDirectory());
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->setOutputDirForInvalidMetaFiles(selectedDir);
    }
}

void MetaMoverMainWindow::on_pushButtonDuplicatesDirBrowse_clicked()
{
    if(lockSlots) {return;}
    std::string selectedDir = this->launchDirectoryBrowser("Select Duplicates Directory",
                                                           "Invalid Duplicates Directory Selection",
                                                           appConfigManager.config.getDuplicatesDirectory());
    // Directory selected is validated - set as source
    if (selectedDir.empty()) {
        return;
    } else {
        this->setOutputDuplicatesDirectory(selectedDir);
    }
}

void MetaMoverMainWindow::on_checkBoxInvalidMetaMove_clicked()
{
    if(lockSlots) {return;}
    this->setFileMetaInvalidMoveToFolderCheckbox(ui->checkBoxInvalidMetaMove->isChecked());
}

void MetaMoverMainWindow::on_checkBoxIncludeSubDir_clicked()
{
    if(lockSlots) {return;}
    this->setIncludeSubdirectoriesCheckbox(ui->checkBoxIncludeSubDir->isChecked());
}

void MetaMoverMainWindow::on_comboBoxDuplicateSelection_currentTextChanged(const QString &currentSelection)
{
    if(lockSlots) {return;}
    setIfDuplicatesFoundSelection(currentSelection.toStdString());
}

void MetaMoverMainWindow::on_comboBoxPhotoOutputFolderStructure_currentTextChanged(const QString &currentSelection)
{
    if(lockSlots) {return;}
    setPhotosOutputFolderStructureSelection(currentSelection.toStdString());
}

void MetaMoverMainWindow::on_radioButtonPhotosDupeSettingFileNameMatch_clicked()
{
    setPhotosDuplicateIdentitySetting(
        ui->radioButtonPhotosDupeSettingFileNameMatch->text().toStdString());
}

void MetaMoverMainWindow::on_radioButtonPhotosDupeSettingEXIFAndFileNameMatch_clicked()
{
    setPhotosDuplicateIdentitySetting(
        ui->radioButtonPhotosDupeSettingEXIFAndFileNameMatch->text().toStdString());
}

void MetaMoverMainWindow::on_checkBoxPhotoReplaceDashesWithUnderScores_clicked()
{
    setPhotosReplaceDashesWithUnderscoresCheckbox(ui->checkBoxPhotoReplaceDashesWithUnderScores->isChecked());
}

void MetaMoverMainWindow::on_pushButtonScan_clicked()
{
    if(!appConfigManager.scanConfigurationValid(true)){ return; }
    toggleScanControls(false);
    resetScanResults();
    emit startScan(appConfigManager.config.getSourceDirectory(),
                    appConfigManager.config.getIncludeSubDirectories());
}

