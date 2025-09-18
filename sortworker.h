////
//// Created by poorc on 2025/9/18.
////
//
//#ifndef SIMULATOR_SORTWORKER_H
//#define SIMULATOR_SORTWORKER_H
//
//#include <QObject>
//#include <QVector>
//#include <QThread>
//
//
//class SortWorker : public QObject {
//Q_OBJECT
//public:
//    explicit SortWorker(const QVector<int>& src, QObject* parent = nullptr);
//
//
//public slots:
//    void run(int method);
//
//
//signals:
//    void swapPerformed(int a, int b);
//    void valueChanged(int pos, int oldV, int newV);
//    void finished();
//
//    void mergeStateChanged(int left, int mid, int right,
//                           int i, int j, int write);
//    void heapSizeChanged(int size);
//
//
//private:
//    QVector<int> m_data;
//    bool shouldStop() const;
//    int mergeLeft, mergeMid, mergeRight, mergeI, mergeJ, mergeWrite;
//    int heapSize;
//// sorting implementations (cooperative cancellation inside)
//    void bubbleSort();
//    void insertionSort();
//    void selectionSort();
//    void quickSort(int l, int r);
//    int partition(int l, int r);
//    void mergeSort(int l, int r);
//    void merge(int l, int m, int r);
//    void heapSort();
//    void heapify(int n, int i);
//// utility: notify and small sleep to allow GUI refresh cadence
//    void notifySwap(int a, int b);
//    void notifyValueChange(int pos, int oldV, int newV);
//
//    void updateMergeState(int l, int m, int r, int i, int j, int write);
//
//    void updateHeapSize(int size);
//};
//
//
//#endif //SIMULATOR_SORTWORKER_H
