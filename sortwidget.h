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
#include <QComboBox>
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
    void setAscending(bool ascending);
    void setSortMethod(int m);
    void setSortbtnText(int m, QComboBox* sortcombo);
    void paintBars(QPainter& p, int n, int w, int maxVal);
    void paintMerge(QPainter& p, int n, int w, int maxVal);
    void paintHeap(QPainter& p, int n, int w, int maxVal);
    void setButton(QWidget* panel);
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);
    void randomize();
    int method=0;
    int autoInterval=300;
    bool autoRunning=false;
    QTimer* timer=new QTimer();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:

    bool isFinished=true;
    int highlightIndex=-1;
    QString lastfileName;
    FileLoader *f=new FileLoader();
    SortWorker* worker=new SortWorker();

    SortAction LastAction;
    QStack<SortAction> SwapStack;
    QStack<SortAction> RebuildStack;

    QMap<int, QRect> barRects;
    QMap<int, QRect> heapRects;

    slots void onLastBtnClicked();
    slots void onNextBtnClicked();
};

#endif //SIMULATOR_SORTWIDGET_H