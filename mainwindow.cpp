#include "mainwindow.hpp"
#include "puzzle.hpp"
#include "../MO/mo.hpp"
#include "../cpp_common/combinator.hpp"
using namespace MO;
auto iddfs_learner =
        []( const puzzle & state, size_t i, const auto & change_to )
        {
            vector< Action > ret;
            iterative_deepening_depth_first_search< Action >(
                state,
                all_action,
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
        };//THIS WILL NOT WORK ON YOUR MACHINE! IT STILL EXISTS BECAUSE I WANT TO ABSTRACT MORE FROM IT, JUST DONT USE!
template< typename ACTION, typename STATE_VAR, size_t NUM_VAR, typename ALL_ACTION, typename ACT_FUNC, typename INVERSE_FUNC >
boost::optional< std::vector< ACTION > > PARTIAL_MATCH(
        const array< STATE_VAR, NUM_VAR > & st, size_t i, const STATE_VAR & change_to,
        ALL_ACTION all_action, ACT_FUNC act_func, INVERSE_FUNC inverse_func, size_t depth )
{
    size_t l = depth / 2, r = depth / 2 + depth % 2;
    auto powerset = common::fix(
        [&]( const auto & SELF, size_t i )
        {
            if ( i == 0 ) { return vector< pair< array< STATE_VAR, NUM_VAR >, vector< ACTION > > >{ make_pair( st, vector< ACTION >{ } ) }; }
            else
            {
                auto psm1 = SELF( i  - 1 );
                decltype( psm1 ) ret;
                ret.reserve( psm1.size( ) );
                for ( const auto & pse : psm1 )
                {
                    all_action(
                        pse.first,
                        boost::make_function_output_iterator(
                            [&]( const ACTION & act )
                            {
                                auto tem = pse.second;
                                tem.push_back( act );
                                ret.push_back( { act_func( pse.first, act ), tem } );
                            } ) );
                }
                return ret;
            }
        } );
    multimap< vector< STATE_VAR >, vector< Action > > change_invarance_l, change_invarance_r;
    auto psl = powerset( l );
    auto psr = powerset( r );
    auto invariance_state =
            [&]( const array< STATE_VAR, NUM_VAR > & arr )
            {
                std::vector< STATE_VAR > ret;
                copy_n( arr.begin( ), i, back_inserter( ret ) );
                return ret;
            };
    for ( const auto & e : psl )
    { change_invarance_l.insert( { invariance_state( e.first ), e.second } ); }
    for ( const auto & e : psr )
    { change_invarance_r.insert( { invariance_state( e.first ), e.second } ); }
    for ( const auto & el : change_invarance_l )
    {
        auto r = change_invarance_r.equal_range( el.first );
        for ( const auto & er : common::make_range_container_proxy( r.first, r.second ) )
        {
            vector< Action > action1 = el.second, action2 = er.second;
            for ( bool b = true; b; b = ! b )
            {
                auto new_state =
                        accumulate(
                            boost::make_transform_iterator( action2.rbegin( ), inverse_func ),
                            boost::make_transform_iterator( action2.rend( ), inverse_func ),
                            accumulate( action1.begin( ),
                                        action1.end( ),
                                        st,
                                        act_func ),
                            act_func );
                if ( new_state[i] == change_to && invariance_state( new_state ) == invariance_state( st ) )
                {
                    action1.reserve( action1.size( ) + action2.size( ) );
                    copy( boost::make_transform_iterator( action2.rbegin( ), inverse_func ),
                          boost::make_transform_iterator( action2.rend( ), inverse_func ),
                          back_inserter( action1 ) );
                    return action1;
                }
                else { swap( action1, action2 ); }
            }
        }
    }
    return boost::optional< std::vector< ACTION > >( );
}
auto lambda_all_action = []( const puzzle & p, auto out ) { return all_action( p, out ); };
auto learner =
        []( const puzzle & state, size_t i, const auto & change_to )
        {
            for (size_t depth = 0; true; ++depth)
            {
                auto res = PARTIAL_MATCH< Action >(
                            state,
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
            make_MO< Action >( goal_state, lambda_all_action, lambda_act, learner );
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
    std::vector< Action > res;
    p_impl->mo.solve_step( p_impl->p, std::back_inserter( res ) );
    p_impl->p = p_impl->mo.apply_macro( p_impl->p, res.begin( ), res.end( ) );
    repaint();
}

void MainWindow::on_pushButton_clicked()
{
    p_impl->p =  random_puzzle();
    repaint();
}
