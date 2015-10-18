#include "qnumber.hpp"
#include <QPainter>
#include <puzzle.hpp>
QNumber::QNumber(QWidget *parent) : QWidget(parent)
{

}

void QNumber::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QFont f = font( );
    f.setPixelSize( 50 );
    p.setFont( f );
    p.setPen( Qt::white );
    p.fillRect( rect( ), QBrush( QColor( 128, 64, 0 ) ) );
    p.drawText( rect( ), Qt::AlignCenter, QString::fromStdString( std::to_string( position_value(*arr, pos) ) ) );
}
