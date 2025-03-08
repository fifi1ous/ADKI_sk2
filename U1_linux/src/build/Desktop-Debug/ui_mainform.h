/********************************************************************************
** Form generated from reading UI file 'mainform.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <draw.h>

QT_BEGIN_NAMESPACE

class Ui_MainForm
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionPoint_Polygon;
    QAction *actionClear_data;
    QAction *actionWinding_Number;
    QAction *actionRay_Crossing;
    QAction *actionClear_all;
    QAction *actionShapefile;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    Draw *Canvas;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuInput;
    QMenu *menuAnalyze;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainForm)
    {
        if (MainForm->objectName().isEmpty())
            MainForm->setObjectName("MainForm");
        MainForm->resize(1159, 775);
        actionOpen = new QAction(MainForm);
        actionOpen->setObjectName("actionOpen");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/open_file.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionOpen->setIcon(icon);
        actionExit = new QAction(MainForm);
        actionExit->setObjectName("actionExit");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/exit.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionExit->setIcon(icon1);
        actionPoint_Polygon = new QAction(MainForm);
        actionPoint_Polygon->setObjectName("actionPoint_Polygon");
        actionPoint_Polygon->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/pointpol.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionPoint_Polygon->setIcon(icon2);
        actionClear_data = new QAction(MainForm);
        actionClear_data->setObjectName("actionClear_data");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/clear.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionClear_data->setIcon(icon3);
        actionWinding_Number = new QAction(MainForm);
        actionWinding_Number->setObjectName("actionWinding_Number");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/winding.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionWinding_Number->setIcon(icon4);
        actionRay_Crossing = new QAction(MainForm);
        actionRay_Crossing->setObjectName("actionRay_Crossing");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/ray.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionRay_Crossing->setIcon(icon5);
        actionClear_all = new QAction(MainForm);
        actionClear_all->setObjectName("actionClear_all");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/clear_all.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionClear_all->setIcon(icon6);
        actionShapefile = new QAction(MainForm);
        actionShapefile->setObjectName("actionShapefile");
        centralwidget = new QWidget(MainForm);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        Canvas = new Draw(centralwidget);
        Canvas->setObjectName("Canvas");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Canvas->sizePolicy().hasHeightForWidth());
        Canvas->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(Canvas);

        MainForm->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainForm);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1159, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuInput = new QMenu(menubar);
        menuInput->setObjectName("menuInput");
        menuAnalyze = new QMenu(menubar);
        menuAnalyze->setObjectName("menuAnalyze");
        MainForm->setMenuBar(menubar);
        statusbar = new QStatusBar(MainForm);
        statusbar->setObjectName("statusbar");
        MainForm->setStatusBar(statusbar);
        toolBar = new QToolBar(MainForm);
        toolBar->setObjectName("toolBar");
        MainForm->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuInput->menuAction());
        menubar->addAction(menuAnalyze->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuFile->addAction(actionShapefile);
        menuInput->addAction(actionPoint_Polygon);
        menuInput->addSeparator();
        menuInput->addAction(actionClear_data);
        menuInput->addAction(actionClear_all);
        menuAnalyze->addAction(actionWinding_Number);
        menuAnalyze->addAction(actionRay_Crossing);
        toolBar->addAction(actionOpen);
        toolBar->addSeparator();
        toolBar->addAction(actionPoint_Polygon);
        toolBar->addSeparator();
        toolBar->addAction(actionWinding_Number);
        toolBar->addAction(actionRay_Crossing);
        toolBar->addSeparator();
        toolBar->addAction(actionClear_data);
        toolBar->addAction(actionClear_all);
        toolBar->addSeparator();
        toolBar->addAction(actionExit);
        toolBar->addSeparator();
        toolBar->addAction(actionShapefile);

        retranslateUi(MainForm);

        QMetaObject::connectSlotsByName(MainForm);
    } // setupUi

    void retranslateUi(QMainWindow *MainForm)
    {
        MainForm->setWindowTitle(QCoreApplication::translate("MainForm", "Analyze point and polyhon position", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainForm", "Open", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen->setToolTip(QCoreApplication::translate("MainForm", "Open a file", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExit->setText(QCoreApplication::translate("MainForm", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("MainForm", "Close application", nullptr));
#endif // QT_CONFIG(tooltip)
        actionPoint_Polygon->setText(QCoreApplication::translate("MainForm", "Point/Polygon", nullptr));
#if QT_CONFIG(tooltip)
        actionPoint_Polygon->setToolTip(QCoreApplication::translate("MainForm", "Input point or polygon", nullptr));
#endif // QT_CONFIG(tooltip)
        actionClear_data->setText(QCoreApplication::translate("MainForm", "Clear results", nullptr));
        actionWinding_Number->setText(QCoreApplication::translate("MainForm", "Winding Number", nullptr));
#if QT_CONFIG(tooltip)
        actionWinding_Number->setToolTip(QCoreApplication::translate("MainForm", "Analyze point and polygon position using Winding Number", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRay_Crossing->setText(QCoreApplication::translate("MainForm", "Ray Crossing", nullptr));
#if QT_CONFIG(tooltip)
        actionRay_Crossing->setToolTip(QCoreApplication::translate("MainForm", "Analyze point and polygon position using Ray Crossing", nullptr));
#endif // QT_CONFIG(tooltip)
        actionClear_all->setText(QCoreApplication::translate("MainForm", "Clear all", nullptr));
#if QT_CONFIG(tooltip)
        actionClear_all->setToolTip(QCoreApplication::translate("MainForm", "Clear all data and results", nullptr));
#endif // QT_CONFIG(tooltip)
        actionShapefile->setText(QCoreApplication::translate("MainForm", "Shapefile", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainForm", "File", nullptr));
        menuInput->setTitle(QCoreApplication::translate("MainForm", "Input", nullptr));
        menuAnalyze->setTitle(QCoreApplication::translate("MainForm", "Analyze", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainForm", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainForm: public Ui_MainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
