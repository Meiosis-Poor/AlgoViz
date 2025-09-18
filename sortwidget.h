//
// Created by poorc on 2025/9/16.
//

#include <QWidget>
#include <QStack>
#include <QPushButton>
#include <QQueue>

#ifndef SIMULATOR_SORTWIDGET_H
#define SIMULATOR_SORTWIDGET_H

#endif //SIMULATOR_SORTWIDGET_H

enum class SortActionType {
    Swap,       // 交换两个元素
    SetValue    // 单个位置赋值（如归并排序写入）
};

struct SortAction {
    SortActionType type;  // 操作类型
    int index1;           // Swap: 交换下标1 / SetValue: 写入下标
    int index2;           // Swap: 交换下标2 / SetValue: -1
    int oldValue;         // 写入前的值（SetValue用）
    int newValue;         // 写入后的值（SetValue用）

    // 额外状态变量，用于可视化
    int heapSize;         // 当前堆大小（HeapSort用）
    int mergeLeft;        // 当前归并左区间
    int mergeMid;         // 当前归并中间点
    int mergeRight;       // 当前归并右区间
    int mergeI;           // 当前左指针
    int mergeJ;           // 当前右指针
    int mergeWrite;       // 当前写入位置

    // 默认构造函数
    SortAction()
            : type(SortActionType::Swap), index1(-1), index2(-1), oldValue(0), newValue(0),
              heapSize(0), mergeLeft(-1), mergeMid(-1), mergeRight(-1), mergeI(-1), mergeJ(-1), mergeWrite(-1) {}

    // 构造函数：交换
    SortAction(int i, int j, int heapSz = 0)
            : type(SortActionType::Swap), index1(i), index2(j), oldValue(0), newValue(0),
              heapSize(heapSz), mergeLeft(-1), mergeMid(-1), mergeRight(-1),
              mergeI(-1), mergeJ(-1), mergeWrite(-1) {}

    // 构造函数：赋值
    SortAction(int i, int oldVal, int newVal, int l, int m, int r, int mi, int mj, int mw)
            : type(SortActionType::SetValue), index1(i), index2(-1), oldValue(oldVal), newValue(newVal),
              heapSize(0), mergeLeft(l), mergeMid(m), mergeRight(r), mergeI(mi), mergeJ(mj), mergeWrite(mw) {}
};



class SortWidget : public QWidget {

public:
    QVector<int> data;
    QVector<int> visualData;

    explicit SortWidget(QWidget *parent = nullptr);
    void inint();
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
    void setSortMethod(int method);
    void setSortbtnText(int method, QPushButton* sortbtn);
    void paintBars(QPainter& p, int n, int w, int maxVal);
    void paintMerge(QPainter& p, int n, int w, int maxVal);
    void paintHeap(QPainter& p, int n, int w, int maxVal);
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    int method=0;
    bool isFinished=true;

    SortAction LastAction;
    QStack<SortAction> SwapStack;
    QStack<SortAction> RebuildStack;
    QQueue<SortAction> ActionQueue;

    int heapSize = 0;
};