//
// Created by poorc on 2025/12/8.
//

#ifndef SIMULATOR_DSLCOMPLETER_H
#define SIMULATOR_DSLCOMPLETER_H

#include <QEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QKeyEvent>
#include <QTextEdit>
#include <QCompleter>
#include <QStringListModel>

class DSLCompleter : public QObject {
Q_OBJECT
public:
    DSLCompleter(QTextEdit *editor, const QStringList &keywords, QObject *parent = nullptr)
            : QObject(parent), editor(editor)
    {
        completer = new QCompleter(keywords, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);
        completer->setFilterMode(Qt::MatchStartsWith);
        completer->setWidget(editor);

        connect(completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
        editor->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj != editor) return false;

        // ============ KeyPress：只负责拦截确定类按键 ============
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            int k = keyEvent->key();

            // 插入确认类
            if (k == Qt::Key_Enter || k == Qt::Key_Return ||
                k == Qt::Key_Tab   || k == Qt::Key_Escape)
            {
                if (completer->popup()->isVisible()) {
                    completer->popup()->hide();
                    return true;  // 拦截，不让 TextEdit 处理
                }
            }

            justCompleted = false;
            return false; // 正常让 QTextEdit 先输入文字
        }

        // ============ KeyRelease：用于实时刷新补全 ============
        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            // 不是字符键就不处理
            QString t = keyEvent->text();
            if (t.isEmpty() || !t[0].isLetterOrNumber()) {
                completer->popup()->hide();
                return false;
            }

            if (justCompleted) {
                justCompleted = false;
                return false;
            }

            QString prefix = textUnderCursor();
            if (prefix.isEmpty()) {
                completer->popup()->hide();
                return false;
            }

            // === 强制刷新模型 ===
            completer->setCompletionPrefix(prefix);
            completer->completionModel()->sort(0);   // 关键步骤：刷新模型
            completer->popup()->setCurrentIndex(
                    completer->completionModel()->index(0, 0)
            );

            if (completer->completionCount() > 0) {
                QRect cr = editor->cursorRect();
                cr.setWidth(completer->popup()->sizeHintForColumn(0)
                            + completer->popup()->verticalScrollBar()->sizeHint().width());
                completer->complete(cr);  // ? 弹出
            }
            else {
                completer->popup()->hide();
            }
        }

        return false;
    }

private slots:
    void insertCompletion(const QString &completion) {
        QString prefix = textUnderCursor();
        QTextCursor tc = editor->textCursor();
        tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, textUnderCursor().length());
        tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, textUnderCursor().length());
        tc.insertText(completion);
        editor->setTextCursor(tc);

        completer->popup()->hide(); // 插入后隐藏
        justCompleted = true;
    }


private:
    QString textUnderCursor() const {
        QTextCursor tc = editor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        return tc.selectedText();
    }

    QTextEdit *editor;
    QCompleter *completer;
    bool justCompleted = false;
};


#endif //SIMULATOR_DSLCOMPLETER_H
