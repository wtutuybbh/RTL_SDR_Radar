#ifndef MODELTABLE_H
#define MODELTABLE_H

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include <QItemSelection>

#include "modeltable_global.h"

#include "interface/IObserver.h"

class IPoolObject;
class IObject;
class ISubject;

enum Column {
    ID,
    NAME,
    TIME,
    AZIM,
    DIST,
    COURSE,
    ALTITUDE,
    SPEED,
    GEOCOORD,
    LAST
};

struct info
{
    uint64_t id;
    bool isBold = false;
};

class MODELTABLERTOSHARED_EXPORT ModelTable : public QAbstractTableModel,
        public IObserver
{
    Q_OBJECT
    typedef QHash< Column, QVariant > SrcData;
    QVector<info> idIndex;
    QHash< uint64_t,SrcData > _listSrc;

    void appendData(QSharedPointer<IObject> &object);
    void removeData(QSharedPointer<IPoolObject> &pool);
    void updateData(QSharedPointer<IObject> &object);

public:
    explicit ModelTable(QObject  *parent = 0);
    ~ModelTable() override;

    void subscribe(QSharedPointer<ISubject> subject) override;
    void unsubscribe(QSharedPointer<ISubject>)override;
    void update(QSharedPointer<IPoolObject> pool) override;

    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool setData( const QModelIndex& index, const QVariant& value, int role ) override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;

signals:
    void sigUpdateSelection(const QItemSelection &index,
                            QItemSelectionModel::SelectionFlags flags);
};

#endif // MODELTABLE_H
