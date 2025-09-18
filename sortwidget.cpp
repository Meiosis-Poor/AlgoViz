#include "sortwidget.h"
#include "fileloader.h"
#include <QPainter>
#include <QThread>
#include <QPushButton>
#include <QCoreApplication>
#include <QMessageBox>

FileLoader *f=new FileLoader("./data/graph.txt");

SortWidget::SortWidget(QWidget *parent) : QWidget(parent) {
    QPushButton *lastbtn = new QPushButton("Last",this);
    lastbtn->move(10,10);
    connect(lastbtn, &QPushButton::clicked, this, [=](){
        if(SwapStack.empty()) return;
        LastAction=SwapStack.top();
        RebuildStack.push(LastAction);
        SwapStack.pop();
        if(LastAction.type==SortActionType::SetValue){
            visualData[LastAction.index1]=LastAction.oldValue;
        }
        else if(LastAction.type==SortActionType::Swap){
            std::swap(visualData[LastAction.index1], visualData[LastAction.index2]);
        }
        update();
    });
    QPushButton *nextbtn = new QPushButton("Next",this);
    nextbtn->move(100,10);
    connect(nextbtn, &QPushButton::clicked, this, [=](){
        if(RebuildStack.empty()) {
            if(ActionQueue.empty()) return;
            LastAction=ActionQueue.front();
            ActionQueue.pop_front();
        }
        else{
            LastAction=RebuildStack.top();
            RebuildStack.pop();
        }
        SwapStack.push(LastAction);
        if(LastAction.type==SortActionType::SetValue){
            visualData[LastAction.index1]=LastAction.newValue;
        }
        else if(LastAction.type==SortActionType::Swap){
            std::swap(visualData[LastAction.index1], visualData[LastAction.index2]);
        }
        update();
        return;
    });
    QPushButton *sortbtn = new QPushButton("Please Choose a Sort Method",this);
    sortbtn->move(200,10);
    connect(sortbtn, &QPushButton::clicked, this, [=](){
        if(!isFinished) QMessageBox::warning(this, "Warning", "Please wait for the last sorting to finish.");
        method++;
        method%=7;
        if(method==0) method=1;
        inint();
        setSortbtnText(method, sortbtn);
        update();
        setSortMethod(method);
    });
    QPushButton *autobtn = new QPushButton("Auto",this);
    autobtn->move(500,10);
    connect(autobtn, &QPushButton::clicked, this, [=](){
        if(!isFinished) return;

    });
}

void SortWidget::inint(){
    isFinished=true;
    LastAction=SortAction();
    SwapStack.clear();
    RebuildStack.clear();
    ActionQueue.clear();
}


void SortWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (visualData.isEmpty()) return;

    int n = visualData.size();
    int w = (width()*2/3) / n;                   // 每条柱的宽度
    int maxVal = *std::max_element(visualData.begin(), visualData.end());
    if(method==1 || method==2 || method==3 || method==4) paintBars(p, n, w, maxVal);
    else if(method==5) paintMerge(p, n, w, maxVal);
    else if(method==6) paintHeap(p, n, w, maxVal);

}

void SortWidget::paintBars(QPainter &p, int n, int w, int maxVal) {
    for (int i = 0; i < n; i++) {
        // 高度按比例映射到窗口高度
        int h = static_cast<int>((double)visualData[i] / maxVal * (height()*2/3 ));
        int x = i * w+25;
        int y = height()*4/5 - h;

        // 高亮当前交换的两条柱
        if(LastAction.index1 == i || LastAction.index2 == i){
            p.setBrush(Qt::red);
        } else {
            p.setBrush(Qt::blue);
        }

        p.drawRect(x, y, w - 2, h);   // 绘制柱状条
        // 绘制柱子上方的数值
        p.setPen(Qt::yellow); // 设置文字颜色
        p.drawText(x+w/2-5, y - 5, QString::number(visualData[i]));
    }
}

void SortWidget::paintMerge(QPainter &p, int n, int w, int maxVal) {
    for (int i = 0; i < n; i++) {
        int h = (double)visualData[i] / maxVal * (height()*2/3);
        int x = i * w + 25;
        int y = height()*4/5 - h;

        if (i >= LastAction.mergeLeft && i <= LastAction.mergeMid) p.setBrush(Qt::blue);   // 左区间
        else if (i > LastAction.mergeMid && i <= LastAction.mergeRight) p.setBrush(Qt::green); // 右区间
        else p.setBrush(Qt::gray);

        if (i == LastAction.mergeI || i == LastAction.mergeJ) p.setBrush(Qt::red);  // 当前比较
        if (i == LastAction.mergeWrite) p.setBrush(Qt::yellow);          // 正在写入

        p.drawRect(x, y, w - 2, h);
        p.drawText(x+w/2-5, y-5, QString::number(visualData[i]));
    }
}

