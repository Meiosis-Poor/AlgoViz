#include "graphwidget.h"
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QInputDialog>
#include <QGridLayout>
#include <QPainterPath>
#include <QHeaderView>
#include <QLabel>
#include <cstdlib>  // for rand
#include <ctime>    // for time

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent) {
    init();
    //setButton(this);
}


void GraphWidget::init() {
    autoRunning=false;
    nowStep={Step::VisitNode, -1, {-1,-1}};
    currentNode=-1;
    currentEdge={-1,-1};
    previousNode.clear();
    previousEdge.clear();
    RebuildStack.clear();
    LastStack.clear();
    dijfa.clear();
    selectedNode1 = -1;
    selectedNode2 = -1;
}

void GraphWidget::clear() {
    init();
    intdata.clear();
    pairdata.clear();
}

void GraphWidget::setButton(QWidget *panel) {
    // ==== 基础控件 ====
    QPushButton *insertbtn = new QPushButton("Insert Node", panel);
    QPushButton *addEdgebtn = new QPushButton("Add Edge", panel);
    QPushButton *deleteNodebtn = new QPushButton("Delete Node", panel);
    QPushButton *deleteEdgebtn = new QPushButton("Delete Edge", panel);
    QPushButton *autobtn = new QPushButton("Auto", panel);
    QPushButton *lastbtn = new QPushButton("Last", panel);
    QPushButton *nextbtn = new QPushButton("Next", panel);

    // ==== 算法选择 ====
    methodCombo = new QComboBox(panel);
    methodCombo->addItem("Please select a method");
    methodCombo->addItem("BFS");
    methodCombo->addItem("DFS");
    methodCombo->addItem("Prim");
    methodCombo->addItem("Kruskal");
    methodCombo->addItem("Dijkstra");

    // ==== 滑动条 ====
    QSlider *speedSlider = new QSlider(Qt::Horizontal, panel);
    speedSlider->setRange(100, 999);
    speedSlider->setValue(autoInterval);
    QLabel *speedLabel = new QLabel(QString("Speed: %1 ms").arg(autoInterval), panel);

    // ==== 布局 ====
    QGridLayout *grid = new QGridLayout(panel);
    grid->setAlignment(Qt::AlignTop);
    grid->setContentsMargins(10, 10, 10, 10);
    grid->setSpacing(8);

    QSizePolicy longPolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    insertbtn->setSizePolicy(longPolicy);
    addEdgebtn->setSizePolicy(longPolicy);
    deleteNodebtn->setSizePolicy(longPolicy);
    deleteEdgebtn->setSizePolicy(longPolicy);
    lastbtn->setSizePolicy(longPolicy);
    nextbtn->setSizePolicy(longPolicy);

    // ==== 排版（两行） ====
    grid->addWidget(insertbtn,0, 0);
    grid->addWidget(addEdgebtn,0, 1);
    grid->addWidget(deleteNodebtn,1, 0);
    grid->addWidget(deleteEdgebtn,1, 1);
    grid->addWidget(lastbtn,0, 2);
    grid->addWidget(nextbtn,1, 2);
    autobtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    grid->addWidget(autobtn, 0, 3, 2, 1);
    grid->addWidget(speedSlider,0, 4);
    grid->addWidget(speedLabel,1,4);
    methodCombo->setMinimumWidth(200);
    grid->addWidget(methodCombo, 0, 5, 2, 1);

    panel->setLayout(grid);

    // ==== panel 样式（顶部工具栏） ====
    panel->setFixedHeight(90);
    panel->setStyleSheet(R"(
        QWidget {
            background-color: #2E3440;  /* 深灰蓝背景 */
            border-bottom: 2px solid #4C566A;
        }
    )");

    // ==== 按钮样式 ====
    QString btnStyle = R"(
        QPushButton {
            background-color: #5E81AC;
            color: white;
            border-radius: 8px;
            padding: 6px 10px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #81A1C1;
        }
        QPushButton:pressed {
            background-color: #4C566A;
        }
    )";
    insertbtn->setStyleSheet(btnStyle);
    addEdgebtn->setStyleSheet(btnStyle);
    deleteNodebtn->setStyleSheet(btnStyle);
    deleteEdgebtn->setStyleSheet(btnStyle);
    autobtn->setStyleSheet(btnStyle);
    lastbtn->setStyleSheet(btnStyle);
    nextbtn->setStyleSheet(btnStyle);

    // ==== QComboBox 和 Label 样式 ====
    methodCombo->setStyleSheet(R"(
        QComboBox {
            background-color: white;
            color: black;
            border: 1px solid #4C566A;
            border-radius: 5px;
            padding: 3px 6px;
            font-weight: bold;
        }
        QComboBox QAbstractItemView {
            background: white;
            color: black;
            selection-background-color: #81A1C1;
        }
    )");
    speedLabel->setStyleSheet("color: white; font-weight: bold;");

    // ==== 信号与槽 ====
    connect(speedSlider, &QSlider::valueChanged, panel, [=](int value) mutable {
        autoInterval = value;
        if (autoRunning)
            timer->start(autoInterval);
        speedLabel->setText(QString("Speed: %1 ms").arg(value));
    });

    connect(insertbtn, &QPushButton::clicked, this, [=](){ insertNode(); });
    connect(addEdgebtn, &QPushButton::clicked, this, [=](){ addEdge(); });
    connect(deleteNodebtn, &QPushButton::clicked, this, [=](){ deleteNode(); });
    connect(deleteEdgebtn, &QPushButton::clicked, this, [=](){ deleteEdge(); });
    connect(autobtn, &QPushButton::clicked, this, [=](){
        autoRunning = !autoRunning;
        if (autoRunning)
            timer->start(autoInterval);
        else
            timer->stop();
    });
    connect(methodCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        if (!isFinished) {
            QMessageBox::warning(this, "Warning", "Please wait for the last sorting to finish.");
            return;
        }
        if (index == 0) return;
        method = index;
        init();
        setMethodText(method, methodCombo);
        update();
        setMethod(method);
    });
    connect(lastbtn, &QPushButton::clicked, this, [=](){ onLastBtnClicked(); });
    connect(nextbtn, &QPushButton::clicked, this, [=](){ onNextBtnClicked(); });
    connect(timer, &QTimer::timeout, this, &GraphWidget::onNextBtnClicked);
}


