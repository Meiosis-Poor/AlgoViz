/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *sortbtn;
    QAction *graphbtn;
    QAction *openbtn;
    QAction *savebtn;
    QAction *saveotherbtn;
    QAction *resetbtn;
    QAction *randombtn;
    QAction *actionMade_by_Meiosis;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        sortbtn = new QAction(MainWindow);
        sortbtn->setObjectName("sortbtn");
        graphbtn = new QAction(MainWindow);
        graphbtn->setObjectName("graphbtn");
        openbtn = new QAction(MainWindow);
        openbtn->setObjectName("openbtn");
        savebtn = new QAction(MainWindow);
        savebtn->setObjectName("savebtn");
        saveotherbtn = new QAction(MainWindow);
        saveotherbtn->setObjectName("saveotherbtn");
        resetbtn = new QAction(MainWindow);
        resetbtn->setObjectName("resetbtn");
        randombtn = new QAction(MainWindow);
        randombtn->setObjectName("randombtn");
        actionMade_by_Meiosis = new QAction(MainWindow);
        actionMade_by_Meiosis->setObjectName("actionMade_by_Meiosis");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(sortbtn);
        menu->addAction(graphbtn);
        menu_2->addAction(openbtn);
        menu_2->addAction(savebtn);
        menu_2->addAction(saveotherbtn);
        menu_3->addAction(resetbtn);
        menu_3->addAction(randombtn);
        menu_4->addAction(actionMade_by_Meiosis);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        sortbtn->setText(QCoreApplication::translate("MainWindow", "\346\216\222\345\272\217\346\250\241\345\274\217", nullptr));
        graphbtn->setText(QCoreApplication::translate("MainWindow", "\345\233\276\346\250\241\345\274\217", nullptr));
        openbtn->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        savebtn->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        saveotherbtn->setText(QCoreApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", nullptr));
        resetbtn->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
        randombtn->setText(QCoreApplication::translate("MainWindow", "\351\232\217\346\234\272\346\211\223\344\271\261", nullptr));
        actionMade_by_Meiosis->setText(QCoreApplication::translate("MainWindow", "Made by Meiosis", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\250\241\345\274\217\345\210\207\346\215\242", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
