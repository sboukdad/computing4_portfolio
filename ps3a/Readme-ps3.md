# PS3: Sokoban

## Contact
Name: Salma Boukdad
Section: 203
Time to Complete: 2 days


## Description
Explain what the project does.

ps3a
-----
Ps3a implements a Sokoban game UI using c++ and SFML. The program loads a level
from a file, displays the game board with the appropriate tiles, and renders
the player character on top of the grid.

### Features
Describe what your major decisions were and why you did things that way.

#### Part a
One of my major design desicions include using a 1D vector in row-major order to
store the grid state, tracking the player position separately from the grid, and 
using a SpriteSheet class from ps1 to manage the tile extraction from the tilesheet.
I also used maps to store both the direction and the gameboard tiles to the corresponding
coordinates for its element in the tileSheet. This kept the draw logic a lot cleaner
and much more readable.

#### Part b

### Memory
Describe how you decided to store the level data including whether you used smart pointers.

Level data was stored in a std::vector of type char using row-major order with the 
index formula given (i = x + y * width). The spriteSheet manages the texture internally via
the shared pointer, avoiding manual memory management.

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.

None for part a.

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

The main challenges were ordering the row/col coordinates when mapping the tiles 
from the tilesheet, and ensuring the player character was stored separately from
the gird so "." was stored at the player's position instead of '@'

### Extra Credit
Anything special you did.  This is required to earn bonus points.

- undo():
 implemented using a stack of gameState (a pair of the gridState vector<char>
 and the player's coordinates using sf::Vector2u) that saves the gridState 
 player location before each move, allowying the player to step back and forward
 through previous states

- redo():
 implemented using a second stack that saves undone states, allowing the player
 to reapply moves that were undone

- moveCount:
 tracks the number of moves made using moveCount, displayed on screen so 
 the player can see how many moves they've takes to solve the puzzle.


## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.

Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban
