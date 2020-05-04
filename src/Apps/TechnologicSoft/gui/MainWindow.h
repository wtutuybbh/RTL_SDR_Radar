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
    int32_t timeout = 25;
    QCustomPlot *plotSpectrum;
    QCustomPlot *plotWaterfall;
    Core* _core = nullptr;

    int accumSpectrDeep;            // Глубина накопления
    QVector<double> accumSpectrStore;
    QVector<double> powerPeakA;
    QVector<double> powerAccumA;
    QVector<double> xAxis;

    int currAccum = 0;   // Для накопления спектра
    int countAccum;  // Для накопления спектра
    const int N = 8192;
    const int WIDTH_WATERWALL = 50;
    double FS = 2.0e6;

    double peakSpectrCoeff = 0;         // Коэффициент памяти для пикового детектора
    double peakSpectrCoeffDelta = 1.e-5;;
    QPixmap wf;

    QCPColorMap *colorMap;
    QCPColorScale *colorScale;
public:
    explicit MainWindow(Core* core, QWidget *parent = nullptr);
    ~MainWindow();

    QCustomPlot *getCustomPlot()  { return  plotSpectrum; }

private slots:
    void on_pbFreq_clicked();

    void slotUpdateWidgets();

    void on_rbAccumSpectrum_clicked();

    void on_rbPeakSpectrum_clicked();

    void on_sliderPeakSpectrCoeff_valueChanged(int value);

    void on_hslAccumSpectrumDeep_valueChanged(int value);

    void on_dsbFreq_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    GraphicsWidget* _graphicsWidget = nullptr;

    void clearPeaks();
    void fillWaterfallDN(const QVector<double> &powerA);
};

#endif // MAINWINDOW_H
