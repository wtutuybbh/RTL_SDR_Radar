#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

#include <QTimer>
#include "interface/ILogger.h"

class GraphicsWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSharedPointer<ILogger> _logger;
    QTimer _timer;

    int32_t timeout = 500;
    uint8_t COUNT_MSG_MIN = 10;
    uint8_t COUNT_MSG_MAX = 25;
    uint8_t COUNTER_BALANCE = 10;

    int32_t MAX_INTERVAL = 1000;
    int32_t MIN_INTERVAL = 10;
    int32_t COUNT_TEXT_LINE = 1000;
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
private:
    Ui::MainWindow *ui;
    GraphicsWidget* _graphicsWidget = nullptr;

    QString takeColorStrByState(bool state);

private slots:
    void updateTerminal();
    void on_rbRadarMode_clicked();
    void on_rbCartesianMode_clicked();
};

#endif // MAINWINDOW_H
