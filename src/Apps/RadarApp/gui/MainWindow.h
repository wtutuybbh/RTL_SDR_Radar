#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include <QTimer>
#include "interface/ILogger.h"
#include "interface/IDSP.h"

namespace Ui {
class MainWindow;
}

class GraphicsWidget;
class QCustomPlot;
class QCPColorMap;
class QCPItemStraightLine;
class QCPColorScale;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSharedPointer<ILogger> _logger;
    QSharedPointer<IDSP> _dsp;

    QTimer _timer;
    QTimer _timerChart;
    int32_t timeout = 500;
    uint8_t COUNT_MSG_MIN = 10;
    uint8_t COUNT_MSG_MAX = 25;
    uint8_t COUNTER_BALANCE = 10;

    int32_t MAX_INTERVAL = 1000;
    int32_t MIN_INTERVAL = 10;
    int32_t COUNT_TEXT_LINE = 1000;


    const int32_t _timeout = 16;
    const int N = 8192;
    const int WIDTH_WATERWALL = 50;
    const double FS = 2.0e6;
    const double CENTRAL_FREQ = 1090.0;
    QCustomPlot *_plotSpectrum;
    QCustomPlot *_plotWaterfall;


    QVector<double> _accumSpectrumStore;
    QVector<double> _powerPeak;
    QVector<double> _powerAccum;
    QVector<double> _xAxis;

    int _accumSpectrumDeep;            // Глубина накопления
    double _peakSpectrumCoeff = 0;         // Коэффициент памяти для пикового детектора
    double peakSpectrumCoeffDelta = 1.e-5;;
    int _currAccum = 0;   // Для накопления спектра
    int _countAccum = 0;  // Для накопления спектра
    QCPColorMap *_colorMap;
    QCPColorScale *_colorScale;

    Ui::MainWindow *ui;
    GraphicsWidget* _graphicsWidget = nullptr;

    void initChart();
    void clearPeaks();
    void drawWaterfall(const QVector<double> &power);
    QString takeColorStrByState(bool state);
    void logBalancing();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addGraphicsWidget(GraphicsWidget *widget);
    void addTableWidget(QWidget* widget);
    void setReciverDeviceState(bool state);
    void setDBState(bool state);
    void setGPSState(bool state);
    void setLogger(QSharedPointer<ILogger> log) { _logger = log; }

    void prepareDataForChart(SrcDataAdc &data);

    void setDsp(const QSharedPointer<IDSP> &dsp);

private slots:
    void updateTerminal();
    void on_rbRadarMode_clicked();
    void on_rbCartesianMode_clicked();

    void slotUpdateWidgets();

    void on_rbAccumSpectrum_clicked();

    void on_rbPeakSpectrum_clicked();

    void on_sliderPeakSpectrCoeff_valueChanged(int value);

    void on_hslAccumSpectrumDeep_valueChanged(int value);
    void on_tbWorkPanel_currentChanged(int index);
};
#endif // MAINWINDOW_H
