//
// Created by poorc on 2025/10/5.
//

#ifndef SIMULATOR_HELPWINDOW_H
#define SIMULATOR_HELPWINDOW_H

// HelpWindow.h
#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QMessageBox>

class HelpWindow : public QWidget {
Q_OBJECT
public:
    explicit HelpWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("帮助中心");
        resize(800, 600);

        // 左右分割
        QSplitter *splitter = new QSplitter(this);

        // 左侧导航栏
        treeWidget = new QTreeWidget(splitter);
        treeWidget->setHeaderHidden(true);
        treeWidget->setMaximumWidth(220);

        // 一级菜单
        QTreeWidgetItem *homeItem = new QTreeWidgetItem(treeWidget);
        homeItem->setText(0, "首页");

        QTreeWidgetItem *sortItem = new QTreeWidgetItem(treeWidget);
        sortItem->setText(0, "排序模式");
        QTreeWidgetItem *sortDsl = new QTreeWidgetItem(sortItem);
        sortDsl->setText(0, "排序 DSL");

        QTreeWidgetItem *graphItem = new QTreeWidgetItem(treeWidget);
        graphItem->setText(0, "图模式");
        QTreeWidgetItem *graphDsl = new QTreeWidgetItem(graphItem);
        graphDsl->setText(0, "图 DSL");

        QTreeWidgetItem *aiItem = new QTreeWidgetItem(treeWidget);
        aiItem->setText(0, "AI 辅助");

        treeWidget->expandAll(); // 默认展开

        // 美化样式
        treeWidget->setStyleSheet(R"(
QTreeWidget {
    background-color: #2C3E50;
    color: #ECF0F1;
    font-size: 14px;
    border: none;
}
QTreeWidget::item {
    padding: 8px;
}
QTreeWidget::item:selected {
    background-color: #3498DB;
    color: white;
    border-radius: 5px;
}
QTreeWidget::item:hover {
    background-color: #2980B9;
}
)");

        // 右侧显示区域
        textEdit = new QTextEdit(splitter);
        textEdit->setReadOnly(true);

        // 布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(splitter);
        setLayout(layout);

        // 信号槽
        connect(treeWidget, &QTreeWidget::currentItemChanged,
                this, &HelpWindow::onItemSelected);

        // 默认选中首页
        treeWidget->setCurrentItem(homeItem);
    }

private slots:
    void onItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
        if (!current) return;
        QString text = current->text(0);

        if (text == "首页") {
            textEdit->setHtml("<h2>模拟器介绍</h2><p>这里是整体模拟器的介绍...</p>");
        } else if (text == "排序模式") {
            textEdit->setHtml("<h2>排序模式</h2><p>这里介绍排序模式的简单功能和使用方法...</p>");
        } else if (text == "排序 DSL") {
            textEdit->setHtml("<h2>排序 DSL</h2><p>这里介绍排序 DSL 的语法和用法...</p>");
        } else if (text == "图模式") {
            textEdit->setHtml("<h2>图模式</h2><p>这里介绍图模式的简单功能和使用方法...</p>");
        } else if (text == "图 DSL") {
            loadFromMd("./docs/GraphDSLManual.md");
        } else if (text == "AI 辅助") {
            textEdit->setHtml("<h2>AI 辅助</h2><p>这里介绍 AI 辅助功能...</p>");
        } else {
            textEdit->setHtml("<p>未知选项</p>");
        }
    }

    void loadFromMd(const QString &filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "无法打开文件：" + filePath);
            return;
        }
        QByteArray data = file.readAll();
        QString mdText = QString::fromUtf8(data);
        file.close();

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        textEdit->setMarkdown(mdText);
#else
        textEdit->setPlainText(mdText);
#endif
    }

private:
    QTreeWidget *treeWidget;
    QTextEdit *textEdit;
};



#endif //SIMULATOR_HELPWINDOW_H
