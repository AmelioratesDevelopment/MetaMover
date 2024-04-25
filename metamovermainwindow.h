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

    void LoadAppConfig();

    void SetSourceDirectory(std::string selectedFolder);
    void SetOutputDirectory(std::string selectedFolder);

private slots:
    void on_pushButtonBrowseSource_clicked();
    void on_pushButtonBrowseOutput_clicked();

private:
    Ui::MetaMoverMainWindow *ui;
    AppConfigManager appConfigManager;
};
#endif // METAMOVERMAINWINDOW_H
