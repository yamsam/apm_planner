#include "configuration.h"
#include "AboutChiriinMap.h"
#include "ui_AboutChiriinMap.h"


AboutChiriinMap::AboutChiriinMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutChiriinMap)
{
    ui->setupUi(this);
}

AboutChiriinMap::~AboutChiriinMap()
{
    delete ui;
}
