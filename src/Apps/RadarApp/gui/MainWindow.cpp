#include "MainWindow.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GraphicsWidget/GraphicsWidget.h"
#include "qcustomplot/qcustomplot.h"

#include "../AppCore/Core.h"
#include "interface/IDataController.h"
#include "interface/IDSP.h"

#include <Carrier/ServiceLocator.h>


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

    initChart();

    QObject::connect(&_timerChart, &QTimer::timeout,
                     this, &MainWindow::slotUpdateWidgets);

    _accumSpectrumDeep = ui->hslAccumSpectrumDeep->value();
    ui->lAccumSpectrumDeepValue->setText(QString::number(_accumSpectrumDeep));

    _accumSpectrumStore = QVector<double>(N * ui->hslAccumSpectrumDeep->maximum(),0.0);
    _powerPeak = QVector<double>(N, 0 );
    _powerAccum = QVector<double>(N, 0 );

    peakSpectrumCoeffDelta = 1.e-5;
    _peakSpectrumCoeff = ui->hslPeakSpectrumCoeff->value() * peakSpectrumCoeffDelta;
    ui->lPeakSpectrCoeffValue->setText(QString("%1").arg(_peakSpectrumCoeff, 0, 'f', 5));

    Position pos = ServiceLocator::getCarrier()->getGeoCoord();
    ui->dspLatitude->setValue(pos.latitude());
    ui->dspLongitude->setValue(pos.longitude());
}

MainWindow::~MainWindow()
{
    writeSettings();
    _timer.stop();
    delete _plotSpectrum;
    delete _plotWaterfall;

    _accumSpectrumStore.clear();
    _powerPeak.clear();
    _powerAccum.clear();
    _xAxis.clear();
    _dsp.clear();
    delete ui;
}

