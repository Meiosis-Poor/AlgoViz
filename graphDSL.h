//
// Created by poorc on 2025/10/5.
//

#ifndef SIMULATOR_GRAPHDSL_H
#define SIMULATOR_GRAPHDSL_H

#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QCompleter>
#include "GraphWidget.h"

class GraphWidget; // 前置声明，避免循环依赖

class GraphDSLInterpreter : public QObject {
Q_OBJECT
public:
    explicit GraphDSLInterpreter(GraphWidget *graph, QObject *parent = nullptr);
    void run(const QString &code);

private:
    GraphWidget *graph;
    QMap<QString, int> variables; // 变量存储
    void executeLine(const QString &line, int lineNo);
    void executeBlock(const QStringList &lines, int startLine = 1);

    int expandArg(const QString &arg) const; // 将变量替换成数值
};

// 语法高亮器
class GraphDSLSyntaxHighlighter : public QSyntaxHighlighter {
Q_OBJECT
public:
    explicit GraphDSLSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> rules;
};


#endif //SIMULATOR_GRAPHDSL_H
