#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include <QFrame>

namespace Ui {
class ViewSetting;
}

class ViewSetting : public QFrame
{
    Q_OBJECT

public:
    explicit ViewSetting(QWidget *parent = 0);
    ~ViewSetting();

protected:
    void initToolbar();

private slots:

private:
    Ui::ViewSetting *ui;
};

#endif // VIEWSETTING_H