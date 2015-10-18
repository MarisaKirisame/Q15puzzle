#ifndef QNUMBER_HPP
#define QNUMBER_HPP

#include <QWidget>
#include <array>
class QNumber : public QWidget
{
    Q_OBJECT
public:
    explicit QNumber(QWidget *parent = 0);
    size_t pos;
    std::array< size_t, 16 > * arr;
    void paintEvent(QPaintEvent *);
signals:

public slots:
};

#endif // QNUMBER_HPP
