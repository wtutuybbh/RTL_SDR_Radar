#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include "graphicswidget_global.h"

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDebug>
#include <QThread>
#include <QGLWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QTimer>
#include <math.h>

#include "interface/IPoolObject.h"
#include "interface/IObject.h"
#include "../Subject/Subject.h"
#include "implements/MapController.h"
#include "coord/Position.h"
#include "../Carrier/ServiceLocator.h"


class GRAPHICSWIDGETSHARED_EXPORT GraphicsWidget: public QGraphicsView,
        public IObserver
{
    Q_OBJECT
    ///< пул РТО объектов
    QSharedPointer<IPoolObject> _ptrPoolObject = nullptr;
    ///< контроллер карты
    QSharedPointer<IMapController> _ptrMapController = nullptr;
    ///< класс носителя приемниак rtl-sdr
    QSharedPointer<ICarrierClass> _ptrCarrier = nullptr;

    ///< обновление сцены
    QTimer _timer;
    uint32_t TIMEOUT = 25;

    QGraphicsScene * _scene = nullptr;
    QConicalGradient gradient;

    QPointF _fixCursorCoord;
    QPointF _cursorCoord;
    //пиксмапы для курсоров
    QPixmap _pxmCursor;
    QPixmap _pxmSelect;
    ///<флаг фиксации курсора на сцене
    bool _fixCursor = false;
    const QSize _cursorSize = QSize(45,45);

    double _distToBorderMap = 0.0;
    int  _mapZoom = 8;

    const int grad = 360;
    int32_t _angleGradientSector = 0;
    uint16_t _sectorSize = 45;
    bool _updateInSector = true;

    const double _textBorder = 28;

    ///< константы для палитры
    const QColor _clrTron       = QColor(0xAA,0xCF,0xD1);
    const QColor _clrTronAlpha  = QColor(0xAA,0xCF,0xD1,100);
    const QColor _clrGreen      = QColor(0,128,0);
    const QColor _clrRed        = QColor(250,0,0);
    const QColor _clrBlue       = QColor(0,204,255);
    const QColor _clrYellow     = QColor(255,255,0);
    const QColor _clrWhite      = QColor(250,250,250);
    const QColor _clrGrey       = QColor(150,150,150);

    ///< вектор для хранения пеленгов объектов,которые не попадают в масштаб карты
    QVector<double > _vHiddenObject;

    /*!
     * \brief initWidget инициализация виджета для отображения карты и РТО объектов
     * \param size - размер сцены. \warning ширина == высоте
     * \param enableOpenGL - включить поддержку openGl
     */
    void initWidget(const QRect &size, bool enableOpenGL = false);
    /*!
     * \brief initCursors - инициализация своего типа курсоров
     */
    void initCursors();
    /*!
     * \brief changeCursorType - загрузка своего типа курсора
     * \param enableSystem - использовать системные (true)
     * или пользовательские (false)
     */
    void changeCursorType(bool enableSystem);

    /*!
     *  \brief drawBackground  - отрисовка карты,лимба, по умолчанию данная функция
     * кэширует изображение
     *  \param painter - укказатель на класс для рисования
     *  \param rect - область отрисовки
     */
    void drawBackground ( QPainter * painter, const QRectF & rect ) override;
    /*!
     * \brief drawForeground - отрисовка значков объектов, всех изменние и анимации
     *  не кэшируется
     *  \param painter - укказатель на класс для рисования
     *  \param rect - область отрисовки
     */
    void drawForeground(QPainter *painter, const QRectF &rect) override;


    //    //узнать масштаб в километрах
    //    double getDistanceRadarScale();
    //    //рассчет дистанции до объекта
    //    double getDistanceObject(const Position &pos);

    /*!
     * \brief переопределение событий мыши
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent * event) override;
    void wheelEvent (QWheelEvent * event) override;


    //    // из координаты в экранную точку
    //    QPointF geoToScreen(const Position &coord);
    //    // Преобразования из точки экрана в координаты
    //    Position screenToGeo(QPointF view);
    //    void recalcCoordGraphObj();

    /*!
     * \brief getSceneCenterPont получение центральных координат сцены
     * \return координаты центра сцены
     */
    QPointF getSceneCenterPont();
public:
    /*!
     * \brief GraphicsWidget конструктор класса для отображения карты и объектов
     * \param widthRect - размер квадрата виджета
     * \param poolObject - указатель на пул объектов, которые необходимо отображать
     * \param parent - родительский класс
     */
    explicit GraphicsWidget(uint32_t widthRect = 600,
                            QSharedPointer<IPoolObject> poolObject = QSharedPointer<IPoolObject>(),
                            QWidget *parent = 0);
    ~GraphicsWidget() override;

    /*!
     * \brief setMapContoller установка указателя на класс котроллер карты
     * \param ptr - умный указатель на класс контроллер
     */
    void setMapContoller(QSharedPointer<IMapController> ptr)
    {
        _ptrMapController = ptr;
    }

    /*! \brief поддержка паттерна Observer */
    /*!
     * \brief subscribe - подписка на события обновления пула объектов
     * \param poolObject - указатель на пул объектов
     */
    void subscribe(QSharedPointer<IPoolObject> poolObject);
    /*!
     * \brief unsubscribe - отписаться от событий обновления пула объектов
     */
    void unsubscribe() override;
    /*!
     * \brief update - событие обновления пула объектов
     */
    void update(Subject* sub) override;

protected:
    /*!
     * \brief drawMap - отрисовка карты в режиме лимба
     * и индикатора радара
     * \param painter - указатель на классдля рисования
     * \param isDraw - true - рисовать карту, false - черный квадрат
     */
    virtual void drawMap(QPainter *painter, bool isDraw = true);
    /*!
     * \brief resizeEvent - изменнение размера виджета
     * \param event - событие изменение размера
     */
    void resizeEvent(QResizeEvent *event) override;
    /*!
     * \brief drawDotCicleWithLabel - отрисовка лимба с метками
     * \param p - указатель на класс для отрисовки
     * \param rad - расстояние от центра виджета для меток лимба
     */
    virtual void drawDotCicleWithLabel(QPainter *p, qreal rad);
    /*!
     * \brief drawCarrier - отрисовка значка объекта носителя
     * приемника rtl-sdr
     * \param p - указатель на класс для отрисовки
     */
    virtual void drawCarrier(QPainter *p);
    /*!
     * \brief drawHiddenObject отрисовка на лимбе отметок объектов,
     * которые не попали в масштаб карты
     * \param p - - указатель на класс для отрисовки
     */
    virtual void drawHiddenObject(QPainter*p);
    /*!
     * \brief drawText - функция вывода текста на виджет
     * \param p - класс, отвечающий за отрисовку
     * \param X - координата начала текста (0 - левый верхний угол)
     * \param Y - координата начала текста (0 - левый верхний угол)
     * \param str - строка текста для отображения
     * \param drawBorder - вывод внутри рамки
     */
    virtual void drawText(QPainter *p,
                          double X,
                          double Y,
                          const QString &str,
                          bool drawBorder = false);
    /*!
     * \brief printCoord - вывод экранных и геокоординат
     * в область виджетв
     * \param p - класс, отвечающий за отрисовку
     */
    virtual void printCoord(QPainter *p);
    /*!
     * \brief updateObjectOnScene - обновление РТО объекта на сцене.
     * Если объект новый - добавление на сцену
     * Если объект существует на сцене - обновление его параметров
     * Если объект устарел  - удаление его со сцены
     * \param object
     */
    virtual void updateObjectOnScene(QSharedPointer<IObject> &object);
    /*!
     * \brief recalculateCoordObjects - перерасчёт координат объекта на сцене
     *  при изменении масштаба
     */
    virtual void recalculateCoordObjects();
    /*!
     * \brief getDataForTable-получение массива информации с описание РТО объекта
     * Данные передаются в таблицу информации о текущем объекте
     * \param object - текущий выбранный объект
     * \return QStringList параметров для вывода в таблицу
     */
    virtual QStringList getDataForTable(IObject *object);

private slots:
    /*!
     * \brief timeout - обновление положение сектора в радаре
     */
    void timeout();
    /*!
     *  \brief увеличение и уменьшение масштаба карты
     */
    void RadarScalePlus();
    void RadarScaleMinus();
    /*!
     * \brief updateScene - обновление сцены со сбросом кэша
     */
    void updateScene();
public slots:

    /*!
     * \brief slotUpdateData - слот обновления данных
     * с захватом блокировки на пуле объектов
     */
    void slotUpdateData();
signals:
    /*!
     * \brief signalDataToTable - сигнал передачи параметров текущего,
     *  радиотехнического объекта
     * \param list - список параметров РТО объектов
     */
    void signalDataToTable(QStringList list);
};


#endif // GRAPHICSWIDGET_H
