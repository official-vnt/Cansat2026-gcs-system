/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *portComboBox;
    QLabel *label_2;
    QComboBox *baudComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QSpacerItem *horizontalSpacer;
    QLabel *statusLabel;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_team;
    QLabel *lblTeamId;
    QLabel *label_3;
    QLabel *lblPacketId;
    QLabel *label_4;
    QLabel *lblTimestamp;
    QLabel *label_5;
    QLabel *lblVoltage;
    QLabel *label_6;
    QLabel *lblGps;
    QLabel *label_7;
    QLabel *lblSats;
    QLabel *label_8;
    QLabel *lblState;
    QVBoxLayout *verticalLayout_2;
    QChartView *altChartView;
    QChartView *tempChartView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 768);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        portComboBox = new QComboBox(centralwidget);
        portComboBox->setObjectName("portComboBox");

        horizontalLayout->addWidget(portComboBox);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        baudComboBox = new QComboBox(centralwidget);
        baudComboBox->setObjectName("baudComboBox");

        horizontalLayout->addWidget(baudComboBox);

        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");

        horizontalLayout->addWidget(connectButton);

        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName("disconnectButton");
        disconnectButton->setEnabled(false);

        horizontalLayout->addWidget(disconnectButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        horizontalLayout->addWidget(statusLabel);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label_team = new QLabel(groupBox);
        label_team->setObjectName("label_team");

        gridLayout->addWidget(label_team, 0, 0, 1, 1);

        lblTeamId = new QLabel(groupBox);
        lblTeamId->setObjectName("lblTeamId");

        gridLayout->addWidget(lblTeamId, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lblPacketId = new QLabel(groupBox);
        lblPacketId->setObjectName("lblPacketId");

        gridLayout->addWidget(lblPacketId, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lblTimestamp = new QLabel(groupBox);
        lblTimestamp->setObjectName("lblTimestamp");

        gridLayout->addWidget(lblTimestamp, 2, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        lblVoltage = new QLabel(groupBox);
        lblVoltage->setObjectName("lblVoltage");

        gridLayout->addWidget(lblVoltage, 3, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        lblGps = new QLabel(groupBox);
        lblGps->setObjectName("lblGps");

        gridLayout->addWidget(lblGps, 4, 1, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        lblSats = new QLabel(groupBox);
        lblSats->setObjectName("lblSats");

        gridLayout->addWidget(lblSats, 5, 1, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 6, 0, 1, 1);

        lblState = new QLabel(groupBox);
        lblState->setObjectName("lblState");

        gridLayout->addWidget(lblState, 6, 1, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        altChartView = new QChartView(centralwidget);
        altChartView->setObjectName("altChartView");

        verticalLayout_2->addWidget(altChartView);

        tempChartView = new QChartView(centralwidget);
        tempChartView->setObjectName("tempChartView");

        verticalLayout_2->addWidget(tempChartView);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1024, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GCS Telemetry Dashboard", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "COM Port:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Baud Rate:", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Disconnected", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Telemetry Data", nullptr));
        label_team->setText(QCoreApplication::translate("MainWindow", "Team ID:", nullptr));
        lblTeamId->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Packet Count:", nullptr));
        lblPacketId->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Timestamp:", nullptr));
        lblTimestamp->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Voltage:", nullptr));
        lblVoltage->setText(QCoreApplication::translate("MainWindow", "0 V", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "GPS (Lat, Lon):", nullptr));
        lblGps->setText(QCoreApplication::translate("MainWindow", "0, 0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Satellites:", nullptr));
        lblSats->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "State:", nullptr));
        lblState->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
