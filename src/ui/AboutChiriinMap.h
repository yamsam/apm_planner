#ifndef ABOUTCHIRIINMAP_H
#define ABOUTCHIRIINMAP_H

#include <QDialog>

namespace Ui {
class AboutChiriinMap;
}

class AboutChiriinMap : public QDialog
{
    Q_OBJECT

public:
    explicit AboutChiriinMap(QWidget *parent = 0);
    ~AboutChiriinMap();

private:
    Ui::AboutChiriinMap *ui;
};

#endif
