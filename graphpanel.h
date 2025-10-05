//
// Created by poorc on 2025/10/6.
//

#ifndef SIMULATOR_GRAPHPANEL_H
#define SIMULATOR_GRAPHPANEL_H

#include "graphwidget.h"
#include <QVBoxLayout>

class GraphPanel : public QWidget {
Q_OBJECT
public:
    GraphPanel(QWidget *parent = nullptr) : QWidget(parent) {
        // 上方控制区
        controlPanel = new QWidget(this);


        // 绘图区
        graphArea = new GraphWidget(this);

        // 调用原来的 setButton，把按钮放到 controlPanel
        graphArea->setButton(controlPanel);
        controlPanel->setFixedHeight(90);

        // 整体垂直布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(controlPanel);
        layout->addWidget(graphArea, 1); // 绘图区自适应剩余空间
        layout->setStretch(0, 0);        // 工具栏固定
        layout->setStretch(1, 1);        // 绘图区拉伸
        layout->setContentsMargins(0, 0, 0, 0);

        setLayout(layout);
    }
    QWidget *controlPanel;
    GraphWidget *graphArea;
};


#endif //SIMULATOR_GRAPHPANEL_H
