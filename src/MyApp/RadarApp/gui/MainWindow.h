#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
private:
    Ui::MainWindow *ui;
    GraphicsWidget* _graphicsWidget = nullptr;

};

#endif // MAINWINDOW_H
