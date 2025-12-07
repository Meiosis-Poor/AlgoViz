#include "sortDSL.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QStringList>
#include <QTextCharFormat>
#include <QBrush>
#include <QColor>

// -------------------- Expression Parser --------------------
class ExprParser {
public:
    ExprParser(const QString &s, const QMap<QString,int>& vars)
            : text(s), vars(vars), pos(0) {}

    int parse() {
        int val = parseExpression();
        skipSpaces();
        return val;
    }

private:
    QString text;
    const QMap<QString,int>& vars;
    int pos;

    void skipSpaces() {
        while (pos < text.length() && text[pos].isSpace())
            pos++;
    }

    // expression = term { (+|-) term }
    int parseExpression() {
        int value = parseTerm();
        while (true) {
            skipSpaces();
            if (match('+')) value += parseTerm();
            else if (match('-')) value -= parseTerm();
            else break;
        }
        return value;
    }

    // term = factor { (*|/) factor }
    int parseTerm() {
        int value = parseFactor();
        while (true) {
            skipSpaces();
            if (match('*')) value *= parseFactor();
            else if (match('/')) {
                int d = parseFactor();
                if (d == 0) d = 1;
                value /= d;
            }
            else break;
        }
        return value;
    }

    // factor = number | variable | '(' expression ')'
    int parseFactor() {
        skipSpaces();

        // 括号
        if (match('(')) {
            int v = parseExpression();
            match(')');
            return v;
        }

        // 数字
        if (text[pos].isDigit()) {
            int start = pos;
            while (pos < text.length() && text[pos].isDigit())
                pos++;
            return text.mid(start, pos - start).toInt();
        }

        // 变量
        if (text[pos].isLetter() || text[pos] == '_') {
            int start = pos;
            while (pos < text.length() &&
                   (text[pos].isLetterOrNumber() || text[pos] == '_'))
                pos++;
            QString var = text.mid(start, pos - start);
            if (vars.contains(var)) return vars[var];
            return 0;
        }

        return 0;
    }

    bool match(QChar ch) {
        skipSpaces();
        if (pos < text.length() && text[pos] == ch) {
            pos++;
            return true;
        }
        return false;
    }
};


SortDSLInterpreter::SortDSLInterpreter(SortWidget *sort, QObject *parent)
        : QObject(parent), sort(sort) {}

// -------------------- 主执行入口 --------------------
void SortDSLInterpreter::run(const QString &code) {
    QStringList lines = code.split('\n', Qt::SkipEmptyParts);
    executeBlock(lines);
}

// -------------------- 支持块执行（for、let） --------------------
void SortDSLInterpreter::executeBlock(const QStringList &lines, int startLine) {
    int lineNo = startLine;
    while (lineNo <= lines.size()) {
        QString line = lines[lineNo - 1].trimmed();
        if (line.isEmpty() || line.startsWith("#")) { lineNo++; continue; }

        // ---------- let 变量定义 ----------
        QRegularExpression letRe(R"(^let\s+(\w+)\s*=\s*(.+)$)");
        auto letMatch = letRe.match(line);
        if (letMatch.hasMatch()) {
            QString name = letMatch.captured(1);
            QString expr = letMatch.captured(2);
            variables[name] = expandArg(expr);
            lineNo++;
            continue;
        }

        // ---------- for 循环 ----------
        // ---------- for 循环 ----------
        QRegularExpression forRe(R"(^for\s+(\w+)\s+in\s+range\s*\(\s*([\w]+)\s*,\s*([\w]+)\s*\)$)");
        auto forMatch = forRe.match(line);
        if (forMatch.hasMatch()) {
            QString var = forMatch.captured(1);
            int start = expandArg(forMatch.captured(2));
            int end   = expandArg(forMatch.captured(3));

            // 收集循环块（支持嵌套）
            QStringList subBlock;
            lineNo++;

            int depth = 1;  // 当前 for 的嵌套深度

            while (lineNo <= lines.size()) {
                QString l = lines[lineNo - 1].trimmed();

                if (l.startsWith("for")) {
                    depth++;
                } else if (l.startsWith("end")) {
                    depth--;
                    if (depth == 0) {
                        break;  // 找到匹配的 end
                    }
                }

                subBlock.append(lines[lineNo - 1]);
                lineNo++;
            }

            if (depth != 0) {
                QMessageBox::warning(nullptr, "DSL Error",
                                     QString("Missing 'end' for 'for' starting at line %1").arg(startLine));
                return;
            }

            // 执行循环
            for (int i = start; i < end; ++i) {
                variables[var] = i;
                executeBlock(subBlock, 1);
            }

            lineNo++;  // 跳过匹配的 end
            continue;
        }

        // ---------- 单行命令 ----------
        try {
            executeLine(line, lineNo);
        } catch (...) {
            QMessageBox::warning(nullptr, "DSL Error",
                                 QString("Error executing line %1: %2").arg(lineNo).arg(line));
        }
        lineNo++;
    }
}

