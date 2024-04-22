#include "metamovermainwindow.h"
#include "./ui_metamovermainwindow.h"

MetaMoverMainWindow::MetaMoverMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MetaMoverMainWindow)
{
    ui->setupUi(this);
}

MetaMoverMainWindow::~MetaMoverMainWindow()
{
    delete ui;
}
