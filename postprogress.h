#ifndef POSTPROGRESS_H
#define POSTPROGRESS_H

#include <QDialog>

namespace Ui {
class PostProgress;
}

class PostProgress : public QDialog
{
    Q_OBJECT

public:
    explicit PostProgress(QWidget *parent = 0);
    ~PostProgress();

    void setCount(int count);
    void setPosition(int index);
    void step();

private:
    Ui::PostProgress *ui;
};

#endif // POSTPROGRESS_H
