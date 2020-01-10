#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "interface/ILogger.h"

class GraphicsWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addGraphicsWidget(GraphicsWidget* widget);
    void addTableWidget(QWidget* widget);
    void setReciverDeviceState(bool state);
    void setDBState(bool state);
    void setGPSState(bool state);

private:
    Ui::MainWindow *ui;
    GraphicsWidget* _graphicsWidget = nullptr;

    QString takeColorStrByState(bool state);

};

#endif // MAINWINDOW_H
