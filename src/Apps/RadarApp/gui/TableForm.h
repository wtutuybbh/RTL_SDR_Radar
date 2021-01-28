#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QWidget>
#include <QTableView>
#include <QUuid>

namespace Ui {
class TableForm;
}
class ModelTable;
class TableForm : public QWidget
{
    Q_OBJECT

public:
    explicit TableForm(QWidget *parent = 0);
    ~TableForm();
    void setTableModel(ModelTable* model);
private:
    Ui::TableForm *ui;
    QTableView* _view;
private slots:
    void clicked(const QModelIndex &index);
signals:
    void signalSetObjectCurrent(QUuid id);
};

#endif // TABLEFORM_H
