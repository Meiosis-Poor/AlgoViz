#pragma once
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

class AIAssistant : public QObject
{
Q_OBJECT
public:
    explicit AIAssistant(QObject *parent = nullptr);

    // 调用 AI，prompt 是用户输入，doc 是 DSL 使用说明
    void generateDSL(const QString &doc, const QString &prompt, const QString &apiKey="sk-Ks4vGJf192y1KKvkYe8tAksASsAgsHydoxcZzsHgebv4Jb4W");

signals:
    // 返回 DSL 代码
    void dslReady(const QString &dslCode);
    void errorOccurred(const QString &errorMsg);

private:
    QNetworkAccessManager *manager;
};
