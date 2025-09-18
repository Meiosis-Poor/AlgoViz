#include "GraphWidget.h"
#include <QPainter>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent) {}

void GraphWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.drawEllipse(50, 50, 40, 40);
}