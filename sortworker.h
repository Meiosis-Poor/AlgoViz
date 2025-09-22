//
// Created by poorc on 2025/9/18.
//

#ifndef SIMULATOR_SORTWORKER_H
#define SIMULATOR_SORTWORKER_H

#include <QObject>
#include <QVector>
#include <QThread>
#include <QQueue>
#include "sortaction.h"


class SortWorker : public QObject {
public:
    explicit SortWorker(QObject *parent = nullptr);
    void init();
    void setData(QVector<int> mydata);
    SortAction takeAction();
    void ActionQueueclear();
    void doSwap(int a, int b);
    void BubbleSort();
    void InsertionSort();
    void SelectionSort();
    void QuickSort(int left, int right);
    int Partition(int left, int right);
    void MergeSort(int left, int right);
    void Merge(int left, int mid, int right);
    void HeapSort();
    void Heapify(int i, int n);

private:
    int heapSize = 0;
    QVector<int> data;
    QQueue<SortAction> ActionQueue;
};


#endif //SIMULATOR_SORTWORKER_H
