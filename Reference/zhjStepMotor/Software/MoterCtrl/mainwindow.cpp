#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_hCom(INVALID_HANDLE_VALUE)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // 初始化波形
    setupPlots();
    m_startTime = QDateTime::currentDateTime();

    m_plotTimer = new QTimer(this);
    connect(m_plotTimer, &QTimer::timeout, this, &MainWindow::updatePlots);
    m_plotTimer->start(200);

    // 串口轮询定时器
    m_serialTimer = new QTimer(this);
    connect(m_serialTimer, &QTimer::timeout, this, &MainWindow::onSerialTimer);
}

MainWindow::~MainWindow()
{
    closeSerial();
    delete ui;
}

// ==================== 波形 ====================

void MainWindow::clearFrameLayout(QFrame *frame)
{
    if (!frame) return;
    QLayout *old = frame->layout();
    if (old) {
        QLayoutItem *item;
        while ((item = old->takeAt(0)) != nullptr) {
            if (item->widget()) item->widget()->setParent(nullptr);
            delete item;
        }
        delete old;
    }
}

void MainWindow::setupPlots()
{
    // 电流波形
    clearFrameLayout(ui->frame_Current);
    m_plotCurrent = new QCustomPlot();
    m_plotCurrent->xAxis->setLabel("时间 (s)");
    m_plotCurrent->yAxis->setLabel("电流 (mA)");
    m_plotCurrent->xAxis->setRange(0, 60);
    m_plotCurrent->yAxis->setRange(-3000, 3000);
    m_plotCurrent->addGraph()->setPen(QPen(QColor("#e74c3c"), 2));
    m_plotCurrent->graph(0)->setName("电流");
    QVBoxLayout *layCur = new QVBoxLayout(ui->frame_Current);
    layCur->setContentsMargins(5, 5, 5, 5);
    layCur->addWidget(m_plotCurrent);

    // 速度波形
    clearFrameLayout(ui->frame_Speed);
    m_plotSpeed = new QCustomPlot();
    m_plotSpeed->xAxis->setLabel("时间 (s)");
    m_plotSpeed->yAxis->setLabel("速度 (RPM)");
    m_plotSpeed->xAxis->setRange(0, 60);
    m_plotSpeed->yAxis->setRange(-500, 500);
    m_plotSpeed->addGraph()->setPen(QPen(QColor("#3498db"), 2));
    m_plotSpeed->graph(0)->setName("速度");
    QVBoxLayout *laySpd = new QVBoxLayout(ui->frame_Speed);
    laySpd->setContentsMargins(5, 5, 5, 5);
    laySpd->addWidget(m_plotSpeed);

    // 位置波形
    clearFrameLayout(ui->frame_Position);
    m_plotPosition = new QCustomPlot();
    m_plotPosition->xAxis->setLabel("时间 (s)");
    m_plotPosition->yAxis->setLabel("位置 (R)");
    m_plotPosition->xAxis->setRange(0, 60);
    m_plotPosition->yAxis->setRange(-1000, 1000);
    m_plotPosition->addGraph()->setPen(QPen(QColor("#27ae60"), 2));
    m_plotPosition->graph(0)->setName("位置");
    QVBoxLayout *layPos = new QVBoxLayout(ui->frame_Position);
    layPos->setContentsMargins(5, 5, 5, 5);
    layPos->addWidget(m_plotPosition);
}

void MainWindow::addDataPoint(double current, double speed, double position)
{
    double t = m_startTime.msecsTo(QDateTime::currentDateTime()) / 1000.0;

    m_timeData.append(t);
    m_currentData.append(current);
    m_speedData.append(speed);
    m_positionData.append(position);

    while (m_timeData.size() > MaxDataPoints) {
        m_timeData.removeFirst();
        m_currentData.removeFirst();
        m_speedData.removeFirst();
        m_positionData.removeFirst();
    }
}

void MainWindow::updatePlots()
{
    if (m_timeData.isEmpty()) return;

    m_plotCurrent->graph(0)->setData(m_timeData, m_currentData);
    m_plotCurrent->xAxis->setRange(m_timeData.first(), m_timeData.last());
    m_plotCurrent->replot();

    m_plotSpeed->graph(0)->setData(m_timeData, m_speedData);
    m_plotSpeed->xAxis->setRange(m_timeData.first(), m_timeData.last());
    m_plotSpeed->replot();

    m_plotPosition->graph(0)->setData(m_timeData, m_positionData);
    m_plotPosition->xAxis->setRange(m_timeData.first(), m_timeData.last());
    m_plotPosition->replot();
}

void MainWindow::updateMotorState(const QString &mode, const QString &state,
                                  double pos, double vel, double cur)
{
    ui->label_RealPos->setText(QString("位置：%1 R").arg(pos, 0, 'f', 2));
    ui->label_RealVel->setText(QString("速度：%1 R/s").arg(vel, 0, 'f', 2));
    ui->label_RealCur->setText(QString("电流：%1 A").arg(cur, 0, 'f', 2));
}