void MainWindow::addGraphicsWidget(GraphicsWidget* widget)
{
    _graphicsWidget = widget;
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

void MainWindow::setDsp(const QSharedPointer<IDSP> &dsp)
{
    _dsp = dsp;
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

void MainWindow::on_rbRadarMode_clicked()
{
    _graphicsWidget->setDisplayMode(DisplayMode::RADAR);
}

void MainWindow::on_rbCartesianMode_clicked()
{
    //_graphicsWidget->setDisplayMode(DisplayMode::CARTESIAN);
    QMessageBox msgBox;
    msgBox.setText("Извините, данный функционал находится в разработке \n Sorry, this feature is under development.");
    msgBox.exec();
}

void MainWindow::on_rbCirlceMode_clicked()
{
    _graphicsWidget->setDisplayMode(DisplayMode::CIRCLE);
}

void MainWindow::initChart()
{
    _xAxis.resize(N);

    for(int i = 0; i < N; ++i)
        _xAxis[i] =((double)i/N * FS - FS/2)/1.e6 + CENTRAL_FREQ;


    _plotSpectrum = new QCustomPlot();
    _plotSpectrum->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _plotSpectrum->setOpenGl(true);
    QCPGraph *graph = _plotSpectrum->addGraph();
    graph->setPen(QPen(QColor(0xAA,0xCF,0xD1)));
    graph->rescaleKeyAxis();

    _plotSpectrum->yAxis->setRange(-10,256);
    _plotSpectrum->xAxis->setRange(CENTRAL_FREQ - FS / 2 / 1.e6 ,
                                   CENTRAL_FREQ + FS / 2 / 1.e6);
    _plotSpectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    _plotSpectrum->setBackground(QBrush(Qt::black));

    _plotSpectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    _plotSpectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    _plotSpectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    _plotSpectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    _plotSpectrum->xAxis->setTickLabelColor(Qt::white);
    _plotSpectrum->yAxis->setTickLabelColor(Qt::white);
    _plotSpectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    _plotSpectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    _plotSpectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    _plotSpectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    _plotSpectrum->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    _plotSpectrum->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->hrlOscilloscope->addWidget(_plotSpectrum);

    _plotWaterfall = new QCustomPlot();
    _plotWaterfall->setOpenGl(true);
    // configure axis rect:
    _plotWaterfall->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    _plotWaterfall->axisRect()->setupFullAxesBox(true);


    // set up the QCPColorMap:
    _colorMap = new QCPColorMap(_plotWaterfall->xAxis, _plotWaterfall->yAxis);

    _colorMap->data()->setSize(N, WIDTH_WATERWALL); // we want the color map to have nx * ny data points
    _colorMap->data()->setRange(QCPRange(CENTRAL_FREQ - FS / 2 / 1.e6 ,
                                         CENTRAL_FREQ + FS / 2 / 1.e6),
                                QCPRange(0, WIDTH_WATERWALL)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:


    // add a color scale:
    _colorScale = new QCPColorScale(_plotWaterfall);
    //plotWaterfall->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
    //colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    _colorScale->setDataRange(QCPRange(0,255));
    _colorMap->setColorScale(_colorScale); // associate the color map with the color scale

    // set the color gradient of the color map to one of the presets:
    _colorMap->setGradient(QCPColorGradient::gpJet);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.
    _colorMap->rescaleDataRange();
    _plotWaterfall->rescaleAxes();
    _plotWaterfall->setBackground(QBrush(Qt::black));

    _plotWaterfall->xAxis->setBasePen(QPen(Qt::white, 1));
    _plotWaterfall->xAxis->setTickPen(QPen(Qt::white, 1));
    _plotWaterfall->xAxis->setSubTickPen(QPen(Qt::white, 1));
    _plotWaterfall->xAxis->setTickLabelColor(Qt::white);
    _plotSpectrum->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);;

    ui->vltWaterfall->addWidget(_plotWaterfall);
}


void MainWindow::prepareDataForChart(SrcDataAdc& data)
{
    if(ui->rbRealSpectrum->isChecked())
    {
        _plotSpectrum->graph()->setData(_xAxis, data.fftVector);
        drawWaterfall(data.fftVector);
    }

    if(ui->rbSignalEnvelope->isChecked())
    {
        _plotSpectrum->graph()->setData(_xAxis, data.magnitudeVector);
        drawWaterfall(data.magnitudeVector);
    }

    if(ui->rbAccumSpectrum->isChecked())
    {
        _countAccum++;

        if (_countAccum > _accumSpectrumDeep)
            _countAccum = _accumSpectrumDeep;

        for(int i = 0 ; i < N; ++i)
        {
            _powerPeak[i] -= _accumSpectrumStore[_currAccum*N + i];

            _accumSpectrumStore[_currAccum*N + i] = data.magnitudeVector[i];

            _powerPeak[i] += data.magnitudeVector[i];

            _powerAccum[i] = _powerPeak[i];

            _powerAccum[i] /= _countAccum;
        }

        _currAccum +=1;
        _currAccum  %= _accumSpectrumDeep;

        _plotSpectrum->graph()->setData(_xAxis, _powerAccum);
        drawWaterfall(_powerAccum);
    }

    if (ui->rbPeakSpectrum->isChecked())
    {
        for (int i=0; i<N; ++i)
        {
            if (data.magnitudeVector[i] > _powerPeak[i])
                _powerPeak[i] = data.magnitudeVector[i];
            else
                _powerPeak[i] *= _peakSpectrumCoeff;
        }
        _plotSpectrum->graph()->setData(_xAxis,_powerPeak);
        drawWaterfall(_powerPeak);
    }
}

void MainWindow::slotUpdateWidgets()
{
    if(_dsp.isNull() || !_dsp->tryLockDataBuffer())
    {
        _plotSpectrum->replot();
        _plotWaterfall->replot();
        return;
    }

    if(_dsp->isEmptyDataBuffer())
    {
        _dsp->unlockDataBuffer();
        _plotSpectrum->replot();
        _plotWaterfall->replot();
        return;
    }

    SrcDataAdc data = _dsp->takeDataBlock();
    _dsp->unlockDataBuffer();

    prepareDataForChart(data);

    _plotSpectrum->replot();
    _plotWaterfall->replot();
}

void MainWindow::clearPeaks()
{
    if (ui->rbAccumSpectrum->isChecked())
        std::fill_n(_powerPeak.data(), N,0);
    else
        std::fill_n(_powerPeak.data(), N,-1000);

    std::fill_n(_accumSpectrumStore.data(),N * ui->hslAccumSpectrumDeep->maximum(), 0);

    std::fill_n(_powerAccum.data(),N, 0);

    _currAccum = 0;
    _countAccum = 0;
}

void MainWindow::drawWaterfall(const QVector<double> &power)
{
    for (int xIndex=0; xIndex< N; ++xIndex)
    {
        for (int yIndex=1; yIndex < WIDTH_WATERWALL; ++yIndex)
            _colorMap->data()->setCell(xIndex, yIndex-1, _colorMap->data()->cell(xIndex,yIndex));

        _colorMap->data()->setCell(xIndex, WIDTH_WATERWALL - 1, int(power[xIndex]) % 255);
    }
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
    _peakSpectrumCoeff = value * peakSpectrumCoeffDelta;
    ui->lPeakSpectrCoeffValue->setText(QString("%1").arg(_peakSpectrumCoeff, 0, 'f', 5));
}

void MainWindow::on_hslAccumSpectrumDeep_valueChanged(int value)
{
    _accumSpectrumDeep = value;
    ui->lAccumSpectrumDeepValue->setText(QString::number(_accumSpectrumDeep));
    clearPeaks();
}


void MainWindow::on_tbWorkPanel_currentChanged(int index)
{
    if(index == 1)
        _timerChart.start(_timeout);
    else
        _timerChart.stop();
}

void MainWindow::slotConnectToServerState(bool state)
{
    ui->lDBStatus->setStyleSheet(takeColorStrByState(state));
}

void MainWindow::slotNetworkExchange(uint64_t bytesSend, uint64_t errorrCount)
{
}

void MainWindow::on_pdSetCoordinate_clicked()
{
    const Position pos = Position(ui->dspLongitude->value(), ui->dspLatitude->value());
     _graphicsWidget->setCentralCoordinate(pos);
     writeSettings();
}

void MainWindow::writeSettings()
 {
     QSettings settings("RTL_SDR Soft", "RTL_SDR_Radar");

     settings.beginGroup("GeoCoordinate");
     settings.setValue("latitude",  ui->dspLatitude->value());
     settings.setValue("longitude", ui->dspLongitude->value());
     settings.endGroup();

     settings.beginGroup("mainwindow");
     settings.setValue("size", this->size());

     settings.beginGroup("language");
     settings.setValue("index", ui->cmbLanguage->currentIndex());
     settings.endGroup();
 }

void MainWindow::on_pbSetLanguage_clicked()
{
    QString strDir = QApplication::applicationFilePath();
    QDir appDir(strDir);
    appDir.cdUp();
    appDir.cdUp();
    appDir.cdUp();
    appDir.cdUp();
    appDir.cd("./export");

    qtLanguageTranslator.load( appDir.path() + "/" + QString("QtLanguage_en"));
    qApp->installTranslator(&qtLanguageTranslator);
    ui->retranslateUi(this);


    emit setEnglishLanguage(true);
}
