#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmanager.h"
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->password_lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->confirm_lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    // set inital property
    ui->error_tip->setProperty("state", "normal");
    // call repolish to polish qss setting of error_tip label
    g_repolish(ui->error_tip);

    connect(HttpManager::GetInstance().get(), &HttpManager::sig_register_finish, this, RegisterDialog::slot_register_finish);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

/**
 * @brief 点击发送验证码,发送验证码及错误提示
*/
void RegisterDialog::on_variety_pushButton_clicked()
{
    auto email = ui->email_lineEdit->text();
    // 验证邮箱格式是否正确
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match){
        // TODO:发送验证码至邮箱
    } else {
        // notify email format is incorrect
        // TODO:notice the static method:tr()
        verificationTip(tr("email format is incorrect"), false);
    }
}

/**
 * @brief 注册模块响应完成, 读取http服务器响应内容
*/
void RegisterDialog::slot_register_finish(RequestId id, QString res, ErrorCode errcode)
{
    if (errcode != ErrorCode::SUCCESS) {
        verificationTip(tr("网络请求错误"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        verificationTip(tr("json解析失败"), false);
        return;
    }

    if (jsonDoc.isObject()) {
        verificationTip(tr("json解析失败"), false);
        return;
    }

    // 根据id交由回调函数处理
    handler_[id](jsonDoc.object());
    return;
}

void RegisterDialog::verificationTip(const QString str, bool b_ok)
{
    ui->error_tip->setText(str);
    if (b_ok) {
        ui->error_tip->setProperty("state", "normal");
    } else {
        ui->error_tip->setProperty("state", "error");
    }

    g_repolish(ui->error_tip);

}

void RegisterDialog::initHandler()
{
    // 注册获取验证码
    handler_.insert(RequestId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        //TODO:获取验证码逻辑
        if (jsonObj["error"].toInt() != ErrorCode::SUCCESS) {
            verificationTip(tr("参数错误"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        verificationTip(tr("验证码已发送至邮箱, 请注意查收"), true);
        qDebug() << "emali is " << email;
    });
}