void GraphWidget::onLastBtnClicked(){
    if(LastStack.empty()) return;
    RebuildStack.push(nowStep);
    if(nowStep.type==Step::VisitNode){
        currentNode=nowStep.node;
        previousNode.pop();
    }
    else if(nowStep.type==Step::TraverseEdge){
        currentEdge=nowStep.edge;
        previousEdge.pop();
    }
    else{
        dist[nowStep.node]=nowStep.edge.first;
        drawDistTable();
    }
    nowStep=LastStack.top();
    LastStack.pop();
    update();
}

void GraphWidget::onNextBtnClicked() {
    if(RebuildStack.empty()) {
        Step tempstep=worker->nextStep();
        if(tempstep.node==-1 && tempstep.edge.first==-1 && tempstep.edge.second==-1){
            timer->stop();
            return;
        }
        LastStack.push(nowStep);
        nowStep=tempstep;
    }
    else{
        LastStack.push(nowStep);
        nowStep=RebuildStack.top();
        RebuildStack.pop();
    }
    if(nowStep.type==Step::VisitNode){
        currentNode=nowStep.node;
        previousNode.push(currentNode);
    }
    else if(nowStep.type==Step::TraverseEdge){
        currentEdge=nowStep.edge;
        previousEdge.push(currentEdge);
    }
    else{
        dist[nowStep.node]=nowStep.edge.second;
        drawDistTable();
    }
    update();
    return;
}

void GraphWidget::random(int nodeCount, int maxEdgeValue, double edgeProbability) {
    if (nodeCount <= 0) return;

    intdata.clear();
    pairdata.clear();

    std::srand(std::time(nullptr));

    if (nowStructureIndex == 0) {
        intdata.resize(nodeCount, QVector<int>(nodeCount, 0));
        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodeCount; j++) {
                if (i != j && (double)rand() / RAND_MAX < edgeProbability) {
                    intdata[i][j] = rand() % maxEdgeValue + 1;
                }
            }
        }
    }
    else {
        pairdata.resize(nodeCount);
        for (int i = 0; i < nodeCount; i++) {
            for (int j = 0; j < nodeCount; j++) {
                if (i != j && (double)rand() / RAND_MAX < edgeProbability) {
                    int value = rand() % maxEdgeValue + 1;
                    pairdata[i].push_back({j, value});
                }
            }
        }
    }

    init();
    setMethod(method);
    update();
}

