#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graphwidget.h"
#include "sortwidget.h"
#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStackedWidget *stacked=new QStackedWidget(this);
    GraphWidget *graph=new GraphWidget(this);
    SortWidget *sort=new SortWidget(this);
    stacked->insertWidget(0, graph); //建立图的窗口
    stacked->insertWidget(1, sort); //建立排序的窗口
    setCentralWidget(stacked);
    //点击触发模式切换
    connect(ui->graphbtn, &QAction::triggered, this, [=](){
        stacked->setCurrentIndex(0);
    });
    connect(ui->sortbtn, &QAction::triggered, this, [=]() {
        stacked->setCurrentIndex(1);
    });
    connect(ui->openbtn, &QAction::triggered, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(
                this,                       // 父窗口
                "选择打开文件",              // 对话框标题
                QDir::homePath(),           // 默认路径
                "Text Files (*.txt);;All Files (*.*)" // 文件类型过滤
        );
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            //graph->loadFromFile(filePath);
        }
        else{
            if(sort->loadFromFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->savebtn, &QAction::triggered, this, [=]() {
        QString filePath;
        if(lastfilename.isEmpty()) {
            filePath = QFileDialog::getSaveFileName(
                    this,                       // 父窗口
                    "选择另存为位置",              // 对话框标题
                    QDir::homePath(),           // 默认路径
                    "Text Files (*.txt);;All Files (*.*)" // 文件类型过滤
            );
        }
        else filePath = lastfilename;
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            int a=0;
        }
        else{
            if(sort->saveToFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->saveotherbtn, &QAction::triggered, this, [=]() {
        QString filePath = QFileDialog::getSaveFileName(
                this,                       // 父窗口
                "选择另存为位置",              // 对话框标题
                QDir::homePath(),           // 默认路径
                "Text Files (*.txt);;All Files (*.*)" // 文件类型过滤
        );
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            int a=0;
        }
        else{
            if(sort->saveToFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->resetbtn, &QAction::triggered, this, [=]() {
        if(stacked->currentIndex() == 0){
            int a=0;
        }
        else{
            sort->reset();
        }
    });
    connect(ui->randombtn, &QAction::triggered, this, [=]() {
        if(stacked->currentIndex() == 0){
            int a=0;
        }
        else{
            sort->randomize();
        }
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}
