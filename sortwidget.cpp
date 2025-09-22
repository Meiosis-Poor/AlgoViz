#include "sortwidget.h"
#include <QPainter>
#include <QPushButton>
#include <QCoreApplication>
#include <QMessageBox>
#include <QSlider>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QIntValidator>

SortWidget::SortWidget(QWidget *parent) : QWidget(parent) {
    setButton(this);
    //setSlider();
}

void SortWidget::setButton(QWidget* panel) {
    connect(timer, &QTimer::timeout, this, &SortWidget::onNextBtnClicked);

    // ==== 创建按钮 ====
    QPushButton *lastbtn   = new QPushButton("Last", panel);
    QPushButton *nextbtn   = new QPushButton("Next", panel);
    QPushButton *sortbtn   = new QPushButton("Please Choose a Sort Method", panel);
    QPushButton *autobtn   = new QPushButton("Auto", panel);
    QPushButton *insertbtn = new QPushButton("Insert", panel);
    QPushButton *removebtn = new QPushButton("Remove", panel);
    QPushButton *updatebtn = new QPushButton("Update", panel);
    QPushButton *findbtn   = new QPushButton("Find", panel);

    // ==== 创建滑动条 ====
    QSlider *speedSlider = new QSlider(Qt::Horizontal, panel);
    speedSlider->setRange(100, 999);
    speedSlider->setValue(autoInterval);
    QLabel *speedLabel = new QLabel(QString("Speed: %1 ms").arg(autoInterval), panel);

    QGridLayout *grid = new QGridLayout(panel);
    grid->setAlignment(Qt::AlignTop); // 顶部对齐
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(5);
    // 设置前三个按钮拉长
    QSizePolicy longPolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    lastbtn->setSizePolicy(longPolicy);
    nextbtn->setSizePolicy(longPolicy);
    findbtn->setSizePolicy(longPolicy);
    speedSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    // 第一排按钮
    grid->addWidget(lastbtn, 0, 0);
    grid->addWidget(nextbtn, 0, 1);
    grid->addWidget(findbtn, 0, 2);
    grid->addWidget(speedSlider, 0, 4);
    // 第二排按钮
    grid->addWidget(insertbtn, 1, 0);
    grid->addWidget(removebtn, 1, 1);
    grid->addWidget(updatebtn, 1, 2);
    grid->addWidget(speedLabel, 1, 4);

    // Sort\auto 按钮占两行
    autobtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    grid->addWidget(autobtn, 0, 3, 2, 1);
    sortbtn->setMinimumWidth(200);
    sortbtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    grid->addWidget(sortbtn, 0, 5, 2, 1); // 从 (0,5) 开始，占 2 行 1 列
    grid->setRowStretch(3, 1);
    panel->setLayout(grid);

    connect(speedSlider, &QSlider::valueChanged, panel, [=](int value) mutable {
        autoInterval = value;
        if (autoRunning) {
            timer->start(autoInterval); // 更新定时器
        }
        speedLabel->setText(QString("Speed: %1 ms").arg(value)); // 更新 Label
    });

    connect(lastbtn, &QPushButton::clicked, this, [=](){ onLastBtnClicked(); });

    connect(nextbtn, &QPushButton::clicked, this, [=](){ onNextBtnClicked(); });

    connect(sortbtn, &QPushButton::clicked, this, [=](){
        if(!isFinished) {
            QMessageBox::warning(this, "Warning", "Please wait for the last sorting to finish.");
        }
        method++;
        method %= 7;
        if(method == 0) method = 1;
        inint();
        setSortbtnText(method, sortbtn);
        update();
        setSortMethod(method);
    });

    connect(autobtn, &QPushButton::clicked, this, [=](){
        if (autoRunning) {
            timer->stop();
            autoRunning = false;
        } else {
            timer->start(autoInterval);
            autoRunning = true;
        }
    });

    connect(insertbtn, &QPushButton::clicked, this, [=]() {
        QDialog dialog(this);
        dialog.setWindowTitle("Insert Node");
        dialog.setFixedSize(300, 150);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        // 插入位置输入框
        QHBoxLayout *posLayout = new QHBoxLayout;
        QLabel *posLabel = new QLabel("Position:", &dialog);
        QLineEdit *posEdit = new QLineEdit(&dialog);
        posEdit->setValidator(new QIntValidator(0, visualData.size(), posEdit));
        posLayout->addWidget(posLabel);
        posLayout->addWidget(posEdit);

        // 插入值输入框
        QHBoxLayout *valLayout = new QHBoxLayout;
        QLabel *valLabel = new QLabel("Value:", &dialog);
        QLineEdit *valEdit = new QLineEdit(&dialog);
        valEdit->setValidator(new QIntValidator(-100, 100, valEdit));
        valLayout->addWidget(valLabel);
        valLayout->addWidget(valEdit);

        // 确定 & 取消按钮
        QHBoxLayout *btnLayout = new QHBoxLayout;
        QPushButton *okBtn = new QPushButton("OK", &dialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);

        layout->addLayout(posLayout);
        layout->addLayout(valLayout);
        layout->addLayout(btnLayout);

        QObject::connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        QObject::connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

        // 执行对话框
        if (dialog.exec() == QDialog::Accepted) {
            int pos = posEdit->text().toInt();
            int value = valEdit->text().toInt();

            if (pos >= 0 && pos <= visualData.size()) {
                visualData.insert(pos, value);
                highlightIndex = pos;   // 高亮新插入的节点
                inint();
                setSortMethod(method);
                QMessageBox::information(this, "Information", "Node inserted successfully.");
                update();
            } else {
                QMessageBox::warning(this, "Warning", "Invalid position.");
            }
        }
    });

    connect(removebtn, &QPushButton::clicked, this, [=]() {
        if(highlightIndex==-1) QMessageBox::warning(this, "Warning", "Please select a node to remove.");
        else{
            visualData.remove(highlightIndex);
            inint();
            setSortMethod(method);
            QMessageBox::information(this, "Information", "The node has been removed.");
            update();
        }
    });

    connect(updatebtn, &QPushButton::clicked, this, [=]() {
        QDialog dialog(this);
        dialog.setWindowTitle("Update Node");
        dialog.setFixedSize(300, 150);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        // 新值输入框
        QHBoxLayout *valLayout = new QHBoxLayout;
        QLabel *valLabel = new QLabel("New Value:", &dialog);
        QLineEdit *valEdit = new QLineEdit(&dialog);
        valEdit->setValidator(new QIntValidator(-100, 100, valEdit));
        valLayout->addWidget(valLabel);
        valLayout->addWidget(valEdit);

        // 确定 & 取消
        QHBoxLayout *btnLayout = new QHBoxLayout;
        QPushButton *okBtn = new QPushButton("OK", &dialog);
        QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);

        layout->addLayout(valLayout);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            int pos = highlightIndex;
            int newVal = valEdit->text().toInt();
            if (pos >= 0 && pos < visualData.size()) {
                visualData[pos] = newVal;
                highlightIndex = pos;
                inint();
                setSortMethod(method);
                QMessageBox::information(this, "Information", "Node updated successfully.");
                update();
            } else {
                QMessageBox::warning(this, "Warning", "Invalid position.");
            }
        }
    });

    connect(findbtn, &QPushButton::clicked, this, [=]() {
        bool ok;
        QString text = QInputDialog::getText(this, "Find Value",
                                             "Enter value to find:", QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            bool isNumber;
            int value = text.toInt(&isNumber);
            if (isNumber) {
                int idx = visualData.indexOf(value);
                if (idx != -1) {
                    highlightIndex = idx;
                    QMessageBox::information(this, "Found", QString("Value %1 found at position %2.").arg(value).arg(idx));
                    update();
                } else {
                    QMessageBox::information(this, "Not Found", QString("Value %1 not found.").arg(value));
                }
            } else {
                QMessageBox::warning(this, "Warning", "Please enter a valid integer.");
            }
        }
    });
}


