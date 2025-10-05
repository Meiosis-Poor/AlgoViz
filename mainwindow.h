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
    // ===== Í¼ DSL ±à¼­Çø =====
    QDockWidget *graphDslDock;
    QTextEdit *graphDslEditor;
    GraphDSLSyntaxHighlighter *graphDslHighlighter;

    // ===== ÅÅÐò DSL ±à¼­Çø =====
    QDockWidget *sortDslDock;
    QTextEdit *sortDslEditor;
    SortDSLSyntaxHighlighter *sortDslHighlighter;
};
#endif // MAINWINDOW_H
