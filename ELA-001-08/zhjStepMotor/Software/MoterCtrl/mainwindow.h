#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QLineEdit>
#include <windows.h>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_OpenSerial_clicked();
    void on_btn_Start_clicked();
    void on_btn_Stop_clicked();
    void on_radio_ModeCurrent_clicked();
    void on_radio_ModeSpeed_clicked();
    void on_radio_ModePosition_clicked();
    // void on_btn_Calibrate_clicked();
    void on_btn_ClearStall_clicked();
    void on_btn_ZeroPos_clicked();
    // void on_btn_SaveConfig_clicked();
    void on_btn_SendCmd_clicked();
    void onSerialTimer();

private:
    Ui::MainWindow *ui;

    // 串口 (Windows API)
    HANDLE m_hCom;
    QTimer *m_serialTimer;
    QByteArray m_recvBuffer;
    void openSerial(const QString &portName, int baudRate);
    void closeSerial();
    void sendCommand(const QString &cmd);
    void readSerialData();

    // 波形
    QCustomPlot *m_plotCurrent, *m_plotSpeed, *m_plotPosition;
    QTimer *m_plotTimer;
    QDateTime m_startTime;
    QVector<double> m_timeData, m_currentData, m_speedData, m_positionData;
    static const int MaxDataPoints = 1000;

    typedef enum { MODE_CURRENT, MODE_SPEED, MODE_POSITION } MotorMode_t;
    MotorMode_t moterMode = MODE_POSITION;

    void setupPlots();
    void clearFrameLayout(QFrame *frame);
    void updatePlots();
    void addDataPoint(double current, double speed, double position);
    void updateMotorState(const QString &mode, const QString &state,
                          double pos, double vel, double cur);
};

#endif
