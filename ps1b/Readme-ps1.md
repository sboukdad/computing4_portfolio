# PS1: SpriteBuilder

## Contact
Name: Salma Boukdad

Section: 203

Time to Complete: 25 hours


## Description
Explain what the project does.

Ps1a sets the scene for the ps1b, where we define the sb namespace with our TextureView and SpriteSheet classes. TextureView is in charge of creating a sub view of the texture in view using dimension calculations, while SpriteSheet actually 
handles the logic of which view to select in particular. This all comes together
allowing the program to accurately select tiles and use functions like crop with ease.

### Features
Describe the representation you used and why you did things that way as well as any other interesting design decisions you made.

#### Part a

SpriteSheet class: 
- stores a shared pointer to texture obj to manage texture data
- handles tile dimensions and margin size
- 

TextureView class: 
- holds a shared pointer to texture that references the same texture as parent
    SpriteSheet
- stores position (x, y) and dimensions (w, h) defining rectangular region
    within the texture that this view represents

Design Decisions
-----------------
Shared ownership with the shared pointer: I used a shared pointer to a texture
object to allow multiple TextureView objects to safely reference the same
texture without copying it. When a SpriteSheet creates a TextureView via index
operator, each view shared owernship of the texture. The same goes for crop() when
it creates a new view; it shares the original texture 

Benefits: 
- avoids texture duplication
- automatic memory management - texture is only destroyed when all SpriteSheets
    and TextureViews referencing it are gone
- views can outlive the original SpriteSheet

Grid Dimensions: 
In my constructors, I calculated the grid layout -> 
    _width = (textureSize.x - margin) / (tileSize.x + margin);
The formula accounts for margins by subtracting the initial margin offset and 
diviing the effective tile spacing. This helps improve performance for repeated indexing operations

Row-Major Indexing:
The linear indexing operator uses row-major ordering, converting linear indices
to 2D grid coordinates:
    unsigned int col = i % _width;
    unsigned int row = i / _width;
Matching the natural left-to-right, top-to-bottom reading order for SpriteSheets

Margin Handling:
Tile positions are calculated as -> (tileSize + margin) * index,
assuming the sprite sheet starts at (0, 0) with margins apearing only between
tiles, not as a border around the entire sheet

#### Part b

### Issues
What doesn't work.  Be honest.  You might be penalized if you claim something works and it doesn't.

It all works however I struggled the most with my TestMarginLogic unit test. The
margin test was challenging because my approach for it was not broad enough to
pass the implementations it went up against. 
Main issues:
- pixel placement errors: I initially placed test pixels at wrong positions
- color comparison failures: the BOOST_CHECK_EQUAL with color objects failed
    compile because color does not have operator<< so I used color.toInteger() 
    instead
- Over-specific assumptions: Testing exact pixel values assumed implementation
    details that didn't match the reference (like whether there's a leading 
    margin before the first tile).
Solution:
 I switched over to a comparative approach by testing that margins change tile
 extractions rather than testing specific positions. The final test creates two
 SpriteSheets (with and without margins) from the same image and verifies they 
 extract different tiles, proving margins work without assuming exact positions.

### Tests
Describe what is being tested in your Boost unit tests.

Unit Tests:
My boost tests verify
- Indexing: linear and grid indexing return the same tile, and row-major
    ordering works correctly
- Image conversion: toImage() returns correct tile dimensions
- Cropping: crop() creates properly sized sub-regions and throws exceptions for
    invalid crop areas
- Error handling: out of bounds indices throw out_of_range
- Margins: Tiles are extracted from correct positions where margins are
    specified

### Extra Credit
Anything special you did. This is required to earn bonus points.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.

operator overload instruction video: https://youtu.be/9tHu4mWtrnM?si=qMoo147xrYqeTHxZ

size_t reference sheet - https://en.cppreference.com/w/c/types/size_t.html#:~:text=size_t%20is%20commonly%20used%20for,on%2032%2Dbit%20modular%20arithmetic.



### Credits
List where you got any images or other resources.

Roguelike Characters Pack and playing cards from the Boardgame Pack by Kenney (www.kenney.nl) licensed under the CC0 license and edited by Dr Daly.
