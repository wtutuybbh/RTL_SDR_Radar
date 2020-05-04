#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include <QTimer>

class GraphicsWidget;

namespace Ui {
class MainWindow;
}

class QCustomPlot;
class QCPColorMap;
class Core;
class QCPItemStraightLine;
class QCPColorScale;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QTimer _timer;
    const int32_t _timeout = 16;
    const int N = 8192;
    const int WIDTH_WATERWALL = 50;
    const double FS = 2.0e6;
    QCustomPlot *_plotSpectrum;
    QCustomPlot *_plotWaterfall;
    Core* _core = nullptr;

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

public:
    explicit MainWindow(Core* core, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void slotUpdateWidgets();

    void on_rbAccumSpectrum_clicked();

    void on_rbPeakSpectrum_clicked();

    void on_sliderPeakSpectrCoeff_valueChanged(int value);

    void on_hslAccumSpectrumDeep_valueChanged(int value);

    void on_dsbFreq_valueChanged(double arg1);

};

#endif // MAINWINDOW_H