void GraphWidget::reset(QAction* listbtn, QAction* matrixbtn) {
    intdata.clear();
    pairdata.clear();
    QString temp;
    if(lastFileName.isEmpty()) temp="./data/graph2.txt";
    else temp=lastFileName;
    loadFromFile(temp,listbtn,matrixbtn);
    init();
    setMethod(method);
    update();
}


void GraphWidget::setMethodText(int m, QComboBox* combo){
    isFinished = false;
    switch(m){
        case 1: combo->setCurrentText("BFS"); break;
        case 2: combo->setCurrentText("DFS"); break;
        case 3: combo->setCurrentText("Prim"); break;
        case 4: combo->setCurrentText("Kruskal"); break;
        case 5: combo->setCurrentText("Dijkstra"); break;
        default: combo->setCurrentText("请选择算法"); break;
    }
    autoRunning=false;
    timer->stop();
}

void GraphWidget::setMethod(int m) {
    if(intdata.isEmpty() && pairdata.isEmpty()){
        QString temp;
        if(lastFileName.isEmpty()) temp="./data/graph2.txt";
        else temp=lastFileName;
        loadFromFile(temp,new QAction(nullptr),new QAction(nullptr));
    }
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    init();
    if(m != 5) distTable->setVisible(false);
    switch(m) {
        case 1: worker->BFSall(); break;
        case 2: worker->DFSall(); break;
        case 3: worker->primall(); break;
        case 4: worker->kruskal(); break;
        case 5:
        {
            dijfa.clear();
            distTable->setVisible(true);
            int n_size = 0;
            if(nowStructureIndex == 0) n_size = intdata.size();
            else n_size = pairdata.size();
            dist.fill(INT_MAX, n_size);
            dijstart = QInputDialog::getInt(this, "Dijkstra", "Start Node:", 0, 0, n_size-1, 1);
            dist[dijstart]=0;
            drawDistTable();
            dijfa=worker->dijkstra(dijstart);
        }
            break;
        default: break;
    }
    isFinished=true;
}


void GraphWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    drawGraphArea(p);
    drawStructureArea(p);
}

void GraphWidget::drawDistTable() {
    if(!distTable->isVisible()) return;
    int n = dist.size();
    distTable->setRowCount(n);
    for(int i = 0; i < n; i++) {
        distTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        if(dist[i] == INT_MAX)
            distTable->setItem(i, 1, new QTableWidgetItem("INF"));
        else
            distTable->setItem(i, 1, new QTableWidgetItem(QString::number(dist[i])));
    }
}

