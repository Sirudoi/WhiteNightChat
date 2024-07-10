#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>

/**
 * @brief reflush qss setting of QWidget
*/
extern std::function<void(QWidget*)> g_repolish;

/**
 * @brief 路由请求id
 * ID_GET_VARIFY_CODE   :获取验证码
 * ID_REG_USER          :用户注册
*/
enum RequestId {
    ID_GET_VARIFY_CODE = 1001,
    ID_REG_USER = 1002
};

/**
 * @brief 路由请求模块
 * REGISTER:注册模块
*/
enum Module {
    REGISTER = 0
};

/**
 * @brief 错误类型
 * SUCCESS      :解析成功
 * ERR_JSON     :json格式解析失败
 * ERR_NETWORKD :网络错误
*/
enum ErrorCode {
    SUCCESS = 0,
    ERR_JSON = 1,
    ERR_NETWORKD = 2
};

#endif // GLOBAL_H
