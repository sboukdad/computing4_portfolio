# PS3: Sokoban

## Contact
Name: Salma Boukdad
Section: 203
Time to Complete part a: 2 days
Time to Complete part b: 4 days


## Description
Explain what the project does.

ps3a
-----
Ps3a implements a Sokoban game UI using c++ and SFML. The program loads a level
from a file, displays the game board with the appropriate tiles, and renders
the player character on top of the grid.

ps3b
-----
Ps3b implements the Sokoban gameplay mechanics. The program handles keyboard movement
using arrow keys as well as W,S,A,D. Implementation handles box movement, a move
tracker, as well as the ability to undo, redo, and reset the level.


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
The majority of the game mechanic logic is handled by the movePlayer function.
Utilizing grid coodination logic: up/down altering y, left/right altering x
I am able to control the player's movements by changing the respective axis when
necessary. The player has the ability to redo and undo their moves, as well as
the ability to reset the level to its initial state, allowing the player to play
the level again. The move tracker on the top left corner tracks the number of moves
made by the player and is reflected when the player undoes or redoes a move.
The player has the ability to move stored boxes off the storage
location if desired. An unstored box is colored red, while a stored box is signaled
when it turns green. 

### Memory
Describe how you decided to store the level data including whether you used smart pointers.

Level data was stored in a std::vector of type char using row-major order with the 
index formula given (i = x + y * width). The spriteSheet manages the texture internally via
the shared pointer, avoiding manual memory management.

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.

None for part a.

Part b
-------
I use a lambda expression as a parameter to std::count_if in isWon() to check for unstored boxes in the grid 

std::count_if(gridState.begin(), gridState.end(), [](char c) { return c == 'A'; }) == 0;

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

The main challenges were ordering the row/col coordinates when mapping the tiles 
from the tilesheet, and ensuring the player character was stored separately from
the grid so "." was stored at the player's position instead of '@'

I did not implement the victory setting to allow for a win when there are more boxes than storage locations.

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

Lambda Help: https://www.w3schools.com/cpp/cpp_functions_lambda.asp

Algorithm function pick: https://cplusplus.com/reference/algorithm/