void SortWidget::paintHeap(QPainter &p, int n, int, int) {
    if (n == 0) return;

    int nodeSize = 30;
    for (int i = 0; i < n; i++) {
        int level = (int)log2(i+1);
        int nodesInLevel = 1 << level;
        int pos = i - nodesInLevel + 1;
        int spacing = width() / (nodesInLevel + 1);

        int x = (pos+1) * spacing;
        int y = 50 + level * 70;

        // 画连线
        if (i > 0) {
            int parent = (i-1)/2;
            int parentLevel = (int)log2(parent+1);
            int parentNodes = 1 << parentLevel;
            int parentPos = parent - parentNodes + 1;
            int parentSpacing = width() / (parentNodes + 1);
            int parentX = (parentPos+1) * parentSpacing;
            int parentY = 50 + parentLevel * 70;
            p.drawLine(parentX, parentY, x, y);
        }

        // 节点颜色
        if (i == LastAction.index1 || i == LastAction.index2) p.setBrush(Qt::red);
        else if (i >= LastAction.heapSize) p.setBrush(Qt::gray);  // 已移出
        else p.setBrush(Qt::green);

        QRect rect(x - nodeSize/2, y - nodeSize/2, nodeSize, nodeSize);
        p.drawEllipse(rect);
        p.drawText(rect, Qt::AlignCenter, QString::number(visualData[i]));
    }
}



void SortWidget::setSortbtnText(int method, QPushButton* sortbtn){
    isFinished=false;
    switch(method){
        case 1: sortbtn->setText("Bubble Sort"); break;
        case 2: sortbtn->setText("Insertion Sort"); break;
        case 3: sortbtn->setText("Selection Sort"); break;
        case 4: sortbtn->setText("Quick Sort"); break;
        case 5: sortbtn->setText("Merge Sort"); break;
        case 6: sortbtn->setText("Heap Sort"); break;
        default: sortbtn->setText("Please Choose a Sort Method"); break;
    }
}


void SortWidget::setSortMethod(int method){
    data=f->readArray1D();
    visualData=data;
    switch(method) {
        case 1: BubbleSort(); break;
        case 2: InsertionSort(); break;
        case 3: SelectionSort(); break;
        case 4: QuickSort(0, data.size() - 1); break;
        case 5: MergeSort(0, data.size() - 1); break;
        case 6: HeapSort(); break;
        default: break;
    }
}

void SortWidget::doSwap(int a, int b) {
    SortAction action(a, b,heapSize);
    std::swap(data[a], data[b]);
    ActionQueue.push_back(action);
    return;
}

void SortWidget::BubbleSort() {
    int len = data.size();
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                doSwap(j, j + 1);
            }
        }
    }
    isFinished=true;
    return;
}

void SortWidget::InsertionSort() {
    int len = data.size();
    for (int i = 1; i < len; i++) {
        int j = i;
        while (j > 0 && data[j - 1] > data[j]) {
            doSwap(j - 1, j);
            j--;
        }
    }
    isFinished = true;
    return;
}


void SortWidget::SelectionSort() {
    int len = data.size();
    for (int i = 0; i < len - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < len; j++) {
            if (data[j] < data[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex!= i) {
            doSwap(i, minIndex);
        }
    }
    isFinished = true;
    return;
}

void SortWidget::QuickSort(int left, int right) {
    if (left < right) {
        int pivot = Partition(left, right);
        QuickSort(left, pivot - 1);
        QuickSort(pivot + 1, right);
    }
    if(left==0 && right==data.size()-1) isFinished=true;
    return;
}

int SortWidget::Partition(int left, int right) {
    int pivot = data[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (data[j] < pivot) {
            i++;
            doSwap(i, j);
        }
    }
    if(i+1!=right){
        doSwap(i+1, right);
    }
    return i + 1;
}

void SortWidget::MergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        MergeSort(left, mid);
        MergeSort(mid + 1, right);
        Merge(left, mid, right);
    }
    if(left == 0 && right == data.size() - 1) isFinished = true;
    return;
}

void SortWidget::Merge(int left, int mid, int right) {
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

        if (data[i] <= data[j]) temp[k++] = data[i++];
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


void SortWidget::HeapSort() {
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
    isFinished = true;
    return;
}

void SortWidget::Heapify(int len, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < len && data[l] > data[largest]) largest = l;
    if (r < len && data[r] > data[largest]) largest = r;
    if (largest != i) {
        doSwap(i, largest);
        Heapify(len, largest);
    }
    return;
}


