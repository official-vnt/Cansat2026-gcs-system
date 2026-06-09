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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
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
    QWidget *topRibbon;
    QHBoxLayout *topRibbonLayout;
    QLabel *lblTeamIdBadge;
    QFrame *sep1;
    QLabel *label;
    QComboBox *portComboBox;
    QLabel *label_2;
    QComboBox *baudComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QFrame *sep2;
    QLabel *lblTimerCaption;
    QLabel *lblMissionTimer;
    QFrame *sep3;
    QLabel *lblLinkCaption;
    QProgressBar *linkQualityBar;
    QLabel *lblRssiCaption;
    QLabel *lblRssiVal;
    QLabel *lblSnrCaption;
    QLabel *lblSnrVal;
    QFrame *sep4;
    QLabel *statusLabel;
    QSpacerItem *ribbonSpacer;
    QHBoxLayout *mainContentLayout;
    QWidget *leftContainer;
    QVBoxLayout *leftOuterLayout;
    QGroupBox *leftGroupBox;
    QVBoxLayout *leftPanelLayout;
    QHBoxLayout *bottomLeftLayout;
    QGroupBox *loggingGroupBox;
    QVBoxLayout *loggingPanelLayout;
    QGroupBox *stateGroupBox;
    QVBoxLayout *statePanelLayout;
    QWidget *chartsContainer;
    QVBoxLayout *chartsVBoxLayout;
    QChartView *altVoltTimeChartView;
    QChartView *altTempPresChartView;
    QChartView *altAccelChartView;
    QChartView *altGyroChartView;
    QWidget *rightContainer;
    QVBoxLayout *rightOuterLayout;
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
    QGroupBox *commandGroupBox;
    QGridLayout *commandPanelLayout;
    QGroupBox *recoveryGroupBox;
    QVBoxLayout *recoveryPanelLayout;
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
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName("verticalLayout");
        topRibbon = new QWidget(centralwidget);
        topRibbon->setObjectName("topRibbon");
        topRibbon->setMinimumHeight(46);
        topRibbon->setMaximumHeight(46);
        topRibbonLayout = new QHBoxLayout(topRibbon);
        topRibbonLayout->setSpacing(6);
        topRibbonLayout->setObjectName("topRibbonLayout");
        topRibbonLayout->setContentsMargins(0, 0, 0, 0);
        lblTeamIdBadge = new QLabel(topRibbon);
        lblTeamIdBadge->setObjectName("lblTeamIdBadge");

        topRibbonLayout->addWidget(lblTeamIdBadge);

        sep1 = new QFrame(topRibbon);
        sep1->setObjectName("sep1");
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sep1);

        label = new QLabel(topRibbon);
        label->setObjectName("label");

        topRibbonLayout->addWidget(label);

        portComboBox = new QComboBox(topRibbon);
        portComboBox->setObjectName("portComboBox");
        portComboBox->setMaximumWidth(90);

        topRibbonLayout->addWidget(portComboBox);

        label_2 = new QLabel(topRibbon);
        label_2->setObjectName("label_2");

        topRibbonLayout->addWidget(label_2);

        baudComboBox = new QComboBox(topRibbon);
        baudComboBox->setObjectName("baudComboBox");
        baudComboBox->setMaximumWidth(90);

        topRibbonLayout->addWidget(baudComboBox);

        connectButton = new QPushButton(topRibbon);
        connectButton->setObjectName("connectButton");
        connectButton->setMaximumWidth(90);

        topRibbonLayout->addWidget(connectButton);

        disconnectButton = new QPushButton(topRibbon);
        disconnectButton->setObjectName("disconnectButton");
        disconnectButton->setEnabled(false);
        disconnectButton->setMaximumWidth(100);

        topRibbonLayout->addWidget(disconnectButton);

        sep2 = new QFrame(topRibbon);
        sep2->setObjectName("sep2");
        sep2->setFrameShape(QFrame::VLine);
        sep2->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sep2);

        lblTimerCaption = new QLabel(topRibbon);
        lblTimerCaption->setObjectName("lblTimerCaption");

        topRibbonLayout->addWidget(lblTimerCaption);

        lblMissionTimer = new QLabel(topRibbon);
        lblMissionTimer->setObjectName("lblMissionTimer");

        topRibbonLayout->addWidget(lblMissionTimer);

        sep3 = new QFrame(topRibbon);
        sep3->setObjectName("sep3");
        sep3->setFrameShape(QFrame::VLine);
        sep3->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sep3);

        lblLinkCaption = new QLabel(topRibbon);
        lblLinkCaption->setObjectName("lblLinkCaption");

        topRibbonLayout->addWidget(lblLinkCaption);

        linkQualityBar = new QProgressBar(topRibbon);
        linkQualityBar->setObjectName("linkQualityBar");
        linkQualityBar->setMinimumWidth(60);
        linkQualityBar->setMaximumWidth(60);
        linkQualityBar->setMaximumHeight(10);
        linkQualityBar->setValue(0);
        linkQualityBar->setTextVisible(false);

        topRibbonLayout->addWidget(linkQualityBar);

        lblRssiCaption = new QLabel(topRibbon);
        lblRssiCaption->setObjectName("lblRssiCaption");

        topRibbonLayout->addWidget(lblRssiCaption);

        lblRssiVal = new QLabel(topRibbon);
        lblRssiVal->setObjectName("lblRssiVal");

        topRibbonLayout->addWidget(lblRssiVal);

        lblSnrCaption = new QLabel(topRibbon);
        lblSnrCaption->setObjectName("lblSnrCaption");

        topRibbonLayout->addWidget(lblSnrCaption);

        lblSnrVal = new QLabel(topRibbon);
        lblSnrVal->setObjectName("lblSnrVal");

        topRibbonLayout->addWidget(lblSnrVal);

        sep4 = new QFrame(topRibbon);
        sep4->setObjectName("sep4");
        sep4->setFrameShape(QFrame::VLine);
        sep4->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sep4);

        statusLabel = new QLabel(topRibbon);
        statusLabel->setObjectName("statusLabel");

        topRibbonLayout->addWidget(statusLabel);

        ribbonSpacer = new QSpacerItem(5, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topRibbonLayout->addItem(ribbonSpacer);


        verticalLayout->addWidget(topRibbon);

        mainContentLayout = new QHBoxLayout();
        mainContentLayout->setSpacing(4);
        mainContentLayout->setObjectName("mainContentLayout");
        leftContainer = new QWidget(centralwidget);
        leftContainer->setObjectName("leftContainer");
        leftOuterLayout = new QVBoxLayout(leftContainer);
        leftOuterLayout->setSpacing(3);
        leftOuterLayout->setObjectName("leftOuterLayout");
        leftOuterLayout->setContentsMargins(0, 0, 0, 0);
        leftGroupBox = new QGroupBox(leftContainer);
        leftGroupBox->setObjectName("leftGroupBox");
        leftPanelLayout = new QVBoxLayout(leftGroupBox);
        leftPanelLayout->setSpacing(4);
        leftPanelLayout->setObjectName("leftPanelLayout");

        leftOuterLayout->addWidget(leftGroupBox);

        bottomLeftLayout = new QHBoxLayout();
        bottomLeftLayout->setObjectName("bottomLeftLayout");
        loggingGroupBox = new QGroupBox(leftContainer);
        loggingGroupBox->setObjectName("loggingGroupBox");
        loggingGroupBox->setMaximumHeight(175);
        loggingPanelLayout = new QVBoxLayout(loggingGroupBox);
        loggingPanelLayout->setSpacing(3);
        loggingPanelLayout->setObjectName("loggingPanelLayout");

        bottomLeftLayout->addWidget(loggingGroupBox);

        stateGroupBox = new QGroupBox(leftContainer);
        stateGroupBox->setObjectName("stateGroupBox");
        stateGroupBox->setMaximumHeight(175);
        statePanelLayout = new QVBoxLayout(stateGroupBox);
        statePanelLayout->setSpacing(3);
        statePanelLayout->setObjectName("statePanelLayout");

        bottomLeftLayout->addWidget(stateGroupBox);


        leftOuterLayout->addLayout(bottomLeftLayout);


        mainContentLayout->addWidget(leftContainer);

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
        rightOuterLayout = new QVBoxLayout(rightContainer);
        rightOuterLayout->setSpacing(3);
        rightOuterLayout->setObjectName("rightOuterLayout");
        rightOuterLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(rightContainer);
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


        rightOuterLayout->addWidget(groupBox);

        commandGroupBox = new QGroupBox(rightContainer);
        commandGroupBox->setObjectName("commandGroupBox");
        commandPanelLayout = new QGridLayout(commandGroupBox);
        commandPanelLayout->setSpacing(3);
        commandPanelLayout->setObjectName("commandPanelLayout");

        rightOuterLayout->addWidget(commandGroupBox);

        recoveryGroupBox = new QGroupBox(rightContainer);
        recoveryGroupBox->setObjectName("recoveryGroupBox");
        recoveryPanelLayout = new QVBoxLayout(recoveryGroupBox);
        recoveryPanelLayout->setSpacing(3);
        recoveryPanelLayout->setObjectName("recoveryPanelLayout");

        rightOuterLayout->addWidget(recoveryGroupBox);


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
        lblTeamIdBadge->setText(QCoreApplication::translate("MainWindow", "TEAM: 2026-INSPACe-CANSAT-021", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Baud:", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "CONNECT", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "DISCONNECT", nullptr));
        lblTimerCaption->setText(QCoreApplication::translate("MainWindow", "TIMER", nullptr));
        lblMissionTimer->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        lblLinkCaption->setText(QCoreApplication::translate("MainWindow", "LINK", nullptr));
        lblRssiCaption->setText(QCoreApplication::translate("MainWindow", "RSSI:", nullptr));
        lblRssiVal->setText(QCoreApplication::translate("MainWindow", "-- dBm", nullptr));
        lblSnrCaption->setText(QCoreApplication::translate("MainWindow", "SNR:", nullptr));
        lblSnrVal->setText(QCoreApplication::translate("MainWindow", "-- dB", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\342\227\213 Disconnected", nullptr));
        leftGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Mission Overview", nullptr));
        loggingGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Data Logging", nullptr));
        stateGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Cansat State", nullptr));
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
        commandGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Command Center", nullptr));
        recoveryGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Recovery Panel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
