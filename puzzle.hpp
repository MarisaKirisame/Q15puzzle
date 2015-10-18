#ifndef PUZZLE_HPP
#define PUZZLE_HPP
#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>
typedef std::array< size_t, 16 > puzzle;
std::random_device rd;

enum class Action { up, down, left, right };

template< typename OUTITER >
OUTITER all_action( const puzzle &, OUTITER out )
{
    * out = Action::up; ++out;
    * out = Action::down; ++out;
    * out = Action::left; ++out;
    * out = Action::right; ++out;
    return out;
}

template< typename OS >
OS & operator << ( OS & os, Action act ) {
    return os << ( act == Action::up ? 'U' : act == Action::down ? 'D' : act == Action::left ? 'L' : 'R' );
}

template< typename OS >
OS & operator << ( OS & os, const puzzle & state ) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << state[i * 4 + j] << " ";
        }
        os << std::endl;
    }
    return os;
}

puzzle act( const puzzle & state, Action a ) {
    auto i = std::find( state.begin( ), state.end( ), 0 ) - state.begin( );
    assert( i < 16 );
    puzzle ret = state;
    std::swap(
        ret[i],
        ret[a == Action::up ? ( i > 3 ? i - 4 : i ) :
            a == Action::down ? ( i < 12 ? i + 4 : i ) :
            a == Action::left ? ( i % 4 > 0 ? i - 1 : i ) :
                                ( i % 4 < 3 ? i + 1 : i )] );
    return ret;
}

constexpr puzzle goal_state = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } };

puzzle random_move( const puzzle & p ) {
    std::vector< Action > acts;
    all_action( p, std::back_inserter( acts ) );
    return act( p, acts[ std::uniform_int_distribution< int >( 0, acts.size( ) - 1 )( rd ) ] );
}

puzzle random_puzzle( ) {
    auto ret = goal_state;
    for (int i = 0; i < 152; i++) {
        ret = random_move( ret );
    }
    return ret;
}
#endif // PUZZLE_HPP

