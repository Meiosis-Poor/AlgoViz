//
// Created by poorc on 2025/9/16.
//

#ifndef SIMULATOR_GRAPHWIDGET_H
#define SIMULATOR_GRAPHWIDGET_H

#include <QWidget>
#include <QStack>
#include <QPushButton>
#include <QTimer>
#include <QString>
#include <QTableWidget>
#include <QStackedWidget>
#include <QComboBox>
#include "fileloader.h"
#include "graphworker.h"

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void init();
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName,QAction* listbtn, QAction* matrixbtn);
    void random(int nodeCount, int maxEdgeValue = 10, double edgeProbability = 0.3);
    void reset(QAction* listbtn, QAction* matrixbtn);
    void drawGraphArea(QPainter &p);
    void drawDistTable();
    void setDistTable(QTableWidget *table) { distTable = table; }
    void drawStructureArea(QPainter &p);
    void drawArrow(QPainter &p, const QPoint &startCenter, const QPoint &endCenter, int nodeRadius, const QString &text,bool curved=false);
    void setButton(QWidget *panel);
    void onLastBtnClicked();
    void onNextBtnClicked();
    void setMethod(int m);
    void setMethodText(int m, QComboBox* combo);
    void insertNode();
    void addEdge();
    void deleteNode();
    void deleteEdge();
    void transformToAdjacencyList();
    void transformToAdjacencyMatrix();
    void deleteNodeById(int id);
    void addEdgeByValue(int u,int v,int value);
    void deleteEdgeByValue(int u,int v);
    void runBFS(int start);
    void runDFS(int start);
    void runPrim();
    void runKruskal();
    void runDijkstra(int start);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    QString lastFileName;
    QVector<QVector<int>> intdata;
    QVector<QVector<QPair<int,int>>> pairdata;
    QVector<QPoint> nodePos;  // 保存每个节点圆心坐标
    QVector<QRect> nodeRect;  // 保存每个节点的矩形坐标
    int selectedNode1,selectedNode2;  // 保存选中的节点索引
    int topMargin = 70;
    int nowStructureIndex = 0;
    int method=0;
    bool isFinished=true;
    bool autoRunning=false;
    int autoInterval=300;
    FileLoader *f;
    QTimer* timer=new QTimer();
    Step nowStep;
    int currentNode;                  // 当前处理的节点
    QStack<int> previousNode;        // 之前处理过的节点
    QPair<int,int> currentEdge;       // 当前正在处理的边
    QStack<QPair<int,int>> previousEdge;  // 之前处理过的边
    QStack<Step> RebuildStack;
    QStack<Step> LastStack;
    QVector<int> dist;
    GraphWorker *worker=new GraphWorker();
    QTableWidget *distTable = nullptr;
    QComboBox *methodCombo;

};

#endif //SIMULATOR_GRAPHWIDGET_H