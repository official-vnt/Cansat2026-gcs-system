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
    QVBoxLayout *rootLayout;
    QWidget *topRibbon;
    QHBoxLayout *topRibbonLayout;
    QWidget *titleBlock;
    QVBoxLayout *titleBlockLayout;
    QLabel *lblAppTitle;
    QLabel *lblAppSubtitle;
    QFrame *sepA;
    QWidget *teamBlock;
    QVBoxLayout *teamBlockLayout;
    QLabel *lblTeamCaption;
    QLabel *lblTeamIdBadge;
    QFrame *sepB;
    QWidget *portBlock;
    QVBoxLayout *portBlockLayout;
    QLabel *lblPortCaption;
    QComboBox *portComboBox;
    QWidget *baudBlock;
    QVBoxLayout *baudBlockLayout;
    QLabel *lblBaudCaption;
    QComboBox *baudComboBox;
    QWidget *connBlock;
    QVBoxLayout *connBlockLayout;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QFrame *sepC;
    QWidget *connStatusBlock;
    QVBoxLayout *connStatusLayout;
    QLabel *lblConnCaption;
    QLabel *statusLabel;
    QFrame *sepD;
    QWidget *linkBlock;
    QVBoxLayout *linkBlockLayout;
    QLabel *lblLinkCaption;
    QHBoxLayout *hboxLayout;
    QProgressBar *linkQualityBar;
    QLabel *lblLinkQualText;
    QHBoxLayout *hboxLayout1;
    QLabel *lblRssiCaption;
    QLabel *lblRssiVal;
    QLabel *lblSnrCaption;
    QLabel *lblSnrVal;
    QFrame *sepE;
    QWidget *timerBlock;
    QVBoxLayout *timerBlockLayout;
    QLabel *lblTimerCaption;
    QLabel *lblMissionTimer;
    QFrame *sepF;
    QWidget *recBlock;
    QVBoxLayout *recBlockLayout;
    QLabel *lblRecCaption;
    QLabel *lblRecStatus;
    QSpacerItem *ribbonSpacer;
    QHBoxLayout *mainContentLayout;
    QWidget *leftContainer;
    QVBoxLayout *leftOuterLayout;
    QGroupBox *leftGroupBox;
    QVBoxLayout *leftPanelLayout;
    QGroupBox *stateGroupBox;
    QVBoxLayout *statePanelLayout;
    QGroupBox *loggingGroupBox;
    QVBoxLayout *loggingPanelLayout;
    QWidget *centerContainer;
    QVBoxLayout *centerLayout;
    QGroupBox *chartsGroupBox;
    QGridLayout *chartsGridLayout;
    QChartView *altVoltTimeChartView;
    QChartView *altTempPresChartView;
    QChartView *altAccelChartView;
    QChartView *altGyroChartView;
    QGroupBox *commandGroupBox;
    QGridLayout *commandPanelLayout;
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
    QGroupBox *systemStatusBox;
    QVBoxLayout *systemStatusLayout;
    QGroupBox *alertsBox;
    QVBoxLayout *alertsLayout;
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
        rootLayout = new QVBoxLayout(centralwidget);
        rootLayout->setSpacing(0);
        rootLayout->setContentsMargins(0, 0, 0, 0);
        rootLayout->setObjectName("rootLayout");
        topRibbon = new QWidget(centralwidget);
        topRibbon->setObjectName("topRibbon");
        topRibbon->setMinimumHeight(56);
        topRibbon->setMaximumHeight(56);
        topRibbonLayout = new QHBoxLayout(topRibbon);
        topRibbonLayout->setSpacing(0);
        topRibbonLayout->setObjectName("topRibbonLayout");
        topRibbonLayout->setContentsMargins(0, 0, 12, 0);
        titleBlock = new QWidget(topRibbon);
        titleBlock->setObjectName("titleBlock");
        titleBlock->setMinimumWidth(200);
        titleBlock->setMaximumWidth(220);
        titleBlockLayout = new QVBoxLayout(titleBlock);
        titleBlockLayout->setSpacing(1);
        titleBlockLayout->setObjectName("titleBlockLayout");
        titleBlockLayout->setContentsMargins(12, 6, 12, 4);
        lblAppTitle = new QLabel(titleBlock);
        lblAppTitle->setObjectName("lblAppTitle");

        titleBlockLayout->addWidget(lblAppTitle);

        lblAppSubtitle = new QLabel(titleBlock);
        lblAppSubtitle->setObjectName("lblAppSubtitle");

        titleBlockLayout->addWidget(lblAppSubtitle);


        topRibbonLayout->addWidget(titleBlock);

        sepA = new QFrame(topRibbon);
        sepA->setObjectName("sepA");
        sepA->setFrameShape(QFrame::VLine);
        sepA->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepA);

        teamBlock = new QWidget(topRibbon);
        teamBlock->setObjectName("teamBlock");
        teamBlockLayout = new QVBoxLayout(teamBlock);
        teamBlockLayout->setSpacing(1);
        teamBlockLayout->setObjectName("teamBlockLayout");
        teamBlockLayout->setContentsMargins(10, 6, 10, 4);
        lblTeamCaption = new QLabel(teamBlock);
        lblTeamCaption->setObjectName("lblTeamCaption");

        teamBlockLayout->addWidget(lblTeamCaption);

        lblTeamIdBadge = new QLabel(teamBlock);
        lblTeamIdBadge->setObjectName("lblTeamIdBadge");

        teamBlockLayout->addWidget(lblTeamIdBadge);


        topRibbonLayout->addWidget(teamBlock);

        sepB = new QFrame(topRibbon);
        sepB->setObjectName("sepB");
        sepB->setFrameShape(QFrame::VLine);
        sepB->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepB);

        portBlock = new QWidget(topRibbon);
        portBlock->setObjectName("portBlock");
        portBlockLayout = new QVBoxLayout(portBlock);
        portBlockLayout->setSpacing(1);
        portBlockLayout->setObjectName("portBlockLayout");
        portBlockLayout->setContentsMargins(8, 6, 8, 4);
        lblPortCaption = new QLabel(portBlock);
        lblPortCaption->setObjectName("lblPortCaption");

        portBlockLayout->addWidget(lblPortCaption);

        portComboBox = new QComboBox(portBlock);
        portComboBox->setObjectName("portComboBox");
        portComboBox->setMaximumWidth(85);

        portBlockLayout->addWidget(portComboBox);


        topRibbonLayout->addWidget(portBlock);

        baudBlock = new QWidget(topRibbon);
        baudBlock->setObjectName("baudBlock");
        baudBlockLayout = new QVBoxLayout(baudBlock);
        baudBlockLayout->setSpacing(1);
        baudBlockLayout->setObjectName("baudBlockLayout");
        baudBlockLayout->setContentsMargins(8, 6, 8, 4);
        lblBaudCaption = new QLabel(baudBlock);
        lblBaudCaption->setObjectName("lblBaudCaption");

        baudBlockLayout->addWidget(lblBaudCaption);

        baudComboBox = new QComboBox(baudBlock);
        baudComboBox->setObjectName("baudComboBox");
        baudComboBox->setMaximumWidth(85);

        baudBlockLayout->addWidget(baudComboBox);


        topRibbonLayout->addWidget(baudBlock);

        connBlock = new QWidget(topRibbon);
        connBlock->setObjectName("connBlock");
        connBlockLayout = new QVBoxLayout(connBlock);
        connBlockLayout->setSpacing(2);
        connBlockLayout->setObjectName("connBlockLayout");
        connBlockLayout->setContentsMargins(6, 8, 6, 6);
        connectButton = new QPushButton(connBlock);
        connectButton->setObjectName("connectButton");

        connBlockLayout->addWidget(connectButton);

        disconnectButton = new QPushButton(connBlock);
        disconnectButton->setObjectName("disconnectButton");
        disconnectButton->setEnabled(false);

        connBlockLayout->addWidget(disconnectButton);


        topRibbonLayout->addWidget(connBlock);

        sepC = new QFrame(topRibbon);
        sepC->setObjectName("sepC");
        sepC->setFrameShape(QFrame::VLine);
        sepC->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepC);

        connStatusBlock = new QWidget(topRibbon);
        connStatusBlock->setObjectName("connStatusBlock");
        connStatusLayout = new QVBoxLayout(connStatusBlock);
        connStatusLayout->setSpacing(1);
        connStatusLayout->setObjectName("connStatusLayout");
        connStatusLayout->setContentsMargins(10, 6, 10, 4);
        lblConnCaption = new QLabel(connStatusBlock);
        lblConnCaption->setObjectName("lblConnCaption");

        connStatusLayout->addWidget(lblConnCaption);

        statusLabel = new QLabel(connStatusBlock);
        statusLabel->setObjectName("statusLabel");

        connStatusLayout->addWidget(statusLabel);


        topRibbonLayout->addWidget(connStatusBlock);

        sepD = new QFrame(topRibbon);
        sepD->setObjectName("sepD");
        sepD->setFrameShape(QFrame::VLine);
        sepD->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepD);

        linkBlock = new QWidget(topRibbon);
        linkBlock->setObjectName("linkBlock");
        linkBlockLayout = new QVBoxLayout(linkBlock);
        linkBlockLayout->setSpacing(2);
        linkBlockLayout->setObjectName("linkBlockLayout");
        linkBlockLayout->setContentsMargins(10, 6, 10, 4);
        lblLinkCaption = new QLabel(linkBlock);
        lblLinkCaption->setObjectName("lblLinkCaption");

        linkBlockLayout->addWidget(lblLinkCaption);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName("hboxLayout");
        linkQualityBar = new QProgressBar(linkBlock);
        linkQualityBar->setObjectName("linkQualityBar");
        linkQualityBar->setMinimumWidth(70);
        linkQualityBar->setMaximumWidth(70);
        linkQualityBar->setMaximumHeight(8);
        linkQualityBar->setValue(0);
        linkQualityBar->setTextVisible(false);

        hboxLayout->addWidget(linkQualityBar);

        lblLinkQualText = new QLabel(linkBlock);
        lblLinkQualText->setObjectName("lblLinkQualText");

        hboxLayout->addWidget(lblLinkQualText);


        linkBlockLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(4);
        hboxLayout1->setObjectName("hboxLayout1");
        lblRssiCaption = new QLabel(linkBlock);
        lblRssiCaption->setObjectName("lblRssiCaption");

        hboxLayout1->addWidget(lblRssiCaption);

        lblRssiVal = new QLabel(linkBlock);
        lblRssiVal->setObjectName("lblRssiVal");

        hboxLayout1->addWidget(lblRssiVal);

        lblSnrCaption = new QLabel(linkBlock);
        lblSnrCaption->setObjectName("lblSnrCaption");

        hboxLayout1->addWidget(lblSnrCaption);

        lblSnrVal = new QLabel(linkBlock);
        lblSnrVal->setObjectName("lblSnrVal");

        hboxLayout1->addWidget(lblSnrVal);


        linkBlockLayout->addLayout(hboxLayout1);


        topRibbonLayout->addWidget(linkBlock);

        sepE = new QFrame(topRibbon);
        sepE->setObjectName("sepE");
        sepE->setFrameShape(QFrame::VLine);
        sepE->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepE);

        timerBlock = new QWidget(topRibbon);
        timerBlock->setObjectName("timerBlock");
        timerBlockLayout = new QVBoxLayout(timerBlock);
        timerBlockLayout->setSpacing(1);
        timerBlockLayout->setObjectName("timerBlockLayout");
        timerBlockLayout->setContentsMargins(10, 6, 10, 4);
        lblTimerCaption = new QLabel(timerBlock);
        lblTimerCaption->setObjectName("lblTimerCaption");

        timerBlockLayout->addWidget(lblTimerCaption);

        lblMissionTimer = new QLabel(timerBlock);
        lblMissionTimer->setObjectName("lblMissionTimer");

        timerBlockLayout->addWidget(lblMissionTimer);


        topRibbonLayout->addWidget(timerBlock);

        sepF = new QFrame(topRibbon);
        sepF->setObjectName("sepF");
        sepF->setFrameShape(QFrame::VLine);
        sepF->setFrameShadow(QFrame::Plain);

        topRibbonLayout->addWidget(sepF);

        recBlock = new QWidget(topRibbon);
        recBlock->setObjectName("recBlock");
        recBlockLayout = new QVBoxLayout(recBlock);
        recBlockLayout->setSpacing(1);
        recBlockLayout->setObjectName("recBlockLayout");
        recBlockLayout->setContentsMargins(10, 6, 10, 4);
        lblRecCaption = new QLabel(recBlock);
        lblRecCaption->setObjectName("lblRecCaption");

        recBlockLayout->addWidget(lblRecCaption);

        lblRecStatus = new QLabel(recBlock);
        lblRecStatus->setObjectName("lblRecStatus");

        recBlockLayout->addWidget(lblRecStatus);


        topRibbonLayout->addWidget(recBlock);

        ribbonSpacer = new QSpacerItem(5, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topRibbonLayout->addItem(ribbonSpacer);


        rootLayout->addWidget(topRibbon);

        mainContentLayout = new QHBoxLayout();
        mainContentLayout->setSpacing(4);
        mainContentLayout->setObjectName("mainContentLayout");
        mainContentLayout->setContentsMargins(4, 4, 4, 4);
        leftContainer = new QWidget(centralwidget);
        leftContainer->setObjectName("leftContainer");
        leftOuterLayout = new QVBoxLayout(leftContainer);
        leftOuterLayout->setSpacing(4);
        leftOuterLayout->setContentsMargins(0, 0, 0, 0);
        leftOuterLayout->setObjectName("leftOuterLayout");
        leftOuterLayout->setContentsMargins(0, 0, 0, 0);
        leftGroupBox = new QGroupBox(leftContainer);
        leftGroupBox->setObjectName("leftGroupBox");
        leftPanelLayout = new QVBoxLayout(leftGroupBox);
        leftPanelLayout->setSpacing(4);
        leftPanelLayout->setObjectName("leftPanelLayout");

        leftOuterLayout->addWidget(leftGroupBox);

        stateGroupBox = new QGroupBox(leftContainer);
        stateGroupBox->setObjectName("stateGroupBox");
        stateGroupBox->setMaximumHeight(90);
        statePanelLayout = new QVBoxLayout(stateGroupBox);
        statePanelLayout->setSpacing(4);
        statePanelLayout->setObjectName("statePanelLayout");

        leftOuterLayout->addWidget(stateGroupBox);

        loggingGroupBox = new QGroupBox(leftContainer);
        loggingGroupBox->setObjectName("loggingGroupBox");
        loggingPanelLayout = new QVBoxLayout(loggingGroupBox);
        loggingPanelLayout->setSpacing(3);
        loggingPanelLayout->setObjectName("loggingPanelLayout");

        leftOuterLayout->addWidget(loggingGroupBox);


        mainContentLayout->addWidget(leftContainer);

        centerContainer = new QWidget(centralwidget);
        centerContainer->setObjectName("centerContainer");
        centerLayout = new QVBoxLayout(centerContainer);
        centerLayout->setSpacing(4);
        centerLayout->setContentsMargins(0, 0, 0, 0);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setContentsMargins(0, 0, 0, 0);
        chartsGroupBox = new QGroupBox(centerContainer);
        chartsGroupBox->setObjectName("chartsGroupBox");
        chartsGridLayout = new QGridLayout(chartsGroupBox);
        chartsGridLayout->setSpacing(4);
        chartsGridLayout->setContentsMargins(6, 6, 6, 6);
        chartsGridLayout->setObjectName("chartsGridLayout");
        altVoltTimeChartView = new QChartView(chartsGroupBox);
        altVoltTimeChartView->setObjectName("altVoltTimeChartView");

        chartsGridLayout->addWidget(altVoltTimeChartView, 0, 0, 1, 1);

        altTempPresChartView = new QChartView(chartsGroupBox);
        altTempPresChartView->setObjectName("altTempPresChartView");

        chartsGridLayout->addWidget(altTempPresChartView, 0, 1, 1, 1);

        altAccelChartView = new QChartView(chartsGroupBox);
        altAccelChartView->setObjectName("altAccelChartView");

        chartsGridLayout->addWidget(altAccelChartView, 1, 0, 1, 1);

        altGyroChartView = new QChartView(chartsGroupBox);
        altGyroChartView->setObjectName("altGyroChartView");

        chartsGridLayout->addWidget(altGyroChartView, 1, 1, 1, 1);


        centerLayout->addWidget(chartsGroupBox);

        commandGroupBox = new QGroupBox(centerContainer);
        commandGroupBox->setObjectName("commandGroupBox");
        commandPanelLayout = new QGridLayout(commandGroupBox);
        commandPanelLayout->setSpacing(4);
        commandPanelLayout->setContentsMargins(6, 6, 6, 6);
        commandPanelLayout->setObjectName("commandPanelLayout");

        centerLayout->addWidget(commandGroupBox);

        centerLayout->setStretch(0, 5);
        centerLayout->setStretch(1, 1);

        mainContentLayout->addWidget(centerContainer);

        rightContainer = new QWidget(centralwidget);
        rightContainer->setObjectName("rightContainer");
        rightOuterLayout = new QVBoxLayout(rightContainer);
        rightOuterLayout->setSpacing(4);
        rightOuterLayout->setContentsMargins(0, 0, 0, 0);
        rightOuterLayout->setObjectName("rightOuterLayout");
        rightOuterLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(rightContainer);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(3);
        gridLayout->setContentsMargins(6, 6, 6, 6);
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


        rightOuterLayout->addWidget(groupBox);

        systemStatusBox = new QGroupBox(rightContainer);
        systemStatusBox->setObjectName("systemStatusBox");
        systemStatusLayout = new QVBoxLayout(systemStatusBox);
        systemStatusLayout->setSpacing(4);
        systemStatusLayout->setContentsMargins(8, 8, 8, 8);
        systemStatusLayout->setObjectName("systemStatusLayout");

        rightOuterLayout->addWidget(systemStatusBox);

        alertsBox = new QGroupBox(rightContainer);
        alertsBox->setObjectName("alertsBox");
        alertsLayout = new QVBoxLayout(alertsBox);
        alertsLayout->setSpacing(3);
        alertsLayout->setContentsMargins(8, 8, 8, 8);
        alertsLayout->setObjectName("alertsLayout");

        rightOuterLayout->addWidget(alertsBox);

        recoveryGroupBox = new QGroupBox(rightContainer);
        recoveryGroupBox->setObjectName("recoveryGroupBox");
        recoveryPanelLayout = new QVBoxLayout(recoveryGroupBox);
        recoveryPanelLayout->setSpacing(3);
        recoveryPanelLayout->setObjectName("recoveryPanelLayout");

        rightOuterLayout->addWidget(recoveryGroupBox);

        rightOuterLayout->setStretch(0, 3);
        rightOuterLayout->setStretch(1, 2);
        rightOuterLayout->setStretch(2, 1);

        mainContentLayout->addWidget(rightContainer);

        mainContentLayout->setStretch(0, 25);
        mainContentLayout->setStretch(1, 45);
        mainContentLayout->setStretch(2, 30);

        rootLayout->addLayout(mainContentLayout);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "NAKSHATRA GCS \342\200\224 CanSat Ground Control Station", nullptr));
        lblAppTitle->setText(QCoreApplication::translate("MainWindow", "NAKSHATRA GCS", nullptr));
        lblAppSubtitle->setText(QCoreApplication::translate("MainWindow", "CanSat Ground Control Station", nullptr));
        lblTeamCaption->setText(QCoreApplication::translate("MainWindow", "TEAM", nullptr));
        lblTeamIdBadge->setText(QCoreApplication::translate("MainWindow", "2026-INSPACe-CANSAT-021", nullptr));
        lblPortCaption->setText(QCoreApplication::translate("MainWindow", "PORT", nullptr));
        lblBaudCaption->setText(QCoreApplication::translate("MainWindow", "BAUD RATE", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "CONNECT", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "DISCONNECT", nullptr));
        lblConnCaption->setText(QCoreApplication::translate("MainWindow", "CONNECTION", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\342\227\217 DISCONNECTED", nullptr));
        lblLinkCaption->setText(QCoreApplication::translate("MainWindow", "LINK QUALITY", nullptr));
        lblLinkQualText->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblRssiCaption->setText(QCoreApplication::translate("MainWindow", "RSSI:", nullptr));
        lblRssiVal->setText(QCoreApplication::translate("MainWindow", "-- dBm", nullptr));
        lblSnrCaption->setText(QCoreApplication::translate("MainWindow", "SNR:", nullptr));
        lblSnrVal->setText(QCoreApplication::translate("MainWindow", "-- dB", nullptr));
        lblTimerCaption->setText(QCoreApplication::translate("MainWindow", "TIMER", nullptr));
        lblMissionTimer->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        lblRecCaption->setText(QCoreApplication::translate("MainWindow", "RECORDING", nullptr));
        lblRecStatus->setText(QCoreApplication::translate("MainWindow", "\342\227\217 IDLE", nullptr));
        leftGroupBox->setTitle(QCoreApplication::translate("MainWindow", "3D Orientation", nullptr));
        stateGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Mission Progress", nullptr));
        loggingGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Data Logging", nullptr));
        chartsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Live Telemetry Graphs", nullptr));
        commandGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Command Center", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Telemetry Overview", nullptr));
        label_team->setText(QCoreApplication::translate("MainWindow", "Team ID:", nullptr));
        lblTeamId->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Packet:", nullptr));
        lblPacketId->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Time:", nullptr));
        lblTimestamp->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        systemStatusBox->setTitle(QCoreApplication::translate("MainWindow", "System Status", nullptr));
        alertsBox->setTitle(QCoreApplication::translate("MainWindow", "Recent Alerts", nullptr));
        recoveryGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Recovery Panel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
