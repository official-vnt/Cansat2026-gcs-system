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
    QHBoxLayout *topBarLayout;
    QLabel *label;
    QComboBox *portComboBox;
    QLabel *label_2;
    QComboBox *baudComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QSpacerItem *horizontalSpacer;
    QLabel *statusLabel;
    QHBoxLayout *mainContentLayout;
    QGroupBox *leftGroupBox;
    QVBoxLayout *leftPanelLayout;
    QWidget *chartsContainer;
    QVBoxLayout *chartsVBoxLayout;
    QChartView *altVoltTimeChartView;
    QChartView *altTempPresChartView;
    QChartView *altAccelChartView;
    QChartView *altGyroChartView;
    QWidget *rightContainer;
    QVBoxLayout *rightPanelLayout;
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
    QSpacerItem *rightVerticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1440, 900);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topBarLayout = new QHBoxLayout();
        topBarLayout->setSpacing(8);
        topBarLayout->setObjectName("topBarLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        topBarLayout->addWidget(label);

        portComboBox = new QComboBox(centralwidget);
        portComboBox->setObjectName("portComboBox");

        topBarLayout->addWidget(portComboBox);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        topBarLayout->addWidget(label_2);

        baudComboBox = new QComboBox(centralwidget);
        baudComboBox->setObjectName("baudComboBox");

        topBarLayout->addWidget(baudComboBox);

        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");

        topBarLayout->addWidget(connectButton);

        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName("disconnectButton");
        disconnectButton->setEnabled(false);

        topBarLayout->addWidget(disconnectButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(horizontalSpacer);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        topBarLayout->addWidget(statusLabel);


        verticalLayout->addLayout(topBarLayout);

        mainContentLayout = new QHBoxLayout();
        mainContentLayout->setSpacing(4);
        mainContentLayout->setObjectName("mainContentLayout");
        leftGroupBox = new QGroupBox(centralwidget);
        leftGroupBox->setObjectName("leftGroupBox");
        leftGroupBox->setMinimumSize(QSize(260, 0));
        leftGroupBox->setMaximumSize(QSize(500, 16777215));
        leftPanelLayout = new QVBoxLayout(leftGroupBox);
        leftPanelLayout->setSpacing(8);
        leftPanelLayout->setObjectName("leftPanelLayout");

        mainContentLayout->addWidget(leftGroupBox);

        chartsContainer = new QWidget(centralwidget);
        chartsContainer->setObjectName("chartsContainer");
        chartsVBoxLayout = new QVBoxLayout(chartsContainer);
        chartsVBoxLayout->setSpacing(4);
        chartsVBoxLayout->setObjectName("chartsVBoxLayout");
        chartsVBoxLayout->setContentsMargins(0, 0, 0, 0);
        altVoltTimeChartView = new QChartView(chartsContainer);
        altVoltTimeChartView->setObjectName("altVoltTimeChartView");

        chartsVBoxLayout->addWidget(altVoltTimeChartView);

        altTempPresChartView = new QChartView(chartsContainer);
        altTempPresChartView->setObjectName("altTempPresChartView");

        chartsVBoxLayout->addWidget(altTempPresChartView);

        altAccelChartView = new QChartView(chartsContainer);
        altAccelChartView->setObjectName("altAccelChartView");

        chartsVBoxLayout->addWidget(altAccelChartView);

        altGyroChartView = new QChartView(chartsContainer);
        altGyroChartView->setObjectName("altGyroChartView");

        chartsVBoxLayout->addWidget(altGyroChartView);

        chartsVBoxLayout->setStretch(0, 1);
        chartsVBoxLayout->setStretch(1, 1);
        chartsVBoxLayout->setStretch(2, 1);
        chartsVBoxLayout->setStretch(3, 1);

        mainContentLayout->addWidget(chartsContainer);

        rightContainer = new QWidget(centralwidget);
        rightContainer->setObjectName("rightContainer");
        rightPanelLayout = new QVBoxLayout(rightContainer);
        rightPanelLayout->setSpacing(0);
        rightPanelLayout->setObjectName("rightPanelLayout");
        rightPanelLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(rightContainer);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(260, 0));
        groupBox->setMaximumSize(QSize(500, 16777215));
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


        rightPanelLayout->addWidget(groupBox);

        rightVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rightPanelLayout->addItem(rightVerticalSpacer);


        mainContentLayout->addWidget(rightContainer);

        mainContentLayout->setStretch(0, 3);
        mainContentLayout->setStretch(1, 4);
        mainContentLayout->setStretch(2, 3);

        verticalLayout->addLayout(mainContentLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1440, 22));
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
        leftGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Mission Overview", nullptr));
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
