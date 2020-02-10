#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include "coord/Position.h"
#include "interface/IObject.h"

class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    ///< курс объекта
    float _rtAngle = 0;
    ///< флаг имитированного объекта
    bool _isImit = false;
    ///< геокоординаты объекта с которым связан значек
    Position _geoPos;
    ///< нужно удаление
    bool needDelete = false;
    ///< азимут
    double _azim;
    ///< текст для вывода
    QString _text;
    //загрузка иконки объекта
    void loadPixmap(OBJECT_TYPE type, bool isImit);
    ///< функция обработки операции выбора и и зменения элемента на сцене
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;
    ///< отрисовка иконки объекта
    void drawObjectIcon(bool imit);

public:
    /*!
     * \brief GraphicsObject - конструктор объекта
     * \param type - тип объекта
     * \param imit - флаг имитации
     */
    explicit GraphicsObject(OBJECT_TYPE type = OBJECT_TYPE::base,
                            bool imit = false);

    virtual ~GraphicsObject() override;

    /*!
     * \brief setRotateAngle - установка курса объета
     * \param rta - угол повората относительно севера
     */
    void setRotateAngle(float rta);
    /*!
     * \brief setGeoPosition - установка геоокоординат
     * \param pos - геоокординаты объекта
     */
    void setGeoPosition(const Position& pos) { _geoPos = pos; }
    /*!
     * \brief getGeoPosition - получение геоокоординат
     * \return  геоокординаты объекта,связанного со значком
     */
    Position getGeoPosition() { return _geoPos; }
    /*!
     * \brief getNeedDelete - проверка необходимости удаления объекта со сцены
     * \return true - нужно удалить объект
     */
    bool getNeedDelete() const;
    /*!
     * \brief setNeedDelete - пометка значка для удаления
     * \param value - флаг
     */
    void setNeedDelete(bool value);
    /*!
     * \brief getAzimuth - получение азимута расположения значка
     * \return азимут относительно севера
     */
    float getAzimuth() const;
    /*!
     * \brief setAzimuth- установка текущего значения азимута
     * \param azim - значение азимута
     */
    void setAzimuth(double azim);
    /*!
     * \brief setText - установка текста для отображения
     * \param text - текст сообщения для вывод,
     * разделитель "/"
     */
    void setText(const QString &text);

protected:
    ///< иконка объекта
    QPixmap _pixmapIcon;
    ///< размер значка
    QSize _sizeIcon = QSize(25,25);
    ///< значек выбран текущим
    bool _isSelect = false;
    ///< позиция на сцене
    QPointF _screenPos = QPointF(-10,-10);
    ///< цвет значка
    QColor _colorIcon =  QColor(0,250,0);

    //задаем область отрисовки
    QRectF boundingRect() const override;

    //метод рисования
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem* option,
               QWidget *widget = 0) override;

};

#endif // GRAPHICSOBJECT_H
