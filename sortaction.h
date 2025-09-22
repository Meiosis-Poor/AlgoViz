//
// Created by poorc on 2025/9/19.
//

#ifndef SIMULATOR_SORTACTION_H
#define SIMULATOR_SORTACTION_H

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

#endif //SIMULATOR_SORTACTION_H
