#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
#include "global.h"
#include <QDialog>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_variety_pushButton_clicked();
    void slot_register_finish(RequestId id, QString res, ErrorCode errcode);

private:
    void verificationTip(QString str, bool b_ok);
    void initHandler();

    Ui::RegisterDialog *ui;
    QMap<RequestId, std::function<void(const QJsonObject&)>> handler_;

};

#endif // REGISTERDIALOG_H
