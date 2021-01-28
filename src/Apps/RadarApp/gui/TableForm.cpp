#include <QAbstractItemView>
#include <QHeaderView>
#include <QDebug>

#include "TableForm.h"
#include "ui_TableForm.h"
#include "ModelTable/ModelTable.h"

TableForm::TableForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableForm)
{
    ui->setupUi(this);
    _view = new QTableView(this);
    //заголовки
    _view->horizontalHeader()->setDefaultSectionSize(70);
    _view->horizontalHeader()->setMinimumSectionSize(70);
    _view->verticalHeader()->setDefaultSectionSize(45);
    _view->verticalHeader()->setMinimumSectionSize(45);
    _view->verticalHeader()->hide();
    _view->horizontalHeader()->setStretchLastSection(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->vltTableView->addWidget(_view);

    connect(_view,&QTableView::clicked,this,&TableForm::clicked);
}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::setTableModel(ModelTable *model)
{
    _view->setModel(model);

    _view->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    _view->setColumnWidth(TIME, 120);
    _view->setColumnWidth(GEOCOORD, 90);
    _view->setColumnHidden(UUID,true);

    _view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    _view->setSelectionMode(QAbstractItemView::SingleSelection);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void TableForm::clicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    emit signalSetObjectCurrent(_view->currentIndex().sibling(_view->currentIndex().row(),0).data().toUuid());
}
