#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../MyLib/RTL_SDR_RadarLib/GraphicsWidget/GraphicsWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->teTerminal->setTextColor(QColor(0xAA,0xCF,0xD1));
    ui->teTerminal->insertPlainText("test....");
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::addGraphicsWidget(GraphicsWidget* widget)
{
    ui->vrlRadar->addWidget(widget);
}
