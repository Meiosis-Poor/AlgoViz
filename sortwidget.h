//
// Created by poorc on 2025/9/16.
//

#include <QWidget>
#include <QStack>
#include <QPushButton>
#include <QQueue>
#include <QTimer>
#include <QLabel>
#include <QMap>
#include <QRect>
#include <QPaintEvent>
#include <QMouseEvent>
#include "sortaction.h"
#include "sortworker.h"
#include "fileloader.h"

#ifndef SIMULATOR_SORTWIDGET_H
#define SIMULATOR_SORTWIDGET_H


class SortWidget : public QWidget {

public:
    QVector<int> visualData;

    explicit SortWidget(QWidget *parent = nullptr);
    void inint();
    void reset();
    void setSortMethod(int method);
    void setSortbtnText(int method, QPushButton* sortbtn);
    void paintBars(QPainter& p, int n, int w, int maxVal);
    void paintMerge(QPainter& p, int n, int w, int maxVal);
    void paintHeap(QPainter& p, int n, int w, int maxVal);
    void setButton(QWidget* panel);
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);
    void randomize();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    int method=0;
    bool isFinished=true;
    int autoInterval=300;
    bool autoRunning=false;
    int highlightIndex=-1;
    QString lastfileName;
    FileLoader *f=new FileLoader();
    SortWorker* worker=new SortWorker();

    SortAction LastAction;
    QStack<SortAction> SwapStack;
    QStack<SortAction> RebuildStack;
    QTimer* timer=new QTimer();
    QMap<int, QRect> barRects;
    QMap<int, QRect> heapRects;

    slots void onLastBtnClicked();
    slots void onNextBtnClicked();
};

#endif //SIMULATOR_SORTWIDGET_H