void GraphWidget::drawGraphArea(QPainter &p) {
    int n = 0;
    if(nowStructureIndex == 0) {
        if(intdata.isEmpty()) return;
        n = intdata.size();
    }
    else {
        if(pairdata.isEmpty()) return;
        n = pairdata.size();
    }

    int w = width() * 2 / 3;
    int h = height() - topMargin;
    int radius = std::min(w, h) / 2 - 50;
    QPoint center(w/2, topMargin + h/2);

    nodePos.clear();
    nodeRect.clear();
    int nodeRadius = std::max(10, std::min(25, int(2 * M_PI * radius / n / 2)));

    for(int i = 0; i < n; i++){
        double angle = 2 * M_PI * i / n;
        int x = center.x() + radius * cos(angle);
        int y = center.y() + radius * sin(angle);
        nodePos.push_back(QPoint(x, y));
        nodeRect.push_back(QRect(x-nodeRadius, y-nodeRadius, 2 * nodeRadius, 2 * nodeRadius));
    }

    p.setPen(QPen(Qt::black, 2));

    if(nowStructureIndex == 0) {
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(intdata[i][j] != 0){
                    if(method==5 && dijpath.contains(QPair<int,int>(i,j)))
                        p.setPen(QPen(Qt::yellow, 2));
                    else if((i == selectedNode1 && j == selectedNode2) )
                        p.setPen(QPen(Qt::yellow, 2));
                    else if(nowStep.type==Step::TraverseEdge &&  nowStep.edge.first==i && nowStep.edge.second==j)
                        p.setPen(QPen(Qt::red, 2));
                    else if(previousEdge.contains(QPair<int,int>(i,j)))
                        p.setPen(QPen(Qt::green, 2));
                    else p.setPen(QPen(Qt::gray, 2));
                    bool curved = (intdata[j][i] != 0);
                    drawArrow(p, nodePos[i], nodePos[j],nodeRadius,QString::number(intdata[i][j]),curved);
                }
            }
        }
    }
    else {
        for(int i=0;i<n;i++){
            for(auto &edge: pairdata[i]){
                int j = edge.first;
                if(method==5 && dijpath.contains(QPair<int,int>(i,j)))
                    p.setPen(QPen(Qt::yellow, 2));
                else if((i == selectedNode1 && j == selectedNode2) )
                    p.setPen(QPen(Qt::yellow, 2));
                else if(nowStep.type==Step::TraverseEdge &&  nowStep.edge.first==i && nowStep.edge.second==j)
                    p.setPen(QPen(Qt::red, 2));
                else if(previousEdge.contains(QPair<int,int>(i,j)))
                    p.setPen(QPen(Qt::green, 2));
                else p.setPen(QPen(Qt::gray, 2));
                bool curved = false;
                for(auto &back : pairdata[j]) {
                    if(back.first == i) {
                        curved = true;
                        break;
                    }
                }
                drawArrow(p, nodePos[i], nodePos[j],nodeRadius,QString::number(edge.second),curved);
            }
        }
    }
    p.setPen(QPen(Qt::black, 2));

    for(int i=0;i<n;i++){
        p.setBrush((i == selectedNode1 || i == selectedNode2) ? Qt::yellow : (nowStep.type==Step::VisitNode && nowStep.node==i)? Qt::red : (previousNode.contains(i)) ? Qt::green : Qt::white);
        p.drawEllipse(nodeRect[i]);
        p.drawText(nodeRect[i], Qt::AlignCenter, QString::number(i));
    }
}

void GraphWidget::drawStructureArea(QPainter &p) {
    int left = width() * 2 / 3;
    int w = width() / 3;
    int top = topMargin;
    int h = height() - topMargin;

    p.setPen(Qt::black);
    QFont font = p.font();
    font.setPointSize(10);
    p.setFont(font);
    p.setBrush(Qt::white);

    if (nowStructureIndex == 0) {
        if (intdata.isEmpty()) return;
        int n = intdata.size();
        int maxCellWidth = w / n;
        int maxCellHeight = h / n;
        int cellSize = std::max(10, std::min(maxCellWidth, maxCellHeight));

        int matrixWidth = n * cellSize;
        int matrixHeight = n * cellSize;
        int startX = left + (w - matrixWidth) / 2;
        int startY = top + (h - matrixHeight) / 2;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                QRect cellRect(startX + j * cellSize, startY + i * cellSize, cellSize, cellSize);
                if ((selectedNode1 == i && selectedNode2 == j) || (selectedNode2 == -1 && selectedNode1 == i))
                    p.fillRect(cellRect, QColor(255, 255, 0, 100));
                p.drawRect(cellRect);
                p.drawText(cellRect, Qt::AlignCenter, QString::number(intdata[i][j]));
            }
        }
    } else {
        if (pairdata.isEmpty()) return;

        QFontMetrics fm(p.font());
        int topMarginY = 10;
        int rowPadding = 6;
        int rowHeight = fm.height() + rowPadding;
        int startY = top + topMarginY;
        int leftX = left + 5;
        int rightX = left + w - 5;

        for (int i = 0; i < pairdata.size(); i++) {
            int yy = startY;

            // 前缀 "i ->"
            QString prefix = QString::number(i) + " -> ";
            QSize prefixSize = fm.size(Qt::TextSingleLine, prefix);
            QRect prefixRect(leftX, yy, prefixSize.width() + 6, rowHeight);

            p.setBrush(Qt::white);
            p.fillRect(prefixRect, Qt::white);
            p.setPen(Qt::black);
            p.drawRect(prefixRect);
            p.drawText(prefixRect.adjusted(3,0,-3,0), Qt::AlignLeft | Qt::AlignVCenter, prefix);

            int offsetX = prefixRect.right() + 5;

            for (auto &edge : pairdata[i]) {
                QString nodeStr = QString("%1(%2)").arg(edge.first).arg(edge.second);
                QSize textSize = fm.size(Qt::TextSingleLine, nodeStr);

                // 换行处理
                if (offsetX + textSize.width() + 6 > rightX) {
                    offsetX = prefixRect.right() + 5;
                    yy += rowHeight + 5;
                }

                QRect nodeRect(offsetX, yy, textSize.width() + 6, rowHeight);
                p.setBrush(Qt::white);
                p.fillRect(nodeRect, Qt::white);
                p.setPen(Qt::black);
                p.drawRect(nodeRect);
                p.drawText(nodeRect.adjusted(3,0,-3,0), Qt::AlignLeft | Qt::AlignVCenter, nodeStr);

                // 箭头从矩形右中点发出，长度自动计算，不超出右边界
                int arrowLength = std::min(20, rightX - nodeRect.right() - 2);
                QPoint start(nodeRect.right() + 2, nodeRect.center().y());
                QPoint end(start.x() + arrowLength, start.y());
                p.setPen(Qt::yellow);
                drawArrow(p, start, end, 0, "");

                offsetX = end.x() + 5;
            }

            startY = yy + rowHeight + 5;
        }

    }
}



