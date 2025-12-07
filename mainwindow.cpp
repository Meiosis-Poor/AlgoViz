#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QActionGroup>
#include <QAction>
#include <QSplitter>
#include <QHeaderView>
#include <QDockWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QStackedWidget *stacked = new QStackedWidget(this);
// ---- 图窗口部分 ----
    QSplitter *graphPage = new QSplitter(this);
    GraphPanel *graph = new GraphPanel(this);
    QTableWidget *distTable = new QTableWidget(this);
    distTable->setColumnCount(2);
    distTable->setHorizontalHeaderLabels({"Node", "Dist"});
    distTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    distTable->setVisible(false); // 默认隐藏，只有跑dijkstra时显示
    graphPage->addWidget(graph);
    graphPage->addWidget(distTable);
    graphPage->setStretchFactor(0, 3); // 左边画布宽
    graphPage->setStretchFactor(1, 1); // 右边表格窄
// 传表格指针给 GraphWidget，用于更新
    graph->graphArea->setDistTable(distTable);
    stacked->insertWidget(0, graphPage);



// ---- 排序窗口部分 ----
    SortPanel *sort = new SortPanel(this);
    stacked->insertWidget(1, sort);
    setCentralWidget(stacked);

    QAction *ascAction = new QAction("升序", this);
    ascAction->setCheckable(true);
    ascAction->setChecked(true);  // 默认升序
    QAction *descAction = new QAction("降序", this);
    descAction->setCheckable(true);
    ascAction->setVisible(false);
    descAction->setVisible(false);
    // 创建 QActionGroup 确保单选
    QActionGroup *orderGroup = new QActionGroup(this);
    orderGroup->addAction(ascAction);
    orderGroup->addAction(descAction);
    orderGroup->setExclusive(true);
    // 添加到菜单
    ui->menu_3->addAction(ascAction);
    ui->menu_3->addAction(descAction);
    connect(ascAction, &QAction::triggered, this, [=]() {
        sort->sortArea->setAscending(true);
    });
    connect(descAction, &QAction::triggered, this, [=]() {
        sort->sortArea->setAscending(false);
    });


    QAction *listbtn = new QAction("切换为邻接表", this);
    listbtn->setCheckable(true);
    QAction *matrixbtn = new QAction("切换为邻接矩阵", this);
    matrixbtn->setCheckable(true);
    matrixbtn->setChecked(true);
    matrixbtn->setVisible(false);
    listbtn->setVisible(false);
    // 创建 QActionGroup 确保单选
    QActionGroup *structureGroup = new QActionGroup(this);
    structureGroup->addAction(matrixbtn);
    structureGroup->addAction(listbtn);
    structureGroup->setExclusive(true);
    // 添加到菜单
    ui->menu_3->addAction(matrixbtn);
    ui->menu_3->addAction(listbtn);
    connect(listbtn, &QAction::triggered, this, [=]() {
        graph->graphArea->transformToAdjacencyList();
    });
    connect(matrixbtn, &QAction::triggered, this, [=]() {
        graph->graphArea->transformToAdjacencyMatrix();
    });

    // 创建 QActionGroup 确保单选
    ui->graphbtn->setCheckable(true);
    ui->sortbtn->setCheckable(true);
    QActionGroup *kindGroup = new QActionGroup(this);
    kindGroup->addAction(ui->graphbtn);
    kindGroup->addAction(ui->sortbtn);
    kindGroup->setExclusive(true);


    // 创建 Graph 与 Sort DSL 面板
    createGraphDSLPanel(graph->graphArea);
    createSortDSLPanel(sort->sortArea);
    ui->DSLAction->setCheckable(true);

    stacked->setCurrentIndex(0);
    ascAction->setVisible(false);
    descAction->setVisible(false);
    listbtn->setVisible(true);
    matrixbtn->setVisible(true);
    graphDslDock->setVisible(sortDslDock->isVisible());
    sortDslDock->setVisible(false);
    ui->graphbtn->setChecked(true);

    connect(ui->DSLAction, &QAction::triggered, this, [this,stacked]() {
        // 判断当前模式
        if (stacked->currentIndex() == 0) {
            // 切换 Graph DSL 面板
            graphDslDock->setVisible(!graphDslDock->isVisible());
            ui->DSLAction->setChecked(graphDslDock->isVisible());
            // 隐藏 Sort 面板
            sortDslDock->setVisible(false);
        } else if (stacked->currentIndex() == 1) {
            // 切换 Sort DSL 面板
            sortDslDock->setVisible(!sortDslDock->isVisible());
            ui->DSLAction->setChecked(sortDslDock->isVisible());
            // 隐藏 Graph 面板
            graphDslDock->setVisible(false);
        }
    });



    connect(ui->helpAction, &QAction::triggered, this, [=]() {
        HelpWindow *helpWin = new HelpWindow();
        helpWin->show();
    });


    //点击触发模式切换
    connect(ui->graphbtn, &QAction::triggered, this, [=](){
        stacked->setCurrentIndex(0);
        ascAction->setVisible(false);
        descAction->setVisible(false);
        listbtn->setVisible(true);
        matrixbtn->setVisible(true);
        graphDslDock->setVisible(sortDslDock->isVisible());
        sortDslDock->setVisible(false);
    });
    connect(ui->sortbtn, &QAction::triggered, this, [=]() {
        stacked->setCurrentIndex(1);
        ascAction->setVisible(true);
        descAction->setVisible(true);
        listbtn->setVisible(false);
        matrixbtn->setVisible(false);
        sortDslDock->setVisible(graphDslDock->isVisible());
        graphDslDock->setVisible(false);
    });

    connect(ui->openbtn, &QAction::triggered, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(
                this,
                "选择打开文件",
                QDir::homePath(),
                "Text Files (*.txt);;All Files (*.*)"
        );
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            if(graph->graphArea->loadFromFile(filePath,listbtn, matrixbtn)) lastfilename = filePath;
        }
        else{
            if(sort->sortArea->loadFromFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->savebtn, &QAction::triggered, this, [=]() {
        QString filePath;
        if(lastfilename.isEmpty()) {
            filePath = QFileDialog::getSaveFileName(
                    this,
                    "选择另存为位置",
                    QDir::homePath(),
                    "Text Files (*.txt);;All Files (*.*)"
            );
        }
        else filePath = lastfilename;
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            if(graph->graphArea->saveToFile(filePath)) lastfilename = filePath;
        }
        else{
            if(sort->sortArea->saveToFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->saveotherbtn, &QAction::triggered, this, [=]() {
        QString filePath = QFileDialog::getSaveFileName(
                this,
                "选择另存为位置",
                QDir::homePath(),
                "Text Files (*.txt);;All Files (*.*)"
        );
        if (filePath.isEmpty()) {
            return;
        }
        if(stacked->currentIndex() == 0){
            if(graph->graphArea->saveToFile(filePath)) lastfilename = filePath;
        }
        else{
            if(sort->sortArea->saveToFile(filePath)) lastfilename = filePath;
        }
    });
    connect(ui->resetbtn, &QAction::triggered, this, [=]() {
        if(stacked->currentIndex() == 0){
            graph->graphArea->reset(listbtn, matrixbtn);
        }
        else{
            sort->sortArea->reset();
        }
    });
    connect(ui->randombtn, &QAction::triggered, this, [=]() {
        if(stacked->currentIndex() == 0){
            graph->graphArea->random(6, 20, 0.4);
        }
        else{
            sort->sortArea->randomize();
        }
    });

    connect(ui->actionAI, &QAction::triggered, this, [=]() {
        bool ok;
        QString prompt=QInputDialog::getText(
                this,                    // 父窗口
                "AI 提示输入",            // 对话框标题
                "请输入提示词:",           // 标签文本
                QLineEdit::Normal,       // 输入模式
                "",                       // 默认值
                &ok                      // 是否点击 OK
        );
        if (ok && !prompt.isEmpty()) {
            QString doc;
            if(stacked->currentIndex() == 0) doc=graphdoc;
            else doc=sortdoc;
            ai->generateDSL(doc, prompt);
        }
        else {
            QMessageBox::information(this, "提示", "未输入提示词");
        }
    });

    connect(ai, &AIAssistant::dslReady, this, [this, stacked](const QString &dslCode){
        if(stacked->currentIndex() == 0) graphDslEditor->setPlainText(dslCode);
        else sortDslEditor->setPlainText(dslCode);
        QMessageBox::information(this, "提示", "生成成功");
    });

    connect(ai, &AIAssistant::errorOccurred, this, [this](const QString &err){
        QMessageBox::warning(this, "错误", "AI 错误：" + err);
        qDebug() << "AI Error:" << err;
    });
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createGraphDSLPanel(GraphWidget *graph) {
    // DockWidget 容器
    graphDslDock = new QDockWidget("Graph DSL Panel", this);
    graphDslDock->setAllowedAreas(Qt::RightDockWidgetArea);
    graphDslDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    graphDslDock->setVisible(false);

    // 容器
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);

    // DSL 编辑器
    graphDslEditor = new QTextEdit(container);
    graphDslEditor->setPlaceholderText("Write your Graph DSL code here...");
    layout->addWidget(graphDslEditor);

    // 运行按钮
    QPushButton *runButton = new QPushButton("Run Graph DSL", container);
    layout->addWidget(runButton);

    container->setLayout(layout);
    graphDslDock->setWidget(container);
    addDockWidget(Qt::RightDockWidgetArea, graphDslDock);

    // 高亮器
    graphDslHighlighter = new GraphDSLSyntaxHighlighter(graphDslEditor->document());

    // 解释器
    auto *interpreter = new GraphDSLInterpreter(graph, this);

    // Graph DSL 自动补全
    QStringList graphKeywords = {"let","for","end","insertNode","deleteNode","addEdge",
                                 "deleteEdge","bfs","dfs","prim","kruskal","dijkstra","random","clear"};
    graphCompleter = new DSLCompleter(graphDslEditor, graphKeywords, this);

    // 运行
    connect(runButton, &QPushButton::clicked, this, [this, interpreter]() {
        interpreter->run(graphDslEditor->toPlainText());
    });
}

void MainWindow::createSortDSLPanel(SortWidget *sort) {
    // DockWidget 容器
    sortDslDock = new QDockWidget("Sort DSL Panel", this);
    sortDslDock->setAllowedAreas(Qt::RightDockWidgetArea);
    sortDslDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sortDslDock->setVisible(false);

    // 容器
    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);

    // DSL 编辑器
    sortDslEditor = new QTextEdit(container);
    sortDslEditor->setPlaceholderText("Write your Sort DSL code here...");
    layout->addWidget(sortDslEditor);

    QPushButton *runButton = new QPushButton("Run Sort DSL", container);
    layout->addWidget(runButton);

    container->setLayout(layout);
    sortDslDock->setWidget(container);
    addDockWidget(Qt::RightDockWidgetArea, sortDslDock);

    // 高亮器
    sortDslHighlighter = new SortDSLSyntaxHighlighter(sortDslEditor->document());

    // 解释器
    auto *interpreter = new SortDSLInterpreter(sort, this);

    // Sort DSL 自动补全
    QStringList sortKeywords = {"let","for","end","insert","remove","clear","sort","sortBubble",
                                "sortInsertion","sortSelection","sortQuick","sortMerge","sortHeap","autoRun","stop","reset"};
    sortCompleter = new DSLCompleter(sortDslEditor, sortKeywords, this);

    connect(runButton, &QPushButton::clicked, this, [this, interpreter]() {
        interpreter->run(sortDslEditor->toPlainText());
    });
}



