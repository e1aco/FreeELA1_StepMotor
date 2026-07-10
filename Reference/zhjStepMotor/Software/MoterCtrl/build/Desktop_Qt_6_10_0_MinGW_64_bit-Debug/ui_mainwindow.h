/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox_Serial;
    QLabel *label_Port;
    QLabel *label_Baud;
    QComboBox *combo_Baud;
    QPushButton *btn_OpenSerial;
    QLabel *label_SerialStatus;
    QLineEdit *edit_Port;
    QGroupBox *groupBox_Mode;
    QRadioButton *radio_ModeCurrent;
    QRadioButton *radio_ModeSpeed;
    QRadioButton *radio_ModePosition;
    QPushButton *btn_Stop;
    QDoubleSpinBox *doubleSpinBox_Current;
    QDoubleSpinBox *doubleSpinBox_Speed;
    QDoubleSpinBox *doubleSpinBox_Position;
    QPushButton *btn_Start;
    QFrame *frame_RealData;
    QLabel *label_RealPos;
    QLabel *label_RealVel;
    QLabel *label_RealCur;
    QGroupBox *groupBox_Debug;
    QPushButton *btn_ClearStall;
    QPushButton *btn_ZeroPos;
    QLineEdit *edit_CustomCmd;
    QPushButton *btn_SendCmd;
    QFrame *frame_Current;
    QFrame *frame_Speed;
    QFrame *frame_Position;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox_Serial = new QGroupBox(centralwidget);
        groupBox_Serial->setObjectName("groupBox_Serial");
        groupBox_Serial->setGeometry(QRect(10, 10, 440, 110));
        label_Port = new QLabel(groupBox_Serial);
        label_Port->setObjectName("label_Port");
        label_Port->setGeometry(QRect(15, 25, 60, 24));
        label_Baud = new QLabel(groupBox_Serial);
        label_Baud->setObjectName("label_Baud");
        label_Baud->setGeometry(QRect(210, 25, 60, 24));
        combo_Baud = new QComboBox(groupBox_Serial);
        combo_Baud->addItem(QString());
        combo_Baud->addItem(QString());
        combo_Baud->addItem(QString());
        combo_Baud->setObjectName("combo_Baud");
        combo_Baud->setGeometry(QRect(270, 25, 100, 24));
        btn_OpenSerial = new QPushButton(groupBox_Serial);
        btn_OpenSerial->setObjectName("btn_OpenSerial");
        btn_OpenSerial->setGeometry(QRect(15, 60, 100, 30));
        label_SerialStatus = new QLabel(groupBox_Serial);
        label_SerialStatus->setObjectName("label_SerialStatus");
        label_SerialStatus->setGeometry(QRect(120, 62, 80, 24));
        edit_Port = new QLineEdit(groupBox_Serial);
        edit_Port->setObjectName("edit_Port");
        edit_Port->setGeometry(QRect(75, 25, 120, 24));
        groupBox_Mode = new QGroupBox(centralwidget);
        groupBox_Mode->setObjectName("groupBox_Mode");
        groupBox_Mode->setGeometry(QRect(10, 130, 440, 271));
        radio_ModeCurrent = new QRadioButton(groupBox_Mode);
        radio_ModeCurrent->setObjectName("radio_ModeCurrent");
        radio_ModeCurrent->setGeometry(QRect(20, 25, 100, 24));
        radio_ModeSpeed = new QRadioButton(groupBox_Mode);
        radio_ModeSpeed->setObjectName("radio_ModeSpeed");
        radio_ModeSpeed->setGeometry(QRect(20, 65, 100, 24));
        radio_ModePosition = new QRadioButton(groupBox_Mode);
        radio_ModePosition->setObjectName("radio_ModePosition");
        radio_ModePosition->setGeometry(QRect(20, 105, 100, 24));
        radio_ModePosition->setChecked(true);
        btn_Stop = new QPushButton(groupBox_Mode);
        btn_Stop->setObjectName("btn_Stop");
        btn_Stop->setGeometry(QRect(5, 220, 431, 40));
        doubleSpinBox_Current = new QDoubleSpinBox(groupBox_Mode);
        doubleSpinBox_Current->setObjectName("doubleSpinBox_Current");
        doubleSpinBox_Current->setGeometry(QRect(130, 25, 81, 25));
        doubleSpinBox_Current->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        doubleSpinBox_Current->setMinimum(-99.000000000000000);
        doubleSpinBox_Speed = new QDoubleSpinBox(groupBox_Mode);
        doubleSpinBox_Speed->setObjectName("doubleSpinBox_Speed");
        doubleSpinBox_Speed->setGeometry(QRect(130, 65, 81, 25));
        doubleSpinBox_Speed->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        doubleSpinBox_Speed->setMinimum(-99.000000000000000);
        doubleSpinBox_Position = new QDoubleSpinBox(groupBox_Mode);
        doubleSpinBox_Position->setObjectName("doubleSpinBox_Position");
        doubleSpinBox_Position->setGeometry(QRect(130, 105, 81, 25));
        doubleSpinBox_Position->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        doubleSpinBox_Position->setMinimum(-99.000000000000000);
        btn_Start = new QPushButton(groupBox_Mode);
        btn_Start->setObjectName("btn_Start");
        btn_Start->setGeometry(QRect(5, 170, 431, 40));
        btn_Start->setStyleSheet(QString::fromUtf8("QPushButton{background-color:#27ae60;color:white;font-size:16px;font-weight:bold;border-radius:6px;}QPushButton:hover{background-color:#2ecc71;}"));
        frame_RealData = new QFrame(centralwidget);
        frame_RealData->setObjectName("frame_RealData");
        frame_RealData->setGeometry(QRect(10, 410, 440, 160));
        frame_RealData->setFrameShape(QFrame::Shape::StyledPanel);
        label_RealPos = new QLabel(frame_RealData);
        label_RealPos->setObjectName("label_RealPos");
        label_RealPos->setGeometry(QRect(20, 15, 400, 40));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label_RealPos->setFont(font);
        label_RealPos->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_RealVel = new QLabel(frame_RealData);
        label_RealVel->setObjectName("label_RealVel");
        label_RealVel->setGeometry(QRect(20, 60, 400, 40));
        label_RealVel->setFont(font);
        label_RealVel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_RealCur = new QLabel(frame_RealData);
        label_RealCur->setObjectName("label_RealCur");
        label_RealCur->setGeometry(QRect(20, 105, 400, 40));
        label_RealCur->setFont(font);
        label_RealCur->setAlignment(Qt::AlignmentFlag::AlignCenter);
        groupBox_Debug = new QGroupBox(centralwidget);
        groupBox_Debug->setObjectName("groupBox_Debug");
        groupBox_Debug->setGeometry(QRect(10, 580, 440, 100));
        btn_ClearStall = new QPushButton(groupBox_Debug);
        btn_ClearStall->setObjectName("btn_ClearStall");
        btn_ClearStall->setGeometry(QRect(15, 25, 80, 30));
        btn_ZeroPos = new QPushButton(groupBox_Debug);
        btn_ZeroPos->setObjectName("btn_ZeroPos");
        btn_ZeroPos->setGeometry(QRect(110, 25, 80, 30));
        edit_CustomCmd = new QLineEdit(groupBox_Debug);
        edit_CustomCmd->setObjectName("edit_CustomCmd");
        edit_CustomCmd->setGeometry(QRect(15, 65, 300, 26));
        btn_SendCmd = new QPushButton(groupBox_Debug);
        btn_SendCmd->setObjectName("btn_SendCmd");
        btn_SendCmd->setGeometry(QRect(325, 65, 80, 26));
        frame_Current = new QFrame(centralwidget);
        frame_Current->setObjectName("frame_Current");
        frame_Current->setGeometry(QRect(470, 10, 800, 200));
        frame_Current->setFrameShape(QFrame::Shape::StyledPanel);
        frame_Current->setFrameShadow(QFrame::Shadow::Raised);
        frame_Speed = new QFrame(centralwidget);
        frame_Speed->setObjectName("frame_Speed");
        frame_Speed->setGeometry(QRect(470, 245, 800, 200));
        frame_Speed->setFrameShape(QFrame::Shape::StyledPanel);
        frame_Speed->setFrameShadow(QFrame::Shadow::Raised);
        frame_Position = new QFrame(centralwidget);
        frame_Position->setObjectName("frame_Position");
        frame_Position->setGeometry(QRect(470, 480, 800, 200));
        frame_Position->setFrameShape(QFrame::Shape::StyledPanel);
        frame_Position->setFrameShadow(QFrame::Shadow::Raised);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 25));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\255\245\350\277\233\347\224\265\346\234\272\350\260\203\350\257\225\344\270\212\344\275\215\346\234\272", nullptr));
        groupBox_Serial->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\350\256\276\347\275\256", nullptr));
        label_Port->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267\357\274\232", nullptr));
        label_Baud->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        combo_Baud->setItemText(0, QCoreApplication::translate("MainWindow", "115200", nullptr));
        combo_Baud->setItemText(1, QCoreApplication::translate("MainWindow", "57600", nullptr));
        combo_Baud->setItemText(2, QCoreApplication::translate("MainWindow", "9600", nullptr));

        btn_OpenSerial->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        label_SerialStatus->setStyleSheet(QCoreApplication::translate("MainWindow", "color:red;", nullptr));
        label_SerialStatus->setText(QCoreApplication::translate("MainWindow", "\342\227\217 \346\234\252\350\277\236\346\216\245", nullptr));
        edit_Port->setText(QCoreApplication::translate("MainWindow", "COM7", nullptr));
        edit_Port->setPlaceholderText(QCoreApplication::translate("MainWindow", "\344\276\213\345\246\202 ", nullptr));
        groupBox_Mode->setTitle(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\346\250\241\345\274\217", nullptr));
        radio_ModeCurrent->setText(QCoreApplication::translate("MainWindow", "\347\224\265\346\265\201\346\250\241\345\274\217", nullptr));
        radio_ModeSpeed->setText(QCoreApplication::translate("MainWindow", "\351\200\237\345\272\246\346\250\241\345\274\217", nullptr));
        radio_ModePosition->setText(QCoreApplication::translate("MainWindow", "\344\275\215\347\275\256\346\250\241\345\274\217", nullptr));
        btn_Stop->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton{background-color:#e74c3c;color:white;font-size:16px;font-weight:bold;border-radius:6px;}QPushButton:hover{background-color:#c0392b;}", nullptr));
        btn_Stop->setText(QCoreApplication::translate("MainWindow", "\346\200\245\345\201\234", nullptr));
        btn_Start->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213", nullptr));
        label_RealPos->setText(QCoreApplication::translate("MainWindow", "\344\275\215\347\275\256\357\274\2320.00 R", nullptr));
        label_RealVel->setText(QCoreApplication::translate("MainWindow", "\351\200\237\345\272\246\357\274\2320.00 R/s", nullptr));
        label_RealCur->setText(QCoreApplication::translate("MainWindow", "\347\224\265\346\265\201\357\274\2320.00 A", nullptr));
        groupBox_Debug->setTitle(QCoreApplication::translate("MainWindow", "\350\260\203\350\257\225\345\221\275\344\273\244", nullptr));
        btn_ClearStall->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\345\240\265\350\275\254", nullptr));
        btn_ZeroPos->setText(QCoreApplication::translate("MainWindow", "\344\275\215\347\275\256\345\275\222\351\233\266", nullptr));
        edit_CustomCmd->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\345\221\275\344\273\244...", nullptr));
        btn_SendCmd->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
