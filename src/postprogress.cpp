#include "postprogress.h"
#include "ui_postprogress.h"

PostProgress::PostProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PostProgress)
{
#if defined(Q_OS_MAC)
    setWindowFlags(Qt::Sheet);
#endif

    ui->setupUi(this);

    adjustSize();
}

PostProgress::~PostProgress()
{
    delete ui;
}

void PostProgress::setCount(int count)
{
    ui->postProgress->setMinimum(0);
    ui->postProgress->setMaximum(count);
    ui->postProgress->setValue(0);
}

void PostProgress::setPosition(int index)
{
    ui->postProgress->setValue(index);
}

void PostProgress::step()
{
    ui->postProgress->setValue(ui->postProgress->value() + 1);

}
