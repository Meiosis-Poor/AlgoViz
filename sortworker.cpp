//#include "sortworker.h"
//#include <QThread>
//#include <QDebug>
//
//SortWorker::SortWorker(const QVector<int>& src, QObject* parent)
//        : QObject(parent), m_data(src), heapSize(src.size()),
//          mergeLeft(0), mergeMid(0), mergeRight(0),
//          mergeI(-1), mergeJ(-1), mergeWrite(-1) {}
//
//// ===== 检查线程中断 =====
//bool SortWorker::shouldStop() const {
//    QThread* t = QThread::currentThread();
//    return t && t->isInterruptionRequested();
//}
//
//// ===== 信号通知 =====
//void SortWorker::notifySwap(int a, int b) {
//    emit swapPerformed(a, b);
//    QThread::msleep(60);
//}
//
//void SortWorker::notifyValueChange(int pos, int oldV, int newV) {
//    if (QThread::currentThread()->isInterruptionRequested()) return;
//    emit valueChanged(pos, oldV, newV);  // 会传给 SortWidget
//    QThread::msleep(60);  // 控制可视化速度
//}
//
//
//
//// 更新归并排序状态
//void SortWorker::updateMergeState(int l, int m, int r, int i, int j, int write) {
//    mergeLeft = l;
//    mergeMid = m;
//    mergeRight = r;
//    mergeI = i;
//    mergeJ = j;
//    mergeWrite = write;
//    emit mergeStateChanged(mergeLeft, mergeMid, mergeRight,
//                           mergeI, mergeJ, mergeWrite);
//}
//
//// 更新堆排序状态
//void SortWorker::updateHeapSize(int size) {
//    heapSize = size;
//    emit heapSizeChanged(heapSize);
//}
//
//// ===== 运行指定排序 =====
//void SortWorker::run(int method) {
//    switch (method) {
//        case 1: bubbleSort(); break;
//        case 2: insertionSort(); break;
//        case 3: selectionSort(); break;
//        case 4: quickSort(0, m_data.size() - 1); break;
//        case 5: mergeSort(0, m_data.size() - 1); break;
//        case 6: heapSort(); break;
//        default: break;
//    }
//    emit finished();
//}
//
//// ===== 排序算法 =====
//void SortWorker::bubbleSort() {
//    int n = m_data.size();
//    for (int i = 0; i < n-1; ++i) {
//        for (int j = 0; j < n-i-1; ++j) {
//            if (shouldStop()) return;
//            if (m_data[j] > m_data[j+1])
//                notifySwap(j, j+1);
//        }
//    }
//}
//
//void SortWorker::insertionSort() {
//    int n = m_data.size();
//    for (int i = 1; i < n; ++i) {
//        int j = i;
//        while (j > 0 && m_data[j-1] > m_data[j]) {
//            if (shouldStop()) return;
//            notifySwap(j-1, j);
//            --j;
//        }
//    }
//}
//
//void SortWorker::selectionSort() {
//    int n = m_data.size();
//    for (int i = 0; i < n-1; ++i) {
//        int minIdx = i;
//        for (int j = i+1; j < n; ++j) {
//            if (shouldStop()) return;
//            if (m_data[j] < m_data[minIdx]) minIdx = j;
//        }
//        if (minIdx != i) notifySwap(i, minIdx);
//    }
//}
//
//// ===== Quick Sort =====
//void SortWorker::quickSort(int l, int r) {
//    if (l >= r || shouldStop()) return;
//    int p = partition(l, r);
//    if (shouldStop()) return;
//    quickSort(l, p-1);
//    if (shouldStop()) return;
//    quickSort(p+1, r);
//}
//
//int SortWorker::partition(int l, int r) {
//    int pivot = m_data[r];
//    int i = l - 1;
//    for (int j = l; j < r; ++j) {
//        if (shouldStop()) return r;
//        if (m_data[j] < pivot) {
//            ++i;
//            notifySwap(i, j);
//        }
//    }
//    if (i+1 != r) notifySwap(i+1, r);
//    return i+1;
//}
//
//// ===== Merge Sort =====
//void SortWorker::mergeSort(int l, int r) {
//    if (l >= r || shouldStop()) return;
//    int m = (l + r) / 2;
//    mergeSort(l, m);
//    if (shouldStop()) return;
//    mergeSort(m+1, r);
//    if (shouldStop()) return;
//    merge(l, m, r);
//}
//
//void SortWorker::merge(int l, int m, int r) {
//    QVector<int> tmp(r - l + 1);
//    int i = l, j = m+1, k = 0;
//
//    mergeLeft = l;
//    mergeMid = m;
//    mergeRight = r;
//
//    while (i <= m && j <= r) {
//        if (shouldStop()) return;
//
//        mergeI = i;
//        mergeJ = j;
//        mergeWrite = -1;
//        emit mergeStateChanged(mergeLeft, mergeMid, mergeRight, mergeI, mergeJ, mergeWrite);
//
//        if (m_data[i] <= m_data[j]) tmp[k++] = m_data[i++];
//        else tmp[k++] = m_data[j++];
//    }
//    while (i <= m) { tmp[k++] = m_data[i++]; }
//    while (j <= r) { tmp[k++] = m_data[j++]; }
//
//    for (int t = 0; t < k; ++t) {
//        int pos = l + t;
//        int oldV = m_data[pos];
//        int newV = tmp[t];
//        notifyValueChange(pos, oldV, newV);
//        m_data[pos] = newV;  // 更新线程内部数据
//    }
//
//}
//
//// ===== Heap Sort =====
//void SortWorker::heapSort() {
//    int n = m_data.size();
//    for (int i = n/2-1; i >= 0; --i) {
//        if (shouldStop()) return;
//        updateHeapSize(n);
//        heapify(n, i);
//    }
//    for (int i = n-1; i > 0; --i) {
//        if (shouldStop()) return;
//        notifySwap(0, i);
//        updateHeapSize(i);
//        heapify(i, 0);
//    }
//}
//
//void SortWorker::heapify(int n, int i) {
//    int largest = i;
//    int l = 2*i + 1;
//    int r = 2*i + 2;
//
//    if (l < n && m_data[l] > m_data[largest]) largest = l;
//    if (r < n && m_data[r] > m_data[largest]) largest = r;
//
//    if (largest != i) {
//        notifySwap(i, largest);
//        heapify(n, largest);
//    }
//}
