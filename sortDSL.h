//
// Created by poorc on 2025/10/6.
//

#ifndef SIMULATOR_SORTDSL_H
#define SIMULATOR_SORTDSL_H

#pragma once
#include <QObject>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QVector>
#include "sortwidget.h"

class SortWidget; // 前向声明

// ===================== SortDSL 语法高亮器 =====================
class SortDSLSyntaxHighlighter : public QSyntaxHighlighter {
Q_OBJECT
public:
    explicit SortDSLSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> rules;
};

// ===================== SortDSL 解释器 =====================
class SortDSLInterpreter : public QObject {
Q_OBJECT
public:
    explicit SortDSLInterpreter(SortWidget *sort, QObject *parent = nullptr);

    // 主执行入口
    void run(const QString &code);

private:
    void executeLine(const QString &line, int lineNo);

    SortWidget *sort;
};


#endif //SIMULATOR_SORTDSL_H
