#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>

#include "metamovermainwindow.h"
#include "./ui_metamovermainwindow.h"


MetaMoverMainWindow::MetaMoverMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MetaMoverMainWindow)
    , appConfigManager(AppConfig::get())
{
    ui->setupUi(this);
    this->LoadAppConfig();
}

MetaMoverMainWindow::~MetaMoverMainWindow()
{
    delete ui;
    appConfigManager.save();
}

void MetaMoverMainWindow::LoadAppConfig()
{
    appConfigManager.load();
    SetSourceDirectory(appConfigManager.GetSourceDirectory());
    SetOutputDirectory(appConfigManager.GetOutputDirectory());
}

void MetaMoverMainWindow::SetSourceDirectory(std::string selectedFolder)
{
    if(appConfigManager.GetSourceDirectory() != selectedFolder){
        appConfigManager.SetSourceDirectory(selectedFolder);
    }
    ui->lineEditSourceDir->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::SetOutputDirectory(std::string selectedFolder)
{
    if(appConfigManager.GetOutputDirectory() != selectedFolder){
        appConfigManager.SetOutputDirectory(selectedFolder);
    }
    ui->lineEditOutputDirectory->setText(QString::fromStdString(selectedFolder));
}

void MetaMoverMainWindow::on_pushButtonBrowseSource_clicked()
{
    QString selectedFolder = QFileDialog::getExistingDirectory(0,
                                                               ("Select Source Directory"),
                                                               QDir::homePath());
    if(selectedFolder.isEmpty() || !QDir(selectedFolder).exists()){
        QMessageBox::critical(this,
                              "Error",
                              "Invalid Source Folder Selection",
                              QMessageBox::Ok);
        return;
    }
    // Directory selected is validated - set as source
    this->SetSourceDirectory(selectedFolder.toStdString());
}

void MetaMoverMainWindow::on_pushButtonBrowseOutput_clicked()
{
    QString selectedFolder = QFileDialog::getExistingDirectory(0,
                                                               ("Select Output Directory"),
                                                               QDir::homePath());
    if(selectedFolder.isEmpty() || !QDir(selectedFolder).exists()){
        QMessageBox::critical(this,
                              "Error",
                              "Invalid Source Folder Selection",
                              QMessageBox::Ok);
        return;
    }
    // Directory selected is validated - set as source
    this->SetOutputDirectory(selectedFolder.toStdString());
}

