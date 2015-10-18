#include "mainwindow.hpp"
#include "puzzle.hpp"
#include "../MO/mo.hpp"
#include "../cpp_common/combinator.hpp"
#include <chrono>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>

using namespace MO;
auto lambda_all_action = []( const puzzle & p, auto out ) { return all_action( p, out ); };
auto learner =
        []( const puzzle & state, const array< size_t, 16 > & orientation, size_t i, const auto & change_to )
        {
            for (size_t depth = 0; true; ++depth)
            {
                auto res = PARTIAL_MATCH< Action >(
                            state,
                            orientation,
                            i,
                            change_to,
                            lambda_all_action,
                            act,
                            []( Action a )
                            {
                                return
                                    a == Action::up ? Action::down :
                                    a == Action::down ? Action::up :
                                    a == Action::left ? Action::right :
                                                        Action::left;
                            },
                            depth );
                if ( res ) { return * res; }
            }
        };
auto lambda_act = []( const puzzle & p, Action a ) { return act( p, a ); };
struct impl
{
    MO< size_t, 16, Action, decltype( lambda_all_action ), decltype( lambda_act ), decltype( learner ) > mo =
            make_MO< Action >( goal_state, { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } }, lambda_all_action, lambda_act, learner );
    puzzle p;
    explicit impl( const puzzle & p ) : p( p ) { }
};
MainWindow::~MainWindow( ) { delete ui; delete p_impl; }
MainWindow::MainWindow( QWidget *parent ) : QMainWindow(parent), p_impl( new impl( random_puzzle( ) ) ), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#define MACRO( NUM ) ui->Num ## NUM ->arr = & p_impl->p; ui->Num ## NUM ->pos = NUM;
    MACRO( 0 ); MACRO( 1 ); MACRO( 2 ); MACRO( 3 );
    MACRO( 4 ); MACRO( 5 ); MACRO( 6 ); MACRO( 7 );
    MACRO( 8 ); MACRO( 9 ); MACRO( 10 ); MACRO( 11 );
    MACRO( 12 ); MACRO( 13 ); MACRO( 14 ); MACRO( 15 );
}

string vector_to_string(std::vector<Action> v) {
    string ret = "";
    for (auto i : v) {
        switch (i) {
            case Action::up : ret += "up\n";
            case Action::down : ret += "down\n";
            case Action::left : ret += "left\n";
            default : ret += "right\n";
        }
    }
    return ret;
}

void MainWindow::on_pushButton_3_clicked()
{
    std::vector< std::vector< Action > > res;
    p_impl->mo.solve( p_impl->p, std::back_inserter( res ) );
    std::vector< Action > act;
    flatten( res.begin( ), res.end( ), std::back_inserter( act ) );

    QMessageBox::about( this, tr("test"), tr(vector_to_string(act).c_str()));
}

void MainWindow::on_pushButton_4_clicked()
{
    std::vector< Action > res;
    p_impl->mo.solve_step( p_impl->p, std::back_inserter( res ) );

    QMessageBox::about( this, tr("test"), tr(vector_to_string(res).c_str()));
}

void MainWindow::on_pushButton_clicked()
{
    p_impl->p =  random_puzzle();
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent * ev) {
    switch ( ev->key() ) {
        case Qt::Key_W :
            p_impl->p = act(p_impl->p, Action::up);
            repaint();
            break;
        case Qt::Key_S :
            p_impl->p = act(p_impl->p, Action::down);
            repaint();
            break;
        case Qt::Key_A :
            p_impl->p = act(p_impl->p, Action::left);
            repaint();
            break;
        case Qt::Key_D :
            p_impl->p = act(p_impl->p, Action::right);
            repaint();
            break;
        default: break;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    bool ok;
    QString str = QInputDialog::getText( this,
                tr("swap"), tr("enter two values two swap"),
                QLineEdit::Normal, QString::null, &ok);

    if (ok && !str.isEmpty()) {
        int x = str.split(" ")[0].toInt();
        int y = str.split(" ")[1].toInt();
        p_impl->p = swapTwo(p_impl->p, x, y);
        repaint();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about( this, tr("test"), tr(std::to_string(p_impl->mo.table.size()).c_str()));
}
