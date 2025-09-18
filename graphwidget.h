//
// Created by poorc on 2025/9/16.
//

#ifndef SIMULATOR_GRAPHWIDGET_H
#define SIMULATOR_GRAPHWIDGET_H

#endif //SIMULATOR_GRAPHWIDGET_H
#include <QWidget>

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};