// ==================== 串口 (Windows API) ====================

void MainWindow::openSerial(const QString &portName, int baudRate)
{
    QString fullName = "\\\\.\\" + portName;
    m_hCom = CreateFileA(fullName.toLatin1().data(),
                         GENERIC_READ | GENERIC_WRITE,
                         0, NULL, OPEN_EXISTING, 0, NULL);
    if (m_hCom == INVALID_HANDLE_VALUE) {
        QMessageBox::warning(this, "警告", "无法打开串口: " + portName);
        return;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hCom, &dcb);
    dcb.BaudRate = baudRate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(m_hCom, &dcb);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    SetCommTimeouts(m_hCom, &timeouts);

    ui->btn_OpenSerial->setText("关闭串口");
    ui->label_SerialStatus->setText("● 已连接");
    ui->label_SerialStatus->setStyleSheet("color:green;");

    m_serialTimer->start(100);
}

void MainWindow::closeSerial()
{
    m_serialTimer->stop();
    if (m_hCom != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hCom);
        m_hCom = INVALID_HANDLE_VALUE;
    }
    ui->btn_OpenSerial->setText("打开串口");
    ui->label_SerialStatus->setText("● 未连接");
    ui->label_SerialStatus->setStyleSheet("color:red;");
}

void MainWindow::sendCommand(const QString &cmd)
{
    if (m_hCom == INVALID_HANDLE_VALUE) return;
    QByteArray data = cmd.toUtf8();
    DWORD written;
    WriteFile(m_hCom, data.data(), data.size(), &written, NULL);
}

void MainWindow::readSerialData()
{
    if (m_hCom == INVALID_HANDLE_VALUE) return;
    char buf[1];
    DWORD n;
    while (ReadFile(m_hCom, buf, 1, &n, NULL) && n > 0) {
        if (buf[0] == '\n') {
            QString str = QString::fromUtf8(m_recvBuffer);
            m_recvBuffer.clear();
            if (str.startsWith("T:")) {
                QStringList parts = str.mid(2).split(',');
                if (parts.size() >= 3) {
                    double pos = parts[0].toDouble();
                    double vel = parts[1].toDouble();
                    double cur = parts[2].toDouble();

                    // 大字
                    ui->label_RealPos->setText(QString("位置：%1 R").arg(pos, 0, 'f', 2));
                    ui->label_RealVel->setText(QString("速度：%1 R/s").arg(vel, 0, 'f', 2));
                    ui->label_RealCur->setText(QString("电流：%1 A").arg(cur, 0, 'f', 2));

                    // 存数据
                    double t = m_startTime.msecsTo(QDateTime::currentDateTime()) / 1000.0;
                    m_timeData.append(t);
                    m_currentData.append(cur * 1000);
                    m_speedData.append(vel);
                    m_positionData.append(pos);
                    if (m_timeData.size() > 100) {
                        m_timeData.removeFirst();
                        m_currentData.removeFirst();
                        m_speedData.removeFirst();
                        m_positionData.removeFirst();
                    }
                }
            }
        } else {
            m_recvBuffer.append(buf[0]);
        }
    }
}

void MainWindow::onSerialTimer()
{
    readSerialData();
}

void MainWindow::on_btn_OpenSerial_clicked()
{
    if (m_hCom != INVALID_HANDLE_VALUE) {
        closeSerial();
        return;
    }
    QString port = ui->edit_Port->text().trimmed();
    if (port.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入串口号");
        return;
    }
    openSerial(port, ui->combo_Baud->currentText().toInt());
}

// ==================== 控制 ====================

void MainWindow::on_btn_Start_clicked()
{
    switch (moterMode) {
    case MODE_CURRENT:
        sendCommand(QString("c %1").arg(ui->doubleSpinBox_Current->value()));
        break;
    case MODE_SPEED:
        sendCommand(QString("v %1").arg(ui->doubleSpinBox_Speed->value()));
        break;
    case MODE_POSITION:
        sendCommand(QString("p %1").arg(ui->doubleSpinBox_Position->value()));
        break;
    }
}

void MainWindow::on_btn_Stop_clicked()
{
    sendCommand("s");
}

void MainWindow::on_radio_ModeCurrent_clicked()
{
    moterMode = MODE_CURRENT;
}

void MainWindow::on_radio_ModeSpeed_clicked()
{
    moterMode = MODE_SPEED;
}

void MainWindow::on_radio_ModePosition_clicked()
{
    moterMode = MODE_POSITION;
}

// ==================== 调试命令 ====================

void MainWindow::on_btn_ClearStall_clicked()
{
    sendCommand("l");
}

void MainWindow::on_btn_ZeroPos_clicked()
{
    sendCommand("z");
}

void MainWindow::on_btn_SendCmd_clicked()
{
    QString cmd = ui->edit_CustomCmd->text();
    if (!cmd.isEmpty()) {
        sendCommand(cmd);
        ui->edit_CustomCmd->clear();
    }
}
