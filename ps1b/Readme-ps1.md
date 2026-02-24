# PS1: SpriteBuilder

## Contact
Name: Salma Boukdad

Section: 203

Time to Complete:
(part a) 25 hours 
(part b) 14 hours


## Description
Explain what the project does.

Ps1a sets the scene for the ps1b, where we define the sb namespace with our TextureView and SpriteSheet classes. TextureView is in charge of creating a sub view of the texture in view using dimension calculations, while SpriteSheet actually 
handles the logic of which view to select in particular. This all comes together
allowing the program to accurately select tiles and use functions like crop with ease.

Ps1b builds on ps1a by implementing a SpriteBuilder that creates composite character sprites by randomly selecting and layering tiles from multiple sprite sheets. The program reads a configuration file specifying tile dimensions and sprite sheet paths, then generates unique characters by combining randomly selected parts (body, head, clothing, etc.). Users can interact with the program through a GUI window to randomize characters and save them as PNG images.

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

SpriteBuilder Class:
- Stores a vector of spriteSheet objects, one for each layer (body, head, clothing)
- maintains parallel vectors of sf::Sprite and sf::Texture obj to render the current selection
- uses std::mt19937 (mersenne twister) rand num generator for reproducible randomization
- stores tile size for window creation

Random part selection: 
I used std::uniform_int_distribution to select random tile indices from each sprite sheet. 
The distribution range is [0, shee.length()-1] to coverall available tiles. Each call to randomize() generates a new random index for every spriteSheet, creating a unique combination of all parts. 

Design Desicions: 
-------------------
- texture persistence: I store textures in a separate vector because sf::Sprite holds a pointer
to its texture. Without persistent storage, the texture would be destroyed when it goes out of scope, causing rendering issues. 
- Double randomize in constructor: I have the constructor with seed call randomize() twice to ensure
the texures are properly initialized. This works around a texture loading timing issue in SFML where the first generation sometimes produce artifacts. 
- Seeded RNG: By accepting an optional seed parameter, the same seed always produces the same sequence of sprites, enabling reproducible results for testing and debugging. 

Main program fts:
-------------------
- command-line argument parsing for --file, --scale, and --seed parameters
- window display with configuration scaling and 50-pix padding border
- keyboard controls: R to randomize, S to save with auto-increasing filenames
- saved imgs use unscaled sprites with transparent backgrounds

### Issues
What doesn't work.  Be honest.  You might be penalized if you claim something works and it doesn't.

(ps1a)
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

(ps1b)
The initial implementation had a white background issue on the first generated 
sprite, which was resolved by calling randomize() twice in the constructor. All 
tests pass and both the test exe and main spriteBuilder program function properly

### Tests
Describe what is being tested in your Boost unit tests.

Unit Tests:
(ps1a)
My boost tests verify
- Indexing: linear and grid indexing return the same tile, and row-major
    ordering works correctly
- Image conversion: toImage() returns correct tile dimensions
- Cropping: crop() creates properly sized sub-regions and throws exceptions for
    invalid crop areas
- Error handling: out of bounds indices throw out_of_range
- Margins: Tiles are extracted from correct positions where margins are
    specified

(ps1b)
- fixed seed consistency: two spriteBuilders created with the same seed 
will produce the same picture for the first sprite, ensuring reproducability.
- Different seeds generate different images: three builders with different seeds 
will produce at least one pair of different sprites verifying randomization.
- Image stability: drawing the same spriteBuilder multiple times without calling
randomize() produces identical images, ensuring the sprite doesn't change unexpectedly.
- Seed sequence consistency: two builders with the same seed produce identical second sprites
after calling randomize() once, verifying the entire random sequence is reproducible.


### Extra Credit
Anything special you did. This is required to earn bonus points.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.

operator overload instruction video: https://youtu.be/9tHu4mWtrnM?si=qMoo147xrYqeTHxZ

size_t reference sheet - https://en.cppreference.com/w/c/types/size_t.html#:~:text=size_t%20is%20commonly%20used%20for,on%2032%2Dbit%20modular%20arithmetic.

SFML 3.0 documentation - https://www.sfml-dev.org/documentation/3.0.0/

C++ random number generation - https://en.cppreference.com/w/cpp/numeric/random

### Credits
List where you got any images or other resources.

Roguelike Characters Pack and playing cards from the Boardgame Pack by Kenney (www.kenney.nl) licensed under the CC0 license and edited by Dr Daly.
