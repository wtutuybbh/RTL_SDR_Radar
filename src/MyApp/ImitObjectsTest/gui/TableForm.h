#ifndef TABLEFORM_H
#define TABLEFORM_H

#include <QWidget>
#include <QTableView>

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

};

#endif // TABLEFORM_H
