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
    ServiceLocator::provide(QSharedPointer<ICarrierClass>( new NullCarrier()) );

    _poolObjects = QSharedPointer<IPoolObject>(new PoolObject(OBJECT_TYPE::air));

    addImitObject();

    _subject = QSharedPointer<ISubject>(new Subject());

    _mainWindow  = new MainWindow();

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
}


void Core::updateGeoPositionInfo()
{
    for (auto &iter: _poolObjects->allValues())
    {
        if(!iter.isNull() && iter->isValidGeoCoord())
        {

            iter->setAzimuth(Conversions::LBToPeleng(ServiceLocator::getCarrier()->getGeoCoord(),
                                                     iter->getGeoCoord()));


            iter->setGeoCoord(Conversions::getPlace(ServiceLocator::getCarrier()->getGeoCoord(),
                                                    int(iter->getAzimuth() + 1) % 360,
                                                    40000 + (qrand()%5000)*10));

            iter->setDistance_M(Conversions::getDistance(ServiceLocator::getCarrier()->getGeoCoord(),
                                                         iter->getGeoCoord()));
            iter->setDateTimeStop(QDateTime::currentDateTime());

        }
    }
}

void Core::addImitObject()
{
    _poolObjects->lockPool();

    for (int i = 1; i < 10; i++)
    {
        auto object = _poolObjects->createNewObject(i,
                                                    QDateTime::currentDateTime(),
                                                    Conversions::getPlace(ServiceLocator::getCarrier()->getGeoCoord(),
                                                                          (qrand()+10) % 360,
                                                                          20000),
                                                    true);

        object->setObjectName("ImitObject_"+QString::number(i));
    }

    auto object = _poolObjects->createNewObject(424346,
                                                QDateTime::currentDateTime(),
                                                Conversions::getPlace(ServiceLocator::getCarrier()->getGeoCoord(),
                                                                      (qrand()+10) % 360,
                                                                      20000),
                                                false);
    object->setObjectName("AFL1625");



    _poolObjects->unlockPool();

}


