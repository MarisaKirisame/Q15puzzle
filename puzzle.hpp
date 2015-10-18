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

std::vector< Action > & all_action( )
{
    static std::vector< Action > act = { { Action::up, Action::down, Action::left, Action::right } };
    return act;
}

puzzle act( const puzzle & state, Action a ) {
    auto i = std::find(state.begin( ), state.end( ), 0) - state.begin( );
    assert( i < 16 );
    auto act = [&]( bool is_valid, size_t swap_with )
    {
        if ( is_valid )
        {
            puzzle ret = state;
            std::swap( ret[i], ret[swap_with] );
            return ret;
        }
        else { return state; }
    };
    switch ( a )
    {
        case Action::up : return act( i > 3, i - 4 );
        case Action::down : return act( i < 12, i + 4 );
        case Action::left : return act( i % 4 > 0, i - 1 );
        case Action::right : return act( i % 4 < 3, i + 1 );
    }
}
constexpr puzzle goal_state = { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 } };

Action random_action( ) {
    switch (std::uniform_int_distribution<int> (1, 4)(rd)) {
        case 1 : return Action::up;
        case 2 : return Action::down;
        case 3 : return Action::left;
        default : return Action::right;
    }
}

puzzle random_puzzle( ) {
    std::vector<Action> moves;
    for (int i = 0; i < 152; i++) {
        moves.push_back( random_action() );
    }
    return std::accumulate(moves.begin(), moves.end(), goal_state, act);
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
#endif // PUZZLE_HPP

