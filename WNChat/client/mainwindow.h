#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      main windows
 *
 * @author     White Night
 * @date       2024/07/10
 * @history
 *****************************************************************************/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void SlotSwitchRegister();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginDialog* login_dialog_;
    RegisterDialog* register_dialog_;
};
#endif // MAINWINDOW_H
