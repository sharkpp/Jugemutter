#ifndef VIEWSETTINGABOUTPAGE_H
#define VIEWSETTINGABOUTPAGE_H

#include <QFrame>

namespace Ui {
class ViewSettingAboutPage;
}

class ViewSettingAboutPage : public QFrame
{
    Q_OBJECT

public:
    explicit ViewSettingAboutPage(QWidget *parent = 0);
    ~ViewSettingAboutPage();

private:
    Ui::ViewSettingAboutPage *ui;
};

#endif // VIEWSETTINGABOUTPAGE_H
