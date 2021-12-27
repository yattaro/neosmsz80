#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //const QIcon openIcon = QIcon(":sms_cart.png");
    //QAction *openAct = new QAction(openIcon, "Open ROM...", this);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete current_emu;
}
void MainWindow::setRegisterVals()
{
    current_proc->strregs(regValBuff);
    ui->z80RegView->setPlainText(QString(regValBuff));
}

void MainWindow::setMemView(const char *memStrings)
{
    ui->z80MemView->setPlainText(QString(memStrings));
}

void MainWindow::on_actionOpen_ROM_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open ROM File", QDir::homePath(), "ROM Files (*.sms *.gg *.bin)");
    if(!filename.isNull())
    {
        current_emu = new sms(filename.toStdString());
        current_memory = current_emu->get_mem();
        current_proc = current_emu->get_proc();
        setRegisterVals();
        ui->actionRun->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionStop->setEnabled(false);
    }
}


void MainWindow::on_actionRun_triggered()
{
    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);
}