// -------------------- 参数变量替换 --------------------
int SortDSLInterpreter::expandArg(const QString &arg) const {
    ExprParser parser(arg, variables);
    return parser.parse();
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
    for (int i = 0; i < list.size(); ++i)
        list[i] = QString::number(expandArg(list[i]));

    // ---------- DSL 命令 ----------
    if (cmd == "insert") {
        if (list.size() == 1) {
            int val = list[0].toInt();
            int pos = sort->visualData.size();
            sort->insertNode(pos, val);
        } else if (list.size() == 2) {
            int pos = list[0].toInt();
            int val = list[1].toInt();
            sort->insertNode(pos, val);
        } else {
            QMessageBox::warning(nullptr, "DSL Error",
                                 QString("Invalid insert syntax at line %1").arg(lineNo));
        }
    }
    else if (cmd == "remove" && list.size() == 1) {
        int idx = list[0].toInt();
        if (idx >= 0 && idx < sort->visualData.size())
            sort->visualData.removeAt(idx);
        sort->update();
    }
    else if (cmd == "sort") sort->setSortMethod(sort->method), sort->update();
    else if (cmd == "sortBubble") sort->setSortMethod(1), sort->update();
    else if (cmd == "sortInsertion") sort->setSortMethod(2), sort->update();
    else if (cmd == "sortSelection") sort->setSortMethod(3), sort->update();
    else if (cmd == "sortQuick") sort->setSortMethod(4), sort->update();
    else if (cmd == "sortMerge") sort->setSortMethod(5), sort->update();
    else if (cmd == "sortHeap") sort->setSortMethod(6), sort->update();
    else if (cmd == "setAscending" && list.size() == 1)
        sort->setAscending(list[0] == "1" || list[0].toLower() == "true");
    else if (cmd == "autoRun" && list.size() == 1)
        sort->autoInterval = list[0].toInt(), sort->timer->start(sort->autoInterval), sort->autoRunning = true;
    else if (cmd == "stop") sort->timer->stop(), sort->autoRunning = false;
    else if (cmd == "reset") sort->reset();
    else if (cmd == "clear") sort->visualData.clear(), sort->update();
    else {
        QMessageBox::warning(nullptr, "DSL Error",
                             QString("Unknown command at line %1: %2").arg(lineNo).arg(cmd));
    }
}

// -------------------- DSL 高亮器 --------------------
SortDSLSyntaxHighlighter::SortDSLSyntaxHighlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent) {

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QBrush(QColor("purple")));
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywords = {
            "let","for","end",
            "insert","remove","setAscending",
            "autoRun","stop","reset","clear",
            "sort","sortBubble","sortInsertion","sortSelection","sortQuick","sortMerge","sortHeap"
    };

    for (const QString &kw : keywords)
        rules.append({QRegularExpression("\\b" + kw + "\\b"), keywordFormat});

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
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
