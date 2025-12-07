#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwidget.h"
#include "graphpanel.h"
#include "sortwidget.h"
#include "sortpanel.h"
#include "graphDSL.h"
#include "sortDSL.h"
#include "helpwindow.h"
#include "aiassistant.h"
#include "DSLCompleter.h"
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createGraphDSLPanel(GraphWidget *graph);
    void createSortDSLPanel(SortWidget *sort);

private:
    Ui::MainWindow *ui;
    QString lastfilename;
    QDockWidget *graphDslDock;
    QTextEdit *graphDslEditor;
    GraphDSLSyntaxHighlighter *graphDslHighlighter;

    QDockWidget *sortDslDock;
    QTextEdit *sortDslEditor;
    SortDSLSyntaxHighlighter *sortDslHighlighter;
    AIAssistant *ai=new AIAssistant(this);

    DSLCompleter *graphCompleter;
    DSLCompleter *sortCompleter;

    QString sortdoc="SortDSL 是一个用于可视化排序操作的专用 DSL，支持数组节点操作、排序算法、变量定义和循环控制。请严格按照以下语法生成代码，不要输出解释。\n"
                    "\n"
                    "1. 变量定义\n"
                    "-----------------\n"
                    "语法：\n"
                    "let <变量名> = <整数值>\n"
                    "\n"
                    "说明：\n"
                    "- 定义整数变量，可在命令参数中使用\n"
                    "- 示例：\n"
                    "let n = 5\n"
                    "let val = 10\n"
                    "\n"
                    "2. 循环\n"
                    "-----------------\n"
                    "语法：\n"
                    "for <变量> in range(<起始>,<结束>)\n"
                    "    ...循环体...\n"
                    "end\n"
                    "\n"
                    "说明：\n"
                    "- 循环体内部可使用循环变量\n"
                    "- 示例：\n"
                    "for i in range(0, 5)\n"
                    "    insert(i)\n"
                    "end\n"
                    "\n"
                    "3. 节点操作\n"
                    "-----------------\n"
                    "命令：\n"
                    "insert(<值>)             // 在末尾插入节点\n"
                    "insert(<位置>, <值>)     // 在指定位置插入节点\n"
                    "remove(<位置>)           // 删除指定位置节点\n"
                    "clear()                  // 清空所有节点\n"
                    "\n"
                    "4. 排序操作\n"
                    "-----------------\n"
                    "命令：\n"
                    "sort                  // 按当前方法排序\n"
                    "sortBubble            // 冒泡排序\n"
                    "sortInsertion         // 插入排序\n"
                    "sortSelection         // 选择排序\n"
                    "sortQuick             // 快速排序\n"
                    "sortMerge             // 归并排序\n"
                    "sortHeap              // 堆排序\n"
                    "setAscending(true/false) // 设置升序或降序\n"
                    "\n"
                    "5. 自动运行\n"
                    "-----------------\n"
                    "命令：\n"
                    "autoRun(<间隔>)   // 自动播放排序动画，间隔单位毫秒\n"
                    "stop()             // 停止自动播放\n"
                    "reset()            // 重置排序状态\n"
                    "\n"
                    "6. 注释\n"
                    "-----------------\n"
                    "语法：\n"
                    "# 这是注释\n"
                    "\n"
                    "说明：\n"
                    "- 注释以 # 开头，可放在任何位置\n"
                    "\n"
                    "7. 变量替换\n"
                    "-----------------\n"
                    "- 命令参数中可以使用 let 定义的变量\n"
                    "- 示例：\n"
                    "let pos = 0\n"
                    "let val = 5\n"
                    "insert(pos, val)\n"
                    "\n"
                    "8. 注意事项\n"
                    "-----------------\n"
                    "- 每条命令必须独占一行\n"
                    "- 循环必须以 end 结束\n"
                    "- 节点位置和数值必须为整数\n"
                    "- AI 生成的代码仅返回 DSL 本身，不输出解释";
    QString graphdoc="GraphDSL 是一个用于操作图(Graph)的专用 DSL，支持节点、边、图算法以及变量和循环控制。请根据提示生成 DSL 代码，代码示例必须严格遵守下面语法规则。\n"
                     "\n"
                     "1. 变量定义\n"
                     "-----------------\n"
                     "语法：\n"
                     "let <变量名> = <整数值>\n"
                     "\n"
                     "说明：\n"
                     "- 定义一个整数变量，可在后续命令中使用\n"
                     "- 示例：\n"
                     "let n = 5\n"
                     "let startNode = 0\n"
                     "\n"
                     "2. 循环\n"
                     "-----------------\n"
                     "语法：\n"
                     "for <变量> in range(<起始>,<结束>)\n"
                     "    ...循环体...\n"
                     "end\n"
                     "\n"
                     "说明：\n"
                     "- 循环体内部可以使用循环变量\n"
                     "- 示例：\n"
                     "for i in range(0, 5)\n"
                     "    insertNode()\n"
                     "end\n"
                     "\n"
                     "3. 节点操作\n"
                     "-----------------\n"
                     "命令：\n"
                     "insertNode()         // 插入一个新节点\n"
                     "deleteNode(<id>)     // 删除节点，<id> 为节点编号\n"
                     "\n"
                     "4. 边操作\n"
                     "-----------------\n"
                     "命令：\n"
                     "addEdge(<src>,<dst>,<weight>)     // 添加一条带权边\n"
                     "deleteEdge(<src>,<dst>)           // 删除一条边\n"
                     "\n"
                     "5. 图清空\n"
                     "-----------------\n"
                     "命令：\n"
                     "clear()  // 清空图中所有节点和边\n"
                     "\n"
                     "6. 图算法\n"
                     "-----------------\n"
                     "命令：\n"
                     "bfs(<startNode>)        // 从指定节点执行 BFS\n"
                     "dfs(<startNode>)        // 从指定节点执行 DFS\n"
                     "prim()                  // 执行 Prim 最小生成树算法\n"
                     "kruskal()               // 执行 Kruskal 最小生成树算法\n"
                     "dijkstra(<startNode>)   // 从指定节点执行 Dijkstra 最短路径算法\n"
                     "\n"
                     "7. 随机图生成\n"
                     "-----------------\n"
                     "命令：\n"
                     "random(<nodeCount>, <edgeCount>, <maxWeight>)  \n"
                     "- nodeCount: 节点数量  \n"
                     "- edgeCount: 边数量  \n"
                     "- maxWeight: 权重最大值（浮点数）  \n"
                     "示例：\n"
                     "random(5, 7, 10.0)\n"
                     "\n"
                     "8. 注释\n"
                     "-----------------\n"
                     "语法：\n"
                     "# 这是注释\n"
                     "\n"
                     "说明：\n"
                     "- 注释以 # 开头\n"
                     "- 可放在任何位置，AI 生成代码时可忽略\n"
                     "\n"
                     "9. 变量替换\n"
                     "-----------------\n"
                     "- 命令参数中可以使用变量，如：\n"
                     "let a = 3\n"
                     "addEdge(a, 2, 5)\n"
                     "\n"
                     "10. 注意事项\n"
                     "-----------------\n"
                     "- 每条命令必须占一行\n"
                     "- 循环必须以 end 结束\n"
                     "- 所有节点编号为整数\n"
                     "- AI 生成的代码仅返回 DSL 本身，不输出解释";
};
#endif // MAINWINDOW_H