void GraphWidget::drawArrow(QPainter &p, const QPoint &startCenter, const QPoint &endCenter,
                            int nodeRadius, const QString &text, bool curved) {
    QLineF line(startCenter, endCenter);
    if (line.length() < 1e-6) return;

    QLineF unit = line.unitVector();
    QPointF start = startCenter + (unit.p2() - unit.p1()) * nodeRadius;
    QPointF end   = endCenter   - (unit.p2() - unit.p1()) * nodeRadius;

    QPointF textPos;
    if (curved) {
        // 曲线
        QPointF mid = (start + end) / 2.0;
        QPointF offset(-(end.y() - start.y()), end.x() - start.x());
        offset = offset / std::sqrt(QPointF::dotProduct(offset, offset)) * 20;
        QPointF ctrl = mid + offset;

        QPainterPath path(start);
        path.quadTo(ctrl, end);
        p.drawPath(path);

        // 取路径中点
        textPos = path.pointAtPercent(0.5);
    } else {
        // 直线
        p.drawLine(start, end);
        textPos = (start + end) / 2.0;
    }

    // 箭头
    const double arrowSize = 8.0;
    double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
    QPointF arrowP1 = end - QPointF(std::cos(angle + M_PI/6) * arrowSize,
                                    std::sin(angle + M_PI/6) * arrowSize);
    QPointF arrowP2 = end - QPointF(std::cos(angle - M_PI/6) * arrowSize,
                                    std::sin(angle - M_PI/6) * arrowSize);

    QPolygonF arrowHead;
    arrowHead << end << arrowP1 << arrowP2;
    QBrush oldBrush = p.brush();
    p.setBrush(p.pen().color());
    p.drawPolygon(arrowHead);
    p.setBrush(oldBrush);

    // 绘制文字（稍微偏移，让文字不压住线）
    if (!text.isEmpty()) {
        QFontMetrics fm(p.font());
        QSize textSize = fm.size(Qt::TextSingleLine, text);
        QPointF offset(0, -5); // 上移一点
        QRectF textRect(textPos.x() - textSize.width()/2,
                        textPos.y() - textSize.height()/2,
                        textSize.width(), textSize.height());
        textRect.translate(offset);
        p.drawText(textRect, Qt::AlignCenter, text);
    }
}



void GraphWidget::mousePressEvent(QMouseEvent *event) {
    for (int i = 0; i < nodeRect.size(); i++) {
        if (nodeRect[i].contains(event->pos())) {
            if(selectedNode1 == -1) selectedNode1 = i;
            else if(method!=5 && selectedNode2 == -1 && selectedNode1!= i) selectedNode2 = i;
            else{
                selectedNode1 = i;
                selectedNode2 = -1;
            }
            if(method==5 && selectedNode1!= -1){
                int now=selectedNode1;
                dijpath.clear();
                while(now!=dijstart) {
                    dijpath.push_back(QPair<int, int>(dijfa[now], now));
                    now=dijfa[now];
                }
            }
            update();
            return;
        }
    }
    selectedNode1 = -1;
    selectedNode2 = -1;
    update();
}

