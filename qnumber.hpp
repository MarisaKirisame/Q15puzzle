#ifndef QNUMBER_HPP
#define QNUMBER_HPP

#include <QWidget>

class QNumber : public QWidget
{
    Q_OBJECT
public:
    explicit QNumber(QWidget *parent = 0);
    size_t * num;
    void paintEvent(QPaintEvent *);
signals:

public slots:
};

#endif // QNUMBER_HPP
