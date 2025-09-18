#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graphwidget.h"
#include "sortwidget.h"
#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStackedWidget *stacked=new QStackedWidget(this);
    stacked->insertWidget(0, new GraphWidget(this)); //建立图的窗口
    stacked->insertWidget(1, new SortWidget(this)); //建立排序的窗口
    setCentralWidget(stacked);
    //点击触发模式切换
    connect(ui->graphbtn, &QAction::triggered, this, [=](){
        stacked->setCurrentIndex(0);
    });
    connect(ui->sortbtn, &QAction::triggered, this, [=]() {
        stacked->setCurrentIndex(1);
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}
