#include <QDebug>
#include <QFont>
#include <QColor>
#include <QDateTime>

#include "ModelTable/ModelTable.h"

#include "interface/IObject.h"
#include "interface/ISubject.h"
#include "interface/IPoolObject.h"
#include "MapGraphics/coordUtils/Conversions.h"

ModelTable::ModelTable(QObject *parent): QAbstractTableModel( parent )
{
    qDebug()<<"create TableRTOModel";
}

ModelTable::~ModelTable()
{
    qDebug()<<"delete TableRTOModel";
}

void ModelTable::subscribe(QSharedPointer<ISubject> subject)
{
    if(subject.isNull())
    {
        qDebug()<<"[ModelTable::subscribe] : try subscribe nullptr";
        return;
    }

    subject->Attach(this);
}

void ModelTable::unsubscribe(QSharedPointer<ISubject> subject)
{
    qDebug()<<"unsubscribe ModelTable";

    if(!subject.isNull())
        subject->Deatach(this);

    subject.clear();
}

void ModelTable::update(QSharedPointer<IPoolObject> pool)
{
    if( pool.isNull() )
    {
        qDebug()<<"GraphicsWidget::update() :: nullptr";
        return;
    }

    idIndex.clear();
    idIndex.resize(pool->values().size());

    removeData(pool);
    int indx = 0;
    for(auto &iter: pool->values())
    {
        if(!_listSrc.contains(iter->getId()) &&
                iter->getInUse())
            appendData(iter);
        else
            if(iter->getInUse())
            {
                updateData(iter);
            }
        if(indx >= 0 && indx < idIndex.size())
        {
            idIndex[indx].id = iter->getId();
            idIndex[indx].isBold = false;
            if(iter->getObjectState() == OBJECT_STATE::NEW_OBJECT ||
                    iter->getObjectState() == OBJECT_STATE::UPDATE_OBJECT)
                idIndex[indx].isBold = true;
            ++indx;
        }
    }

    emit dataChanged(QModelIndex(), QModelIndex());

}

int ModelTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _listSrc.count();
}

int ModelTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return LAST;
}

QVariant ModelTable::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() ||
            _listSrc.count() <= index.row() ||
            ( role != Qt::DisplayRole &&
              role != Qt::EditRole &&
              role != Qt::TextAlignmentRole &&
              role != Qt::FontRole)
            )
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if(index.row() >= 0 /*&& index.row() < idIndex.count()*/)
    {
        if (role == Qt::FontRole)
        {
            QFont boldFont("Noto Sans", 10);
            boldFont.setBold(true);

            return boldFont;
        }

        return _listSrc[idIndex[index.row()].id][Column(index.column())];
    }
    else
        return QVariant();
}

bool ModelTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(value);
    if( !index.isValid() ||
            role != Qt::EditRole ||
            _listSrc.count() <= index.row() )
        return false;

    //    _listSrc[ index.row() ][ Column( index.column() ) ] = value;
    //    emit dataChanged( index, index );

    return true;
}

QVariant ModelTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole )
        return QVariant();

    if( orientation == Qt::Vertical )
        return section;

    switch(section )
    {
        case ID:
            return trUtf8("Идент.");
        case NAME:
            return trUtf8("Наимен.");
        case TIME:
            return trUtf8("t обнар. /\nt обнов.");
        case AZIM:
            return trUtf8("П,°");
        case DIST:
            return trUtf8("D,км");
        case COURSE:
            return trUtf8("Курс");
        case ALTITUDE:
            return trUtf8("Высота");
        case SPEED:
            return trUtf8("Скорость");
        case GEOCOORD:
            return trUtf8("Ш,°/\nД,°");
    }

    return QVariant();
}

Qt::ItemFlags ModelTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    // if( index.isValid() )
    //        if( index.column() == SELECTION )
    //            flags |= Qt::ItemIsEditable;

    return flags;
}


void ModelTable::appendData(QSharedPointer<IObject> &object)
{
    if(object.isNull())
        return;

    SrcData data;

    data.insert(ID,QString("%1")
                .arg(QString::number(object->getId(),16)));

    data.insert(NAME,object->getObjectName());

    data.insert(TIME,QString("%1\n%2")
                .arg(object->getDateTimeStart().toString("dd.MM.yy hh:mm:ss"))
                .arg(object->getDateTimeStop().toString("dd.MM.yy hh:mm:ss")));

    data.insert(AZIM,QString("%1")
                .arg(object->getAzimuth(),0, 'f', 1));

    data.insert(DIST, QString("%1").arg(object->getDistance_KM()));
    data.insert(COURSE, QString("%1").arg(object->getCourse(),0,'f',1));
    data.insert(ALTITUDE, QString("%1").arg(object->getAltitude(),0,'f',1));
    data.insert(SPEED, QString("%1").arg(object->getSpeed(),0,'f',1));

    data.insert(GEOCOORD,QString("%1\n%2")
                .arg(object->getGeoCoord().latitude())
                .arg(object->getGeoCoord().longitude()));

    int row = _listSrc.count();
    beginInsertRows( QModelIndex(), row, row );
    _listSrc.insert(object->getId(), data);
    endInsertRows();

    //    if(object->isSelectedObject())
    //        emit sigUpdateSelection(QItemSelection(index(object->getIndex(),_ID),index(object->getIndex(),_GEOCOORD)),
    //                                QItemSelectionModel::ClearAndSelect);
}


void ModelTable::removeData(QSharedPointer<IPoolObject> &pool)
{
    if(pool.isNull())
    {
        qDebug()<<"ModelTableRTO::::removeData() :: nullptr detect";
        return;
    }

    int i = 0;
    QHash< uint64_t,SrcData >::iterator it = _listSrc.begin();
    while( it != _listSrc.end() )
    {
        if(!pool->isExistsObject(it.key()))
        {
            beginRemoveRows( QModelIndex(), i, i );
            it = _listSrc.erase( it );
            endRemoveRows();
        }
        else
        {
            ++i;
            ++it;
        }
    }
}

void ModelTable::updateData(QSharedPointer<IObject> &object)
{
    if(object.isNull())
        return;

    if(!_listSrc.contains(object->getId()))
        return;

    _listSrc[object->getId()][ID] = QString("%1")
            .arg(QString::number(object->getId(),16));

    _listSrc[object->getId()][NAME] = object->getObjectName();

    _listSrc[object->getId()][TIME] = QString("%1\n%2")
            .arg(object->getDateTimeStart().toString("dd.MM.yy hh:mm:ss"))
            .arg(object->getDateTimeStop().toString("dd.MM.yy hh:mm:ss"));

    _listSrc[object->getId()][AZIM] = QString("%1")
            .arg(object->getAzimuth(),0, 'f', 1);

    _listSrc[object->getId()][DIST] = QString("%1").arg(object->getDistance_KM());

    _listSrc[object->getId()][COURSE] = QString("%1")
            .arg(object->getCourse(),0, 'f', 1);

    _listSrc[object->getId()][ALTITUDE] = QString("%1")
            .arg(object->getAltitude(),0, 'f', 1);

    _listSrc[object->getId()][SPEED] = QString("%1")
            .arg(object->getSpeed(),0, 'f', 1);

    QString lat, lon;
    Conversions::geoCoordToString(object->getGeoCoord(),lon,lat);
    QString str_geo = QString("%1\n%2")
            .arg(lat)
            .arg(lon);

    _listSrc[object->getId()][GEOCOORD] = str_geo;
}



