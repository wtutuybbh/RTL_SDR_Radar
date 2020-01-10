#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../MyLib/RTL_SDR_RadarLib/GraphicsWidget/GraphicsWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setReciverDeviceState(false);
    setDBState(false);
    setGPSState(false);
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
