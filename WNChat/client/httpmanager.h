#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H
#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// CRTP模板, 即一个类可以继承自以他为模板的类
class HttpManager :
    public QObject,
    public Singleton<HttpManager>,
    public std::enable_shared_from_this<HttpManager>
{
    Q_OBJECT

public:
    ~HttpManager();

private:
    // Singleton在初始化时会创建HttpManager对象的指针, 这里构造函数私有之后会无法调用
    // 因此将Singleton设置为友元
    friend class Singleton<HttpManager>;
    HttpManager();

    QNetworkAccessManager manager_;
    void PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Module module);

private slots:
    void slot_http_finish(RequestId id, QString res, ErrorCode errcode, Module mod);

signals:
    void sig_http_finish(RequestId id, QString res, ErrorCode errcode, Module mod);
    void sig_register_finish(RequestId id, QString res, ErrorCode errcode);
};

#endif // HTTPMANAGER_H
