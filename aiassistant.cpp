#include "aiassistant.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>

AIAssistant::AIAssistant(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void AIAssistant::generateDSL(const QString &doc, const QString &prompt, const QString &apiKey)
{
    QString fullPrompt = doc + "\n用户提示: " + prompt + "\n请只返回 DSL 代码，不要解释。";

    QUrl url("https://api2.aigcbest.top/v1/chat/completions");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

    // 构建 JSON 请求
    QJsonObject message;
    message["role"] = "user";
    message["content"] = fullPrompt;

    QJsonArray messages;
    messages.append(message);

    QJsonObject json;
    json["model"] = "gpt-5-mini";
    json["messages"] = messages;
    json["temperature"] = 0; // 稳定输出
    json["max_tokens"] = 1000;

    QJsonDocument docJson(json);
    QByteArray data = docJson.toJson();

    QProgressDialog *progress = new QProgressDialog("AI 正在生成 DSL 代码，请稍候...", nullptr, 0, 0);
    progress->setWindowModality(Qt::ApplicationModal);
    progress->setCancelButton(nullptr);
    progress->setWindowTitle("处理中");
    progress->setMinimumDuration(0);
    progress->show();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [reply, this, progress]() {
        progress->close();
        progress->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject obj = jsonResponse.object();
        QString text;
        if (obj.contains("choices")) {
            QJsonArray choices = obj["choices"].toArray();
            if (!choices.isEmpty()) {
                text = choices[0].toObject()["message"].toObject()["content"].toString();
            }
        }
        emit dslReady(text.trimmed());
        reply->deleteLater();
    });
}