void SortWidget::onLastBtnClicked(){
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
}

void SortWidget::onNextBtnClicked() {
    if(RebuildStack.empty()) {
        SortAction tempaction=worker->takeAction();
        if(tempaction.index1==-1 && tempaction.index2==-1) return;
        LastAction=tempaction;
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
}

void SortWidget::inint(){
    isFinished=true;
    LastAction=SortAction();
    SwapStack.clear();
    RebuildStack.clear();
    barRects.clear();
    heapRects.clear();
    highlightIndex=-1;
}

void SortWidget::reset() {
    inint();
    visualData.clear();
    worker->init();
    setSortMethod(method);
    update();
}

void SortWidget::mousePressEvent(QMouseEvent *event) {
    QPoint pos = event->pos();

    // 检查柱状条点击
    for (auto it = barRects.begin(); it != barRects.end(); ++it) {
        if (it.value().contains(pos)) {
            int index = it.key();
            qDebug() << "Bar clicked at index:" << index;
            highlightIndex=index;
            update();
            return;
        }
    }

    // 检查堆节点点击
    for (auto it = heapRects.begin(); it != heapRects.end(); ++it) {
        if (it.value().contains(pos)) {
            int index = it.key();
            qDebug() << "Heap node clicked at index:" << index;
            highlightIndex=index;
            update();
            return;
        }
    }
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
        QRect rect(x, y, w-2, h);
        if (i == highlightIndex) p.setPen(QPen(Qt::red, 2));  // 高亮红框
        else p.setPen(QPen(Qt::black, 1));  // 默认黑框
        p.drawRect(rect);   // 绘制柱状条
        barRects[i] = rect;
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

        QRect rect(x, y, w - 2, h);
        if (i == highlightIndex) p.setPen(QPen(Qt::red, 2));  // 高亮红框
        else p.setPen(QPen(Qt::black, 1));  // 默认黑框
        p.drawRect(rect);
        barRects[i] = rect;
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
        if (i == highlightIndex) p.setPen(QPen(Qt::red, 2));  // 高亮红框
        else p.setPen(QPen(Qt::black, 1));  // 默认黑框
        p.drawEllipse(rect);
        heapRects[i] = rect;
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
    autoRunning=false;
    timer->stop();
}


void SortWidget::setSortMethod(int method){
    worker->init();
    if(visualData.isEmpty()){
        QString temp;
        if(lastfileName.isEmpty()) temp="./data/graph.txt";
        else temp=lastfileName;
        QVector<int>tempdata=f->readArray1D(temp);
        visualData=tempdata;
    }
    worker->setData(visualData);
    switch(method) {
        case 1: worker->BubbleSort(); break;
        case 2: worker->InsertionSort(); break;
        case 3: worker->SelectionSort(); break;
        case 4: worker->QuickSort(0, visualData.size() - 1); break;
        case 5: worker->MergeSort(0, visualData.size() - 1); break;
        case 6: worker->HeapSort(); break;
        default: break;
    }
    isFinished=true;
}

bool SortWidget::saveToFile(const QString &fileName) {
    if(method==0 || visualData.isEmpty()){
        QMessageBox::warning(this, "Warning", "Please choose a sort method and sort the data first.");
        return false;
    }
    f->saveToFile(fileName, visualData);
    lastfileName=fileName;
    return true;
}

bool SortWidget::loadFromFile(const QString &fileName) {
    if(method==0){
        QMessageBox::warning(this, "Warning", "Please choose a sort method first.");
        return false;
    }
    visualData=f->readArray1D(fileName);
    if(visualData.isEmpty()) return false;
    lastfileName=fileName;
    worker->setData(visualData);
    setSortMethod(method);
    update();
    return true;
}

void SortWidget::randomize() {
    if(method==0){
        QMessageBox::warning(this, "Warning", "Please choose a sort method first.");
        return;
    }
    inint();
    worker->init();
    visualData.clear();
    int len=10;
    visualData.resize(len);
    for(int i=0;i<len;i++){
        visualData[i]=rand()%100-50;
        if(visualData[i]<0) visualData[i]*=-1;
    }
    worker->setData(visualData);
    setSortMethod(method);
    update();
}


