#include "graphDSL.h"
#include <QMessageBox>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
#include <QStringList>

GraphDSLInterpreter::GraphDSLInterpreter(GraphWidget *graph, QObject *parent)
        : QObject(parent), graph(graph) {}

void GraphDSLInterpreter::run(const QString &code) {
    QStringList lines = code.split('\n', Qt::SkipEmptyParts);
    executeBlock(lines);
}


void GraphDSLInterpreter::executeBlock(const QStringList &lines, int startLine) {
    int lineNo = startLine;
    while (lineNo <= lines.size()) {
        QString line = lines[lineNo - 1].trimmed();
        if (line.isEmpty()) { lineNo++; continue; }

        // 支持变量定义：let a = 5
        QRegularExpression letRe(R"(^let\s+(\w+)\s*=\s*(\d+)$)");
        auto letMatch = letRe.match(line);
        if (letMatch.hasMatch()) {
            variables[letMatch.captured(1)] = letMatch.captured(2).toInt();
            lineNo++;
            continue;
        }

        // 支持 for 循环
        QRegularExpression forRe(R"(^for\s+(\w+)\s+in\s+range\s*\(\s*([\w]+)\s*,\s*([\w]+)\s*\)$)");
        auto forMatch = forRe.match(line);
        if (forMatch.hasMatch()) {
            QString var = forMatch.captured(1);
            int start = expandArg(forMatch.captured(2));
            int end   = expandArg(forMatch.captured(3));

            // 收集循环块
            QStringList subBlock;
            lineNo++;
            while (lineNo <= lines.size() && !lines[lineNo-1].trimmed().startsWith("end")) {
                subBlock.append(lines[lineNo-1]);
                lineNo++;
            }

            for (int i = start; i < end; ++i) {
                variables[var] = i;
                executeBlock(subBlock, 1);
            }
            lineNo++; // 跳过 end
            continue;
        }

        // 执行单行命令
        try {
            executeLine(line, lineNo);
        } catch (...) {
            QMessageBox::warning(nullptr, "DSL Error",
                                 QString("Error executing line %1: %2").arg(lineNo).arg(line));
        }
        lineNo++;
    }
}

// ----------------- 将变量替换为数值 -----------------
int GraphDSLInterpreter::expandArg(const QString &arg) const {
    QString a = arg.trimmed();
    if (variables.contains(a))
        return variables[a];
    return a.toInt();
}

// ----------------- 执行单行命令 -----------------
void GraphDSLInterpreter::executeLine(const QString &line, int lineNo) {
    QRegularExpression reCall(R"(^(\w+)\s*\((.*)\)$)");
    auto match = reCall.match(line);

    QString cmd, args;
    if (match.hasMatch()) {
        cmd = match.captured(1);
        args = match.captured(2);
    }
    else {
        cmd = line;
    }

    QStringList list = args.split(',', Qt::SkipEmptyParts);
    for (int i = 0; i < list.size(); ++i)
        list[i] = QString::number(expandArg(list[i]));

    if (cmd == "insertNode") {
        graph->insertNode();
    }
    else if (cmd == "deleteNode" && list.size() == 1) {
        graph->deleteNodeById(list[0].toInt());
    }
    else if (cmd == "addEdge" && list.size() == 3) {
        graph->addEdgeByValue(list[0].toInt(), list[1].toInt(), list[2].toInt());
    }
    else if (cmd == "deleteEdge" && list.size() == 2) {
        graph->deleteEdgeByValue(list[0].toInt(), list[1].toInt());
    }
    else if (cmd == "clear") {
        graph->clear();
    }
    else if (cmd == "bfs" && list.size() == 1) {
        graph->runBFS(list[0].toInt());
    }
    else if (cmd == "dfs" && list.size() == 1) {
        graph->runDFS(list[0].toInt());
    }
    else if (cmd == "prim") graph->runPrim();
    else if (cmd == "kruskal") graph->runKruskal();
    else if (cmd == "dijkstra" && list.size() == 1) graph->runDijkstra(list[0].toInt());
    else if (cmd == "random" && list.size() == 3) graph->random(list[0].toInt(), list[1].toInt(), list[2].toDouble());
    else {
        QMessageBox::warning(nullptr, "DSL Error",
                             QString("Unknown command at line %1: %2").arg(lineNo).arg(cmd));
    }

    graph->update();
}

// ----------------- 高亮器 -----------------
GraphDSLSyntaxHighlighter::GraphDSLSyntaxHighlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent) {
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QBrush(QColor("purple")));
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywords = {"let","for","end","insertNode","deleteNode","addEdge",
                            "deleteEdge","bfs","dfs","prim","kruskal","dijkstra","random"};

    for (const QString &kw : keywords) {
        rules.append({QRegularExpression("\\b" + kw + "\\b"), keywordFormat});
    }

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QBrush(QColor("darkOrange")));
    rules.append({QRegularExpression("\\b\\d+\\b"), numberFormat});

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QBrush(QColor("gray")));
    rules.append({QRegularExpression("#.*$"), commentFormat});
}

void GraphDSLSyntaxHighlighter::highlightBlock(const QString &text) {
    for (const auto &rule : rules) {
        QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
        while (i.hasNext()) {
            auto match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
