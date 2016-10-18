#include "Robot.h"
#include "Arena.h"
#include "globals.h"
using namespace std;

Robot::Robot(Arena* ap, int r, int c) {
    if (ap == nullptr) {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 0  ||  r >= ap->rows()  ||  c < 0  ||  c >= ap->cols()) {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = 2;
}

int Robot::row() const {
    return m_row;
}

int Robot::col() const {
    return m_col;
}

void Robot::move() {
    switch (rand() % 4)
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

bool Robot::takeDamageAndLive() {
    if (m_health == 2) {
        m_health--;
        return true;
    }
    return false;
}