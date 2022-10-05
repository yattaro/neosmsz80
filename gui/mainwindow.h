#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "../emu/sms.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRegisterVals();
    void setMemView();

private slots:
    void on_actionOpen_ROM_triggered();

    void on_actionRun_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;
    void openFile();
    sms *current_emu;
    bool current_emu_initialized = false;
    z80mem *current_memory;
    z80proc *current_proc;
    char regValBuff[198];
};
#endif // MAINWINDOW_H
