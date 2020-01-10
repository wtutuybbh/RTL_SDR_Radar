#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../MyLib/RTL_SDR_RadarLib/GraphicsWidget/GraphicsWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->teTerminal->setTextColor(QColor(0xAA,0xCF,0xD1));
    setReciverDeviceState(false);
    setDBState(false);
    setGPSState(false);

    QObject::connect(&_timer, &QTimer::timeout,
                     this, &MainWindow::updateTerminal);
    _timer.start(timeout);
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::addGraphicsWidget(GraphicsWidget* widget)
{
    ui->vrlRadar->addWidget(widget);
}

void MainWindow::addTableWidget(QWidget *widget)
{
    ui->vltTableSrc->addWidget(widget);
}

void MainWindow::setReciverDeviceState(bool state)
{
     ui->lRtlSdrStatus->setStyleSheet(takeColorStrByState(state));
}

void MainWindow::setDBState(bool state)
{
    ui->lDBStatus->setStyleSheet(takeColorStrByState(state));
}

void MainWindow::setGPSState(bool state)
{
    ui->lGPSStatus->setStyleSheet(takeColorStrByState(state));
}


QString MainWindow::takeColorStrByState(bool state)
{
    QString color{"background-color: grey"};
    if(state)
        color = QString("background-color: green");

    return color;
}


void MainWindow::updateTerminal()
{
    if(!_logger.isNull() && !_logger->isEmpty())
    {
        ui->teTerminal->append(_logger->pop());
        ui->pgDbgMesgFIFO->setValue(_logger->countMsg());
    }

    logBalancing();

    if(ui->teTerminal->document()->lineCount() > COUNT_TEXT_LINE)
        ui->teTerminal->clear();
}

void MainWindow::logBalancing()
{
    if(_logger.isNull())
        return;

    static uint8_t upBalanceCounter = 0;
    static uint8_t downBalanceCounter = 0;
    int interval = _timer.interval();

    if(_logger->countMsg() < COUNT_MSG_MIN)
    {
        if(downBalanceCounter > COUNTER_BALANCE)
        {
            interval =_timer.interval() * 2;
            if((interval) > MAX_INTERVAL)
                interval = MAX_INTERVAL;
            downBalanceCounter = 0;
        }
        else
            ++downBalanceCounter;
    }

    if((_logger->countMsg() > COUNT_MSG_MAX))
    {
        if(upBalanceCounter > COUNTER_BALANCE)
        {
            interval =_timer.interval() / 2;
            if((interval) < MIN_INTERVAL)
                interval = MIN_INTERVAL;
            upBalanceCounter = 0;
        }
        else
            ++upBalanceCounter;
    }

    if(interval != _timer.interval())
        _timer.setInterval(interval);
}
