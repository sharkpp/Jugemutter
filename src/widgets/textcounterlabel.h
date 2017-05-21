#ifndef TEXTCOUNTERLABEL_H
#define TEXTCOUNTERLABEL_H

#include <QLabel>
#include <QColor>

class TextCounterLabel : public QLabel
{
    Q_OBJECT

public:
    TextCounterLabel(QWidget *parent = Q_NULLPTR);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_buddy_textChanged();

private:
    int warningThreshold;
    QColor normalColor;
    QColor warningColor;
};

#endif // TEXTCOUNTERLABEL_H
