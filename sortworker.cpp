#include "sortworker.h"

SortWorker::SortWorker(QObject *parent) {
    data.clear();
    heapSize = 0;
    return;
}

void SortWorker::init(){
    data.clear();
    heapSize = 0;
    ActionQueue.clear();
    return;
}

void SortWorker::setData(QVector<int> mydata) {
    data = mydata;
    return;
}

SortAction SortWorker::takeAction() {
    if(ActionQueue.empty()) return SortAction(-1,-1);
    auto act = ActionQueue.front();
    ActionQueue.pop_front();
    return act;
}

void SortWorker::ActionQueueclear(){
    ActionQueue.clear();
    return;
}


void SortWorker::doSwap(int a, int b) {
    SortAction action(a, b,heapSize);
    std::swap(data[a], data[b]);
    ActionQueue.push_back(action);
    return;
}

bool SortWorker::cmp(int a, int b){
    return ascending ? (a > b) : (a < b);
}


void SortWorker::BubbleSort() {
    int len = data.size();
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (cmp(data[j],data[j + 1])) {
                doSwap(j, j + 1);
            }
        }
    }
    return;
}

void SortWorker::InsertionSort() {
    int len = data.size();
    for (int i = 1; i < len; i++) {
        int j = i;
        while (j > 0 && cmp(data[j - 1],data[j])) {
            doSwap(j - 1, j);
            j--;
        }
    }
    return;
}


void SortWorker::SelectionSort() {
    int len = data.size();
    for (int i = 0; i < len - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < len; j++) {
            if (cmp(data[minIndex],data[j])) {
                minIndex = j;
            }
        }
        if (minIndex!= i) {
            doSwap(i, minIndex);
        }
    }
    return;
}

void SortWorker::QuickSort(int left, int right) {
    if (left < right) {
        int pivot = Partition(left, right);
        QuickSort(left, pivot - 1);
        QuickSort(pivot + 1, right);
    }
    return;
}

int SortWorker::Partition(int left, int right) {
    int pivot = data[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (cmp(pivot,data[j])) {
            i++;
            doSwap(i, j);
        }
    }
    if(i+1!=right){
        doSwap(i+1, right);
    }
    return i + 1;
}

void SortWorker::MergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        MergeSort(left, mid);
        MergeSort(mid + 1, right);
        Merge(left, mid, right);
    }
    return;
}

void SortWorker::Merge(int left, int mid, int right) {
    QVector<int> temp(right - left + 1);
    int i = left;
    int j = mid + 1;
    int k = 0;

    int mergeLeft = left;
    int mergeMid = mid;
    int mergeRight = right;
    int mergeI = -1;
    int mergeJ = -1;
    int mergeWrite = -1;

    while (i <= mid && j <= right) {
        mergeI = i;
        mergeJ = j;
        mergeWrite = left + k;

        if (!cmp(data[i],data[j])) temp[k++] = data[i++];
        else temp[k++] = data[j++];
    }

    while (i <= mid) {
        mergeI = i;
        mergeJ = -1;
        mergeWrite = left + k;
        temp[k++] = data[i++];
    }

    while (j <= right) {
        mergeI = -1;
        mergeJ = j;
        mergeWrite = left + k;
        temp[k++] = data[j++];
    }

    for (int m = 0; m < k; m++) {
        int pos = left + m;
        mergeWrite = pos;
        SortAction action(pos,data[pos],temp[m],mergeLeft,mergeMid,mergeRight,mergeI,mergeJ,mergeWrite);
        ActionQueue.push_back(action);
        data[pos] = temp[m];
    }
    return;
}


void SortWorker::HeapSort() {
    int len = data.size();
    heapSize = len;
    for (int i = len / 2 - 1; i >= 0; i--) {
        Heapify(len, i);
    }
    for (int i = len - 1; i >0; i--) {
        doSwap(0, i);        // 交换堆顶和最后一个元素
        heapSize = i;        // 调整堆大小
        Heapify(i, 0);       // 调整
    }
    return;
}

void SortWorker::Heapify(int len, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < len && cmp(data[l],data[largest])) largest = l;
    if (r < len && cmp(data[r],data[largest])) largest = r;
    if (largest != i) {
        doSwap(i, largest);
        Heapify(len, largest);
    }
    return;
}