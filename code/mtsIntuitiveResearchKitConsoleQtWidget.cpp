/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: mtsIntuitiveResearchKitConsoleQtWidget.cpp 4138 2013-04-27 21:48:29Z adeguet1 $

  Author(s):  Anton Deguet
  Created on: 2013-05-17

  (C) Copyright 2013 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/


// system include
#include <iostream>

// Qt include
#include <QString>
#include <QtGui>

// cisst
#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <cisstMultiTask/mtsInterfaceProvided.h>
#include <sawIntuitiveResearchKit/mtsIntuitiveResearchKitConsoleQtWidget.h>

CMN_IMPLEMENT_SERVICES(mtsIntuitiveResearchKitConsoleQtWidget);

mtsIntuitiveResearchKitConsoleQtWidget::mtsIntuitiveResearchKitConsoleQtWidget(const std::string & componentName):
    mtsComponent(componentName)
{
    // Setup CISST Interface
#if 0
    mtsInterfaceRequired * requiredInterface = AddInterfaceRequired("TeleOperation");
    if (requiredInterface) {
        requiredInterface->AddFunction("Enable", TeleOperation.Enable);
    }
#endif


    mtsInterfaceRequired* reqPSM = AddInterfaceRequired("PSM");
    if (reqPSM) {
        reqPSM->AddFunction("SetRobotControlState", PSM.SetRobotControlState);
//        reqPSM->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::StateMsgEventHandler, this, "State");

        reqPSM->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::StateMsgEventHandler,
                                     this, "RobotStatusMsg");
        reqPSM->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::ErrorMsgEventHandler,
                                     this, "RobotErrorMsg");
    }

    mtsInterfaceRequired* reqMTM = AddInterfaceRequired("MTM");
    if (reqMTM) {
        reqMTM->AddFunction("SetRobotControlState", MTM.SetRobotControlState);
        reqMTM->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::StateMsgEventHandler,
                                     this, "RobotStatusMsg");
        reqMTM->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::ErrorMsgEventHandler,
                                     this, "RobotErrorMsg");
    }


    setupUi();
}

void mtsIntuitiveResearchKitConsoleQtWidget::Configure(const std::string & filename)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "Configure: " << filename << std::endl;
}

void mtsIntuitiveResearchKitConsoleQtWidget::Startup(void)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "mtsIntuitiveResearchKitConsoleQtWidget::Startup" << std::endl;
    show();
}

void mtsIntuitiveResearchKitConsoleQtWidget::Cleanup(void)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "mtsIntuitiveResearchKitConsoleQtWidget::Cleanup" << std::endl;
}

void mtsIntuitiveResearchKitConsoleQtWidget::closeEvent(QCloseEvent * event)
{
    event->accept();
}

//void mtsIntuitiveResearchKitConsoleQtWidget::slot_SetStateButton(int id)
//{
//    std::cerr << "----- " << id << std::endl;
//    std::string newState = "state";
//    MTM.SetState(newState);
//}

void mtsIntuitiveResearchKitConsoleQtWidget::slot_SetStateButton(QAbstractButton *radioButton)
{
    std::cout << "---- Radio Button " << radioButton->text().toStdString() << std::endl;
    std::string state = radioButton->text().toStdString();
    PSM.SetRobotControlState(mtsStdString(state));
    MTM.SetRobotControlState(mtsStdString(state));
}




void mtsIntuitiveResearchKitConsoleQtWidget::setupUi(void)
{
    QGridLayout * frameLayout = new QGridLayout;

    CurrentStateLabel = new QLabel("undefined");
    frameLayout->addWidget(CurrentStateLabel, 0, 0);

    QGroupBox * groupBox = new QGroupBox("Desired state");
    QRadioButton * homeState = new QRadioButton("Home");
    QRadioButton * teleOpMode = new QRadioButton("Teleop");
    homeState->setChecked(true);
    QButtonGroup * group = new QButtonGroup;
	group->addButton(homeState);
	group->addButton(teleOpMode);
	group->setExclusive(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(homeState);
    vbox->addWidget(teleOpMode);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    frameLayout->addWidget(groupBox, 1, 0);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(frameLayout);
    setLayout(mainLayout);

    setWindowTitle("Intuitive Research Kit");
    resize(sizeHint());

//    connect(group, SIGNAL(buttonClicked(int)), this, SLOT(slot_SetStateButton(int)));

    connect(group, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(slot_SetStateButton(QAbstractButton*)));
}

void mtsIntuitiveResearchKitConsoleQtWidget::StateMsgEventHandler(const std::string & newState)
{
    CurrentStateLabel->setText(newState.c_str());
}

void mtsIntuitiveResearchKitConsoleQtWidget::ErrorMsgEventHandler(const std::string &newMsg)
{
    // ZC: temp for testing
    std::cerr << newMsg << std::endl;
}