void GraphWidget::insertNode() {
    if(nowStructureIndex == 0){
        int n = intdata.size();
        for(int i = 0; i < n; i++) {
            intdata[i].push_back(0);
        }
        intdata.push_back(QVector<int>(n + 1, 0));
    }
    else{
        int n = pairdata.size();
        pairdata.push_back({});
        pairdata[n].push_back(qMakePair(n,0));
    }
    setMethod(method);
    update();
}

void GraphWidget::addEdge() {
    if(selectedNode1 == -1 || selectedNode2 == -1){
        QMessageBox::warning(this, "Error", "Please select two nodes.");
        return;
    }
    if(selectedNode1 == selectedNode2){
        QMessageBox::warning(this, "Error", "Please select different nodes.");
        return;
    }
    int value=QInputDialog::getInt(this, "Add Edge", "Value:");
    if(nowStructureIndex == 0){
        intdata[selectedNode1][selectedNode2] = value;
    }
    else{
        pairdata[selectedNode1].push_back({selectedNode2,value});
    }
    selectedNode1 = -1;
    selectedNode2 = -1;
    setMethod(method);
    update();
}

void GraphWidget::deleteNode() {
    bool ok;
    int nownode = QInputDialog::getInt(this, "Delete Node", "NodeId:", 0, 0,
                                       nowStructureIndex==0 ? intdata.size()-1 : pairdata.size()-1, 1, &ok);
    if(!ok) return;
    if(nownode<0 || nownode>=intdata.size()){
        QMessageBox::warning(this, "Error", "Invalid NodeId.");
        return;
    }
    if(nowStructureIndex == 0){
        intdata.erase(intdata.begin() + nownode);
        for(int i = 0; i < intdata.size(); i++){
            if(nownode < intdata[i].size())
                intdata[i].erase(intdata[i].begin() + nownode);
        }
    }
    else{
        pairdata.erase(pairdata.begin() + nownode);
        for(auto &edges : pairdata){
            for(int j = edges.size() - 1; j >= 0; j--){
                if(edges[j].first == nownode)
                    edges.erase(edges.begin() + j);
                else if(edges[j].first > nownode)
                    edges[j].first--;
            }
        }
    }
    setMethod(method);
    update();
}

void GraphWidget::deleteEdge() {
    if(selectedNode1 == -1 || selectedNode2 == -1){
        QMessageBox::warning(this, "Error", "Please select two nodes.");
        return;
    }
    if(selectedNode1 == selectedNode2){
        QMessageBox::warning(this, "Error", "Please select different nodes.");
        return;
    }
    if(nowStructureIndex == 0){
        intdata[selectedNode1][selectedNode2] = 0;
    }
    else{
        auto &edges = pairdata[selectedNode1];
        for(int j = edges.size() - 1; j >= 0; j--){
            if(edges[j].first == selectedNode2)
                edges.erase(edges.begin() + j);
        }
    }
    selectedNode1 = -1;
    selectedNode2 = -1;
    setMethod(method);
    update();
}

void GraphWidget::transformToAdjacencyList() {
    if(nowStructureIndex == 1) return;
    nowStructureIndex = 1;
    pairdata.clear();
    pairdata.resize(intdata.size());
    for(int i = 0; i < intdata.size(); i++){
        for(int j = 0; j < intdata.size(); j++){
            if(intdata[i][j] != 0){
                pairdata[i].push_back({j,intdata[i][j]});
            }
        }
    }
    intdata.clear();
    setMethod(method);
    update();
}

void GraphWidget::transformToAdjacencyMatrix() {
    if(nowStructureIndex == 0) return;
    nowStructureIndex = 0;
    int n = pairdata.size();
    intdata.clear();
    intdata.resize(n, QVector<int>(n, 0));
    for(int i = 0; i < n; i++){
        for(auto &edge : pairdata[i]){
            intdata[i][edge.first] = edge.second;
        }
    }
    pairdata.clear();
    setMethod(method);
    update();
}


bool GraphWidget::saveToFile(const QString &fileName) {
    f = new FileLoader();
    if(intdata.isEmpty() && pairdata.isEmpty()){
        QMessageBox::warning(this, "Error", "No data to save.");
        return false;
    }
    if(!intdata.empty()) f->saveToFile(fileName,intdata);
    else f->saveToFile(fileName,pairdata);
    lastFileName = fileName;
    return true;
}

