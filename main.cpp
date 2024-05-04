#include "metamovermainwindow.h"
#include "scanner.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MetaMover_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Set up the Scanner in its own thread
    QThread scannerThread;
    Scanner* scanner = new Scanner(); // Configure with desired thread pool size
    scanner->moveToThread(&scannerThread);
    scannerThread.start();

    // Pass the scanner to the main window
    MetaMoverMainWindow w(scanner);
    w.show();

    int execResult = a.exec();

    // Clean up the thread
    scannerThread.quit();
    scannerThread.wait();
    delete scanner;

    return execResult;
}
