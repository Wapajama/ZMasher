/********************************************************************************
** Form generated from reading UI file 'modelviewerwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWERWINDOW_H
#define UI_MODELVIEWERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelViewerWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *splitter;
    QGraphicsView *graphicsView;
    QWidget *PropertyView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerWindow)
    {
        if (ModelViewerWindow->objectName().isEmpty())
            ModelViewerWindow->setObjectName(QString::fromUtf8("ModelViewerWindow"));
        ModelViewerWindow->resize(954, 638);
        centralWidget = new QWidget(ModelViewerWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        graphicsView = new QGraphicsView(splitter);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        splitter->addWidget(graphicsView);
        PropertyView = new QWidget(splitter);
        PropertyView->setObjectName(QString::fromUtf8("PropertyView"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(PropertyView->sizePolicy().hasHeightForWidth());
        PropertyView->setSizePolicy(sizePolicy1);
        splitter->addWidget(PropertyView);

        horizontalLayout_2->addWidget(splitter);

        ModelViewerWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 954, 21));
        ModelViewerWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ModelViewerWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelViewerWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ModelViewerWindow->setStatusBar(statusBar);

        retranslateUi(ModelViewerWindow);

        QMetaObject::connectSlotsByName(ModelViewerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerWindow)
    {
        ModelViewerWindow->setWindowTitle(QApplication::translate("ModelViewerWindow", "ModelViewerWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerWindow: public Ui_ModelViewerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWERWINDOW_H
