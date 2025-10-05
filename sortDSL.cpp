#include "sortDSL.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QStringList>

SortDSLInterpreter::SortDSLInterpreter(SortWidget *sort, QObject *parent)
        : QObject(parent), sort(sort) {}

// -------------------- 主执行入口 --------------------
void SortDSLInterpreter::run(const QString &code) {
    QStringList lines = code.split('\n', Qt::SkipEmptyParts);
    int lineNo = 1;
    for (const QString &rawLine : lines) {
        QString line = rawLine.trimmed();
        if (line.isEmpty() || line.startsWith("#")) continue;

        try {
            executeLine(line, lineNo);
        } catch (...) {
            QMessageBox::warning(nullptr, "DSL Error",
                                 QString("Error executing line %1: %2").arg(lineNo).arg(line));
        }
        lineNo++;
    }
}

// -------------------- 执行单行命令 --------------------
void SortDSLInterpreter::executeLine(const QString &line, int lineNo) {
    QRegularExpression reCall(R"(^(\w+)\s*\((.*)\)$)");
    auto match = reCall.match(line);

    QString cmd, args;
    if (match.hasMatch()) {
        cmd = match.captured(1);
        args = match.captured(2);
    } else {
        cmd = line;
    }

    QStringList list = args.split(',', Qt::SkipEmptyParts);
    for (QString &arg : list)
        arg = arg.trimmed();

    // --------------- DSL 命令解析 ---------------
    if (cmd == "insert" && list.size() == 1) {
        sort->visualData.append(list[0].toInt());
        sort->update();
    }
    else if (cmd == "remove" && list.size() == 1) {
        int idx = list[0].toInt();
        if (idx >= 0 && idx < sort->visualData.size())
            sort->visualData.removeAt(idx);
        sort->update();
    }
    else if (cmd == "setMethod" && list.size() == 1) {
        sort->method = list[0].toInt();
        sort->setSortMethod(sort->method);
        sort->update();
    }
    else if (cmd == "setAscending" && list.size() == 1) {
        bool asc = (list[0].toLower() == "true" || list[0] == "1");
        sort->setAscending(asc);
    }
    else if (cmd == "autoRun" && list.size() == 1) {
        sort->autoInterval = list[0].toInt();
        sort->timer->start(sort->autoInterval);
        sort->autoRunning = true;
    }
    else if (cmd == "stop") {
        sort->timer->stop();
        sort->autoRunning = false;
    }
    else if (cmd == "reset") {
        sort->reset();
    }
    else if (cmd == "clear") {
        sort->visualData.clear();
        sort->update();
    }
    else {
        QMessageBox::warning(nullptr, "DSL Error",
                             QString("Unknown command at line %1: %2").arg(lineNo).arg(cmd));
    }
}

SortDSLSyntaxHighlighter::SortDSLSyntaxHighlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent) {

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QBrush(QColor("purple")));
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywords = {
            "insert","remove","setMethod","setAscending",
            "autoRun","stop","reset","clear"
    };

    for (const QString &kw : keywords) {
        rules.append({QRegularExpression("\\b" + kw + "\\b"), keywordFormat});
    }

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QBrush(QColor("darkOrange")));
    rules.append({QRegularExpression("\\b\\d+\\b"), numberFormat});

    QTextCharFormat boolFormat;
    boolFormat.setForeground(QBrush(QColor("darkGreen")));
    rules.append({QRegularExpression("\\b(true|false)\\b"), boolFormat});

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QBrush(QColor("gray")));
    rules.append({QRegularExpression("#.*$"), commentFormat});
}

void SortDSLSyntaxHighlighter::highlightBlock(const QString &text) {
    for (const auto &rule : rules) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            auto match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}