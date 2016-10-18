#include "Arena.h"
#include "Player.h"
#include "globals.h"
#include <string>
using namespace std;


Arena::Arena(int nRows, int nCols) {
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS) {
        cout << "***** Arena created with invalid size " << nRows << " by "  
             << nCols << "!" << endl; 
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena() {
    delete m_player;
    for (int i = 0; i < m_nRobots; i++) {
        delete m_robots[i];
    }
}

int Arena::rows() const {
    return m_rows;
}

int Arena::cols() const {
    return m_cols;
}

Player* Arena::player() const {
    return m_player;
}

int Arena::robotCount() const {
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const {
    int count = 0;
    for(int i = 0; i < m_nRobots; i++) {
        if (m_robots[i]->row() == r && m_robots[i]->col() == c) {
            count++;
        }
    }
    return count;
}

void Arena::display(string msg) const {
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';
       

    for(int i = 0; i < m_nRobots; i++) {
        int row = m_robots[i]->row();
        int col = m_robots[i]->col();
        int robot_count = nRobotsAt(row, col);
        switch (robot_count) {
            case 1:
                grid[row][col] = 'R'; break;
            case 2:
                grid[row][col] = '2'; break;
            case 3:
                grid[row][col] = '3'; break;
            case 4:
                grid[row][col] = '4'; break;
            case 5:
                grid[row][col] = '5'; break;
            case 6:
                grid[row][col] = '6'; break;
            case 7:
                grid[row][col] = '7'; break;
            case 8:
                grid[row][col] = '8'; break;
            case 9:
                grid[row][col] = '9'; break;
            default:
                grid[row][col] = '9'; break;
        }
    }

        // Indicate player's position
    if (m_player != nullptr) {
          // Set the char to '@', unless there's also a robot there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()][m_player->col()];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++) {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c) {
      // If MAXROBOTS have already been added, return false.  Otherwise,
      // dynamically allocate a new robot at coordinates (r,c).  Save the
      // pointer to the newly allocated robot and return true.
      if (m_nRobots == MAXROBOTS) {
          return false;
      }
      m_robots[m_nRobots++] = new Robot(this, r, c);
      return true;
}

bool Arena::addPlayer(int r, int c) {
      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::damageRobotAt(int r, int c) {
    for (int i = 0; i < m_nRobots; i++) {
        if (m_robots[i]->row() == r && m_robots[i]->col() == c) {
            bool alive = m_robots[i]->takeDamageAndLive();
            if (alive) {
                break;
            }
            else {                
                // stackoverflow.com/questions/33514362/
                // removing-elements-from-an-array-of-pointers-c
                delete m_robots[i];
                m_robots[i] = m_robots[m_nRobots - 1];
                m_robots[m_nRobots - 1] = NULL;
                m_nRobots--;
                break;
            }
        }
    }
}

bool Arena::moveRobots() {
   for (int i = 0; i < m_nRobots; i++) {
       m_robots[i]->move();
       if (m_robots[i]->row() == m_player->row() && m_robots[i]->col() == m_player->col()) {
           m_player->setDead();
       }
   }
    return m_player->isDead();
}
