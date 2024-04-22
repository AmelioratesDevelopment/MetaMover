#ifndef METAMOVERMAINWINDOW_H
#define METAMOVERMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MetaMoverMainWindow;
}
QT_END_NAMESPACE

class MetaMoverMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MetaMoverMainWindow(QWidget *parent = nullptr);
    ~MetaMoverMainWindow();

private:
    Ui::MetaMoverMainWindow *ui;
};
#endif // METAMOVERMAINWINDOW_H
