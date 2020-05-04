#include "gui/MainWindow.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "qcustomplot/qcustomplot.h"
#include "../AppCore/Core.h"
#include "RTL_SDR_Reciver/RTL_SDR_Reciver.h"
#include "interface/IDataController.h"
#include "interface/IDSP.h"
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(Core* core, QWidget *parent) :
    QMainWindow(parent),
    _core(core),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    plotSpectrum = new QCustomPlot();
    plotSpectrum->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plotSpectrum->setOpenGl(true);
    QCPGraph *graph = plotSpectrum->addGraph();
    graph->setPen(QPen(Qt::blue));
    graph->rescaleKeyAxis();


    plotSpectrum->yAxis->setRange(0,200);
    plotSpectrum->xAxis->setRange(ui->dsbFreq->value() - FS / 2 / 1.e6 ,
                                  ui->dsbFreq->value() + FS / 2 / 1.e6);
    plotSpectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);

    ui->hrlOscilloscope->addWidget(plotSpectrum);

    QObject::connect(&_timer, &QTimer::timeout,
                     this, &MainWindow::slotUpdateWidgets);
    _timer.start(timeout);

    xAxis.resize(N);

    for(int i = 0; i < N; ++i)
        xAxis[i] =((double)i/N * FS - FS/2)/1.e6 + ui->dsbFreq->value();

    accumSpectrDeep = ui->hslAccumSpectrumDeep->value();
    ui->lAccumSpectrumDeepValue->setText(QString::number(accumSpectrDeep));
    accumSpectrStore = QVector<double>(N * ui->hslAccumSpectrumDeep->maximum(),0.0);

    powerPeakA = QVector<double>(N, 0 );
    powerAccumA = QVector<double>(N, 0 );

    peakSpectrCoeffDelta = 1.e-5;
    peakSpectrCoeff = ui->sliderPeakSpectrCoeff->value() * peakSpectrCoeffDelta;
    ui->lPeakSpectrCoeffValue->setText(QString("%1").arg(peakSpectrCoeff, 0, 'f', 5));


    plotWaterfall = new QCustomPlot();
    plotWaterfall->setOpenGl(true);
    // configure axis rect:
    plotWaterfall->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    plotWaterfall->axisRect()->setupFullAxesBox(true);
    //    plotWaterWall->xAxis->setLabel("x");
    // plotWaterWall->yAxis->setLabel("y");



    // set up the QCPColorMap:
    colorMap = new QCPColorMap(plotWaterfall->xAxis, plotWaterfall->yAxis);

    colorMap->data()->setSize(N, WIDTH_WATERWALL); // we want the color map to have nx * ny data points
    colorMap->data()->setRange(QCPRange(ui->dsbFreq->value() - FS / 2 / 1.e6 ,
                                        ui->dsbFreq->value() + FS / 2 / 1.e6),
                               QCPRange(0, WIDTH_WATERWALL)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:


    // add a color scale:
    colorScale = new QCPColorScale(plotWaterfall);
   // plotWaterfall->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorScale->setDataRange(QCPRange(0,255));
    colorMap->setColorScale(colorScale); // associate the color map with the color scale

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(plotWaterfall);
    plotWaterfall->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);



    // set the color gradient of the color map to one of the presets:
    colorMap->setGradient(QCPColorGradient::gpJet);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.
    colorMap->rescaleDataRange();
    plotWaterfall->rescaleAxes();
    ui->vltWaterfall->addWidget(plotWaterfall);

}

MainWindow::~MainWindow()
{
    _core = nullptr;

    delete ui;
}

void MainWindow::on_pbFreq_clicked()
{
    if (_core == nullptr)
        return;

    _core->device()->setFreq(uint32_t(ui->dsbFreq->value() * 1.0e6));
}


