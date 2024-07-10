#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set the parent class of both to mainwindows
    login_dialog_ = new LoginDialog(this);
    setCentralWidget(login_dialog_);

    register_dialog_ = new RegisterDialog(this);

    // connect register signal
    connect(login_dialog_, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);

    // Embed two dialog into the mainwindow
    login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    register_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchRegister()
{
    // hide login ui and show register ui
    setCentralWidget(register_dialog_);
    login_dialog_->hide();
    register_dialog_->show();
}
