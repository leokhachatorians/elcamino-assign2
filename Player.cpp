#include <string>
#include "Player.h"
#include "globals.h"
#include "Arena.h"
using namespace std;

Player::Player(Arena *ap, int r, int c) {
    if (ap == nullptr) {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols()) {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const {
    return m_row;
}

int Player::col() const {
    return m_col;
}

int Player::age() const {
    return m_age;
}

string Player::takeComputerChosenTurn() {
    // This function first determines the FOV the player has,
    // and then determines which action results in the least
    // amount of danger.
    //
    // **
    // Note that it looks only 3 cells ahead at any given time,
    // any more and it causes the AI to not really take into
    // consideration the immediate threats.
    // **
    int top = 0, bottom = 0, left = 0, right = 0;
    int top_l = 0, top_r = 0, bot_l = 0, bot_r = 0;
    int total_above, total_below, total_right, total_left;
    int range_u, range_d, range_l, range_r;

    range_r = m_arena->cols() - m_col - 1;
    range_r = (range_r > 3) ? 3 : range_r;

    range_l = m_col;
    range_l = (range_l > 3) ? 3 : range_l;

    range_d = m_arena->rows() - m_row - 1;
    range_d = (range_d > 3) ? 3 : range_d;

    range_u = m_row;
    range_u = (range_u > 3) ? 3 : range_u;

    for (int r = m_row - range_u; r < m_row + range_d; r++) {
        for (int c = m_col - range_l; c < m_col + range_r; c++) {
            if (m_arena->nRobotsAt(r, c) > 0) {
                if (r < m_row && c == m_col)
                    top++;
                else if (r < m_row && c < m_col)
                    top_l++;
                else if (r < m_row && c > m_col)
                    top_r++;
                else if (r > m_row && c == m_col)
                    bottom++;
                else if (r > m_row && c < m_col)
                    bot_l++;
                else if (r > m_row && c > m_col)
                    bot_r++;
                else if (r == m_row && c < m_col)
                    left++;
                else
                    right++;
            }
        }
    }

    total_above = top + top_l + top_r;
    total_left = left + top_l + bot_l;
    total_below = bottom + bot_l + bot_r;
    total_right = right + top_r + bot_r;

   if (
        total_above <= total_left &&
        total_above <= total_below &&
        total_above <= total_right) {
        move(0);
        return "Moved Up";
    }
    else if (
        total_below <= total_above &&
        total_below <= total_left &&
        total_below <= total_right) {
        move(1);
        return "Moved Down";
    }
    else if (
        total_left <= total_right &&
        total_left <= total_above &&
        total_left <= total_below) {
        move(2);
        return "Moved Left";
    }
    else if (
        total_right <= total_left &&
        total_right <= total_above &&
        total_right <= total_below) {
        move(3);
        return "Moved Right";
    }
    else {
        stand();
        return "Stood";
    }
}

void Player::stand() {
    m_age++;
}

void Player::move(int dir) {
    m_age++;
    switch (dir)
    {
      case UP:
          if (m_row > 0) {
              m_row--;
          }
        break;
      case DOWN:
        if (m_row < m_arena->rows() - 1) {
            m_row++;
        }
        break;
      case LEFT:
        if (m_col > 0) {
            m_col--;
        }
        break;
      case RIGHT:
        if (m_col < m_arena->cols() - 1) {
            m_col++;
        }
        break;
    }
}

bool Player::shoot(int dir) {
    m_age++;
    if (rand() % 3 == 0) {
        return false;
    }
    int steps = 0;
    while (steps < 5) {
        switch (dir) {
            case 0:
                if (m_arena->nRobotsAt(m_row - steps, m_col) > 0) {
                    m_arena->damageRobotAt(m_row - steps, m_col);
                }
                steps++;
                break;
            case 1:
                if (m_arena->nRobotsAt(m_row + steps, m_col) > 0) {
                    m_arena->damageRobotAt(m_row + steps, m_col);
                }
                steps++;
                break;
            case 2:
                if (m_arena->nRobotsAt(m_row, m_col - steps) > 0) {
                    m_arena->damageRobotAt(m_row, m_col - steps);
                }
                steps++;
                break;
            case 3:
                if (m_arena->nRobotsAt(m_row, m_col + steps) > 0) {
                    m_arena->damageRobotAt(m_row, m_col + steps);
                }
                steps++;
                break;
            default:
                break;
        }
    }
    return true;
}

bool Player::isDead() const {
    return m_dead;
}

void Player::setDead() {
    m_dead = true;
}