bool GraphWidget::loadFromFile(const QString &fileName,QAction* listbtn, QAction* matrixbtn) {
    init();
    f = new FileLoader();
    if(f->detectType(fileName) == FileDataType::AdjacencyMatrix){
        intdata=f->readAdjacencyMatrix(fileName);
        nowStructureIndex = 0;
        lastFileName = fileName;
        matrixbtn->setChecked(true);
        listbtn->setChecked(false);
    }
    else if(f->detectType(fileName) == FileDataType::AdjacencyList){
        pairdata=f->readAdjacencyList(fileName);
        nowStructureIndex = 1;
        lastFileName = fileName;
        listbtn->setChecked(true);
        matrixbtn->setChecked(false);
    }
    else{
        QMessageBox::warning(this, "Error", "Unsupported file type.");
        return false;
    }
    update();
    return true;
}

void GraphWidget::deleteNodeById(int id){
    if(id<0 || id>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    if(nowStructureIndex == 0){
        intdata.erase(intdata.begin() + id);
        for(int i = 0; i < intdata.size(); i++){
            if(id < intdata[i].size())
                intdata[i].erase(intdata[i].begin() + id);
        }
    }
    else{
        pairdata.erase(pairdata.begin() + id);
        for(auto &edges : pairdata){
            for(int j = edges.size() - 1; j >= 0; j--){
                if(edges[j].first == id)
                    edges.erase(edges.begin() + j);
                else if(edges[j].first > id)
                    edges[j].first--;
            }
        }
    }
    setMethod(method);
    update();
}

void  GraphWidget::addEdgeByValue(int u,int v,int value){
    if(u<0 || u>=intdata.size() || v<0 || v>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    if(nowStructureIndex == 0){
        intdata[u][v] = value;
    }
    else{
        pairdata[u].push_back({v,value});
    }
    selectedNode1 = -1;
    selectedNode2 = -1;
    setMethod(method);
    update();
}

void  GraphWidget::deleteEdgeByValue(int u,int v){
    if(u<0 || u>=intdata.size() || v<0 || v>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    if(nowStructureIndex == 0){
        intdata[u][v] = 0;
    }
    else{
        auto &edges = pairdata[u];
        for(int j = edges.size() - 1; j >= 0; j--){
            if(edges[j].first == v)
                edges.erase(edges.begin() + j);
        }
    }
    selectedNode1 = -1;
    selectedNode2 = -1;
    setMethod(method);
    update();
}

void  GraphWidget::runBFS(int start){
    if(intdata.empty() && pairdata.empty()){
        throw std::invalid_argument("No data to run.");
    }
    if(start<0 || start>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    method = 1;
    methodCombo->setCurrentIndex(1);
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    worker->BFS(start);
    update();
}

void  GraphWidget::runDFS(int start){
    if(intdata.empty() && pairdata.empty()){
        throw std::invalid_argument("No data to run.");
    }
    if(start<0 || start>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    method = 2;
    methodCombo->setCurrentIndex(2);
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    worker->DFS(start);
    update();
}

void  GraphWidget::runPrim(){
    if(intdata.empty() && pairdata.empty()){
        throw std::invalid_argument("No data to run.");
    }
    method = 3;
    methodCombo->setCurrentIndex(3);
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    worker->primall();
    update();
}

void  GraphWidget::runKruskal(){
    if(intdata.empty() && pairdata.empty()){
        throw std::invalid_argument("No data to run.");
    }
    method = 4;
    methodCombo->setCurrentIndex(4);
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    worker->kruskal();
    update();
}

void  GraphWidget::runDijkstra(int start){
    if(intdata.empty() && pairdata.empty()){
        throw std::invalid_argument("No data to run.");
    }
    if(start<0 || start>=intdata.size()){
        throw std::invalid_argument("Invalid NodeId.");
    }
    method = 5;
    methodCombo->setCurrentIndex(5);
    dijstart=start;
    if(nowStructureIndex == 0) worker->setData(intdata);
    else worker->setData(pairdata);
    dijfa.clear();
    dist.fill(INT_MAX, dist.size());
    dist[dijstart]=0;
    drawDistTable();
    dijfa=worker->dijkstra(dijstart);
    update();
}