void MainWindow::slotUpdateWidgets()
{
    if(_core->dsp().isNull())
        return;
    SrcDataAdc data;
    if(_core->dsp()->tryLockDataBuffer())
    {
        if(_core->dsp()->isEmptyDataBuffer())
        {
            _core->dsp()->unlockDataBuffer();
            return;
        }
        data = _core->dsp()->takeDataBlock();

        _core->dsp()->unlockDataBuffer();
    }

    if(ui->rbRealSpectrum->isChecked())
    {

        plotSpectrum->graph()->setData(xAxis, data.fftVector);
        fillWaterfallDN(data.fftVector);
    }

    if(ui->rbSignalEnvelope->isChecked())
    {
        plotSpectrum->graph()->setData(xAxis, data.magnitudeVector);
        fillWaterfallDN(data.magnitudeVector);

    }

    if(ui->rbAccumSpectrum->isChecked())
    {

        countAccum++;

        if (countAccum > accumSpectrDeep)
            countAccum = accumSpectrDeep;


        for(int i = 0 ; i < N; ++i)
        {
            powerPeakA[i] -= accumSpectrStore[currAccum*N + i];

            accumSpectrStore[currAccum*N + i] = data.magnitudeVector[i];

            powerPeakA[i] += data.magnitudeVector[i];

            powerAccumA[i] = powerPeakA[i];

            powerAccumA[i] /= countAccum;

        }

        currAccum +=1;
        currAccum  %= accumSpectrDeep;

        plotSpectrum->graph()->setData(xAxis, powerAccumA);
        fillWaterfallDN(powerAccumA);

    }

    if (ui->rbPeakSpectrum->isChecked())
    {
        for (int i=0; i<N; ++i)
        {
            if (data.magnitudeVector[i] > powerPeakA[i])
                powerPeakA[i] = data.magnitudeVector[i];
            else
                powerPeakA[i] *= peakSpectrCoeff;
        }
        plotSpectrum->graph()->setData(xAxis,powerPeakA);
        fillWaterfallDN(powerPeakA);
    }

    plotSpectrum->replot();


}

void MainWindow::clearPeaks()
{
    if (ui->rbAccumSpectrum->isChecked())
        std::fill_n(powerPeakA.data(), N,0);
    else
        std::fill_n(powerPeakA.data(), N,-1000);

    std::fill_n(accumSpectrStore.data(),N * ui->hslAccumSpectrumDeep->maximum(), 0);

    std::fill_n(powerAccumA.data(),N, 0);

    currAccum = 0;
    countAccum = 0;
}

void MainWindow::fillWaterfallDN(const QVector<double> &powerA)
{
    int magn;

    for (int xIndex=0; xIndex< N; ++xIndex)
    {
        for (int yIndex=1; yIndex < WIDTH_WATERWALL; ++yIndex)
            colorMap->data()->setCell(xIndex, yIndex-1, colorMap->data()->cell(xIndex,yIndex));

        magn = int(powerA[xIndex]) % 255;
        colorMap->data()->setCell(xIndex, WIDTH_WATERWALL - 1, magn);
    }

    plotWaterfall->replot();
}

void MainWindow::on_rbAccumSpectrum_clicked()
{
    clearPeaks();
}

void MainWindow::on_rbPeakSpectrum_clicked()
{
    clearPeaks();
}

void MainWindow::on_sliderPeakSpectrCoeff_valueChanged(int value)
{
    peakSpectrCoeff = value * peakSpectrCoeffDelta;
    ui->lPeakSpectrCoeffValue->setText(QString("%1").arg(peakSpectrCoeff, 0, 'f', 5));
}

void MainWindow::on_hslAccumSpectrumDeep_valueChanged(int value)
{
    accumSpectrDeep = value;
    ui->lAccumSpectrumDeepValue->setText(QString::number(accumSpectrDeep));
    clearPeaks();
}


void MainWindow::on_dsbFreq_valueChanged(double arg1)
{
    _core->device()->setFreq(uint32_t(arg1 * 1.0e6));
    for(int i = 0; i < N; ++i)
        xAxis[i] =((double)i/N * FS - FS/2)/1.e6 +arg1;

    plotSpectrum->xAxis->setRange(arg1 - FS / 2 / 1.e6 , arg1 + FS / 2 / 1.e6);

    colorMap->data()->setRange(QCPRange(arg1 - FS / 2 / 1.e6 ,
                                        arg1 + FS / 2 / 1.e6),
                               QCPRange(0, WIDTH_WATERWALL));
    //colorMap->rescaleDataRange();
    plotWaterfall->rescaleAxes();
    clearPeaks();
}
