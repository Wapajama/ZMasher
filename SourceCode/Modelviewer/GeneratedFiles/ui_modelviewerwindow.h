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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelViewerWindow
{
public:
    QAction *actionLoad_File;
    QAction *actionLoad_Model;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTabWidget *PropertyView;
    QWidget *PropertyViewPage1;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *cameraPosLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *cameraPosX;
    QLabel *cameraPosY;
    QLabel *cameraPosZ;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *ModelViewerWindow)
    {
        if (ModelViewerWindow->objectName().isEmpty())
            ModelViewerWindow->setObjectName(QString::fromUtf8("ModelViewerWindow"));
        ModelViewerWindow->resize(995, 638);
        actionLoad_File = new QAction(ModelViewerWindow);
        actionLoad_File->setObjectName(QString::fromUtf8("actionLoad_File"));
        actionLoad_Model = new QAction(ModelViewerWindow);
        actionLoad_Model->setObjectName(QString::fromUtf8("actionLoad_Model"));
        centralWidget = new QWidget(ModelViewerWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        graphicsView = new QWidget(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(graphicsView);

        ModelViewerWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 995, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        ModelViewerWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ModelViewerWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelViewerWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ModelViewerWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(ModelViewerWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        horizontalLayout = new QHBoxLayout(dockWidgetContents);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        PropertyView = new QTabWidget(dockWidgetContents);
        PropertyView->setObjectName(QString::fromUtf8("PropertyView"));
        PropertyView->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(PropertyView->sizePolicy().hasHeightForWidth());
        PropertyView->setSizePolicy(sizePolicy1);
        PropertyViewPage1 = new QWidget();
        PropertyViewPage1->setObjectName(QString::fromUtf8("PropertyViewPage1"));
        verticalLayout_3 = new QVBoxLayout(PropertyViewPage1);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        cameraPosLabel = new QLabel(PropertyViewPage1);
        cameraPosLabel->setObjectName(QString::fromUtf8("cameraPosLabel"));

        verticalLayout->addWidget(cameraPosLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        cameraPosX = new QLabel(PropertyViewPage1);
        cameraPosX->setObjectName(QString::fromUtf8("cameraPosX"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cameraPosX->sizePolicy().hasHeightForWidth());
        cameraPosX->setSizePolicy(sizePolicy2);
        cameraPosX->setMinimumSize(QSize(43, 13));
        cameraPosX->setMaximumSize(QSize(43, 16777215));

        horizontalLayout_3->addWidget(cameraPosX);

        cameraPosY = new QLabel(PropertyViewPage1);
        cameraPosY->setObjectName(QString::fromUtf8("cameraPosY"));
        sizePolicy2.setHeightForWidth(cameraPosY->sizePolicy().hasHeightForWidth());
        cameraPosY->setSizePolicy(sizePolicy2);
        cameraPosY->setMinimumSize(QSize(43, 13));
        cameraPosY->setMaximumSize(QSize(43, 16777215));

        horizontalLayout_3->addWidget(cameraPosY);

        cameraPosZ = new QLabel(PropertyViewPage1);
        cameraPosZ->setObjectName(QString::fromUtf8("cameraPosZ"));
        sizePolicy2.setHeightForWidth(cameraPosZ->sizePolicy().hasHeightForWidth());
        cameraPosZ->setSizePolicy(sizePolicy2);
        cameraPosZ->setMinimumSize(QSize(43, 13));
        cameraPosZ->setMaximumSize(QSize(43, 16777215));

        horizontalLayout_3->addWidget(cameraPosZ);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 463, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);

        PropertyView->addTab(PropertyViewPage1, QString());

        horizontalLayout->addWidget(PropertyView);

        dockWidget->setWidget(dockWidgetContents);
        ModelViewerWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad_Model);

        retranslateUi(ModelViewerWindow);

        QMetaObject::connectSlotsByName(ModelViewerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerWindow)
    {
        ModelViewerWindow->setWindowTitle(QApplication::translate("ModelViewerWindow", "ModelViewerWindow", nullptr));
        actionLoad_File->setText(QApplication::translate("ModelViewerWindow", "Load File..", nullptr));
        actionLoad_Model->setText(QApplication::translate("ModelViewerWindow", "Load Model", nullptr));
#ifndef QT_NO_SHORTCUT
        actionLoad_Model->setShortcut(QApplication::translate("ModelViewerWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        menuFile->setTitle(QApplication::translate("ModelViewerWindow", "File..", nullptr));
        cameraPosLabel->setText(QApplication::translate("ModelViewerWindow", "Camera position", nullptr));
        cameraPosX->setText(QApplication::translate("ModelViewerWindow", "x: 12345", nullptr));
        cameraPosY->setText(QApplication::translate("ModelViewerWindow", "x: 12345", nullptr));
        cameraPosZ->setText(QApplication::translate("ModelViewerWindow", "x: 12345", nullptr));
        PropertyView->setTabText(PropertyView->indexOf(PropertyViewPage1), QString());
    } // retranslateUi

};

namespace Ui {
    class ModelViewerWindow: public Ui_ModelViewerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWERWINDOW_H
