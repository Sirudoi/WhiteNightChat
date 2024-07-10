#include "httpmanager.h"



HttpManager::~HttpManager()
{

}

HttpManager::HttpManager()
{
    connect(this, &HttpManager::sig_http_finish, this, &HttpManager::slot_http_finish);
}

void HttpManager::PostHttpRequest(QUrl url, QJsonObject json, RequestId id, Module module)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");                  // 设置http内容为json
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length())); // 设置content length字段, 长度需要用QByteArray序列化

    // 通过智能指针获取this对象, 后续回调需要使用this指针的内容
    // 使用原生this传参可能出现调用回调时this销毁的问题
    // 因此这里将this交由shared pointor管理最好, 后续传参也使用shared pointor
    // 使用shared pointor能保证回调触发前,this对象不会被销毁
    auto self = shared_from_this();

    QNetworkReply* reply = manager_.post(request, data);
    // 捕获self,捕获后引用计数会增加, 保证回调触发this对象不销毁
    connect(reply, &QNetworkReply::finished, [self, reply, id, module](){
        // 错误情况
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            // 完成信号
            emit self->sig_http_finish(id, "", ErrorCode::ERR_NETWORKD, module);
            reply->deleteLater(); // TODO:注意这个
            return;
        }

        // 正常情况
        QString res = reply->readAll();
        emit self->sig_http_finish(id, res, ErrorCode::SUCCESS, module);
        reply->deleteLater();
        return;
    });
}

void HttpManager::slot_http_finish(RequestId id, QString res, ErrorCode errcode, Module mod)
{
    if (mod == Module::REGISTER) {
        emit sig_register_finish(id, res, errcode);
    }
}
