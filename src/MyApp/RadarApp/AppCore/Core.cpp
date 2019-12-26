#include "Core.h"
#include <QApplication>

#include "../MyLib/RTL_SDR_RadarLib/PoolObject/PoolObject.h"
#include "../MyLib/RTL_SDR_RadarLib/Logger/Logger.h"
#include "../MyLib/RTL_SDR_RadarLib/Carrier/Carrier.h"
#include "../MyLib/RTL_SDR_RadarLib/Carrier/ServiceLocator.h"
#include "../MyLib/RTL_SDR_RadarLib/DataController/DataController.h"
#include "../MyLib/RTL_SDR_RadarLib/RTL_SDR_Reciver/RTL_SDR_Reciver.h"
#include "../MyLib/RTL_SDR_RadarLib/Demodulator/Demodulator.h"
#include "../MyLib/RTL_SDR_RadarLib/GraphicsWidget/GraphicsWidget.h"
#include "../MyLib/RTL_SDR_RadarLib/ModelTable/ModelTable.h"
#include "../include/coord/Conversions.h"

#include "publisher/Subject.h"


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

    _demodulator.clear();

    _subject->Deatach(_graphicsWidget);
    delete _graphicsWidget;

    _subject->Deatach(_modelTable);
    delete _modelTable;

    _subject.clear();

    _poolObjects.clear();

    delete _mainWindow;
}

void Core::init()
{  
    _logger = QSharedPointer<ILogger>(new Logger(sizeLog));
    _device = QSharedPointer<IReciverDevice>(new RTL_SDR_Reciver());
    _device->setLogger(_logger);
    _device->openDevice();


    _poolObjects = QSharedPointer<IPoolObject>(new PoolObject(OBJECT_TYPE::air));

    _demodulator = QSharedPointer<IDemodulator>(new Demodulator(_poolObjects));
    _demodulator->setLogger(_logger);

    _dataController = QSharedPointer<IDataController>(new DataController(_device,
                                                                         _demodulator));
    _dataController->run();

    ServiceLocator::provide(QSharedPointer<ICarrierClass>( new NullCarrier()) );
    _subject = QSharedPointer<ISubject>(new Subject());

    _mainWindow  = new MainWindow();
    _mainWindow->setLogger(_logger);

    _graphicsWidget = new GraphicsWidget(SIZE_WIDGET, _mainWindow);
    _graphicsWidget->subscribe(_subject);
    _mainWindow->addGraphicsWidget(_graphicsWidget);

    _modelTable = new ModelTable(_mainWindow);
    _modelTable->subscribe(_subject);

    _tableSrc = new TableForm(_mainWindow);
    _tableSrc->setTableModel(_modelTable);
    _mainWindow->addTableWidget(_tableSrc);

    _mainWindow->adjustSize();
    _mainWindow->show();


    QObject::connect(&_timerUpdateWidgets, &QTimer::timeout,
                     this, &Core::slotUpdateWidgets);
    _timerUpdateWidgets.start(TIMEOUT_UPDATE);
}


void Core::slotUpdateWidgets()
{
    if(_poolObjects.isNull())
        return;

    if(_poolObjects->tryLockPool())
    {
        updateGeoPositionInfo();
        _subject->Notify(_poolObjects);
        _poolObjects->unlockPool();
    }

    if(!_device.isNull() && (_mainWindow != nullptr))
        _mainWindow->setReciverDeviceState(_device->isOpenDevice());
}


void Core::updateGeoPositionInfo()
{
    for (auto &iter: _poolObjects->allValues())
    {
        if(!iter.isNull() && iter->isValidGeoCoord())
        {

            iter->setAzimuth(Conversions::LBToPeleng(ServiceLocator::getCarrier()->getGeoCoord(),
                                                     iter->getGeoCoord()));

            iter->setDistance_M(Conversions::getDistance(ServiceLocator::getCarrier()->getGeoCoord(),
                                                         iter->getGeoCoord()));
        }
    }
}
