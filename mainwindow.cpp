#include "mainwindow.hpp"
#include "puzzle.hpp"
#include "../MO/mo.hpp"
using namespace MO;
auto learner =
        []( const puzzle & state, size_t i, const auto & change_to )
        {
            vector< Action > ret;
            breadth_first_search< Action >(
                state,
                []( const auto &, auto out ) { return std::copy( all_action( ).begin( ), all_action( ).end( ), out ); },
                act,
                [&]( const auto & st )
                {
                    return
                        all_of(
                            make_counting_iterator( static_cast< size_t >( 0 ) ),
                            make_counting_iterator( i ),
                            [&]( size_t i ){ return state[i] == st[i]; } ) && st[i] == change_to;
                },
                back_inserter( ret ) );
            return ret;
        };
auto lambda_act = []( const puzzle & p, Action a ) { return act( p, a ); };
struct impl
{
    MO< size_t, 16, Action, decltype( lambda_act ), decltype( learner ) > mo = make_MO( goal_state, all_action( ), lambda_act, learner );
    puzzle p;
    explicit impl( const puzzle & p ) : p( p ) { }
};
MainWindow::~MainWindow( ) { delete ui; delete p_impl; }
MainWindow::MainWindow( QWidget *parent ) : QMainWindow(parent), p_impl( new impl( random_puzzle( ) ) ), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#define MACRO( NUM ) ui->Num ## NUM ->num = &p_impl->p[NUM]
    MACRO( 0 ); MACRO( 1 ); MACRO( 2 ); MACRO( 3 );
    MACRO( 4 ); MACRO( 5 ); MACRO( 6 ); MACRO( 7 );
    MACRO( 8 ); MACRO( 9 ); MACRO( 10 ); MACRO( 11 );
    MACRO( 12 ); MACRO( 13 ); MACRO( 14 ); MACRO( 15 );
}

void MainWindow::on_pushButton_3_clicked()
{
    std::vector< std::vector< Action > > res;
    p_impl->mo.solve( p_impl->p, std::back_inserter( res ) );
    std::vector< Action > act;
    flatten( res.begin( ), res.end( ), std::back_inserter( act ) );
    p_impl->p = p_impl->mo.apply_macro( p_impl->p, act.begin( ), act.end( ) );
    repaint( );
}

void MainWindow::on_pushButton_4_clicked()
{
    std::vector< std::vector< Action > > res;
    p_impl->mo.solve( p_impl->p, std::back_inserter( res ) );
    auto it = std::find_if_not( res.begin(), res.end(), []( const auto& t ){ return t.empty(); });
    if ( it != res.end() ) {
        p_impl->p = p_impl->mo.apply_macro( p_impl->p, it->begin( ), it->end( ) );
        repaint();
    }
}

void MainWindow::on_pushButton_clicked()
{
    p_impl->p =  random_puzzle();
    repaint();
}
