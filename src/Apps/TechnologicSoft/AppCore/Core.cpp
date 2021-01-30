#include "Core.h"
#include <QApplication>

#include "DataController/DataController.h"
#include "RTL_SDR_Reciver/RTL_SDR_Reciver.h"
#include "Demodulator/Demodulator.h"
#include "DSPLib/DSPLib.h"
#include "qcustomplot/qcustomplot.h"
#include "Carrier/ServiceLocator.h"
#include "osm/coordUtils/Position.h"

QSharedPointer<IReciverDevice> Core::device() const
{
    return _device;
}

QSharedPointer<IDSP> Core::dsp() const
{
    return _dsp;
}

QSharedPointer<IDataController> Core::dataController() const
{
    return _dataController;
}

Core::Core(QObject *parent) : QObject(parent)
{

    QFile File(QApplication::applicationDirPath()+"/../../../import/style.qss");
    if(File.open(QFile::ReadOnly))
    {
        QString StyleSheet = QLatin1String(File.readAll());
        qApp->setStyleSheet(StyleSheet);
    }
    else
        qDebug()<<"error load QSS file.Need filepath"
               <<QApplication::applicationDirPath()+"/import/style.qss";
}

Core::~Core()
{
    _dataController->stop();
    _dataController.clear();

    _device->closeDevice();
    _device.clear();

    _dsp.clear();

    delete _mainWindow;
}

void Core::init()
{  
    //модуль взаимодействия с приемником
    _device = QSharedPointer<IReciverDevice>(new RTL_SDR_Reciver());
    _device->openDevice();

    _dsp = QSharedPointer<IDSP> (new DSPLib());

    //контроллер данных
    _dataController = QSharedPointer<IDataController>(new DataController(_device,
                                                                         nullptr));
    _dataController->setDSP(_dsp);

    _dataController->run();
}






