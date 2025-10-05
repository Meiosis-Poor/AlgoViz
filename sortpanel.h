//
// Created by poorc on 2025/10/6.
//

#ifndef SIMULATOR_SORTPANEL_H
#define SIMULATOR_SORTPANEL_H

#include "sortwidget.h"
#include <QVBoxLayout>

class SortPanel : public QWidget {
Q_OBJECT
public:
    SortPanel(QWidget *parent = nullptr) : QWidget(parent) {
        // 上方控制区
        controlPanel = new QWidget(this);


        // 下方绘图区
        sortArea = new SortWidget(this);

        // 调用原来的 setButton
        sortArea->setButton(controlPanel);
        controlPanel->setFixedHeight(90);

        // 垂直布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(controlPanel);
        layout->addWidget(sortArea, 1);
        layout->setStretch(0, 0);
        layout->setStretch(1, 1);
        layout->setContentsMargins(0, 0, 0, 0);

        setLayout(layout);
    }

    QWidget *controlPanel;
    SortWidget *sortArea;
};


#endif //SIMULATOR_SORTPANEL_H
