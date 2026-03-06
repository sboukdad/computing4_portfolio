# PS2: Pythagorean Tree

## Contact
Name: Salma Boukdad
Section: 203
Time to Complete: 3 days


## Description
Explain what the project does.

This project implements a fractal Pythagorean Tree using c++ and SFML. The program
recursively generates a tree-like structure where each square has two child squares
positioned on its top edge, forming right triangles that demonstrate the
Pythagorean theorem. The tree creates a really beautiful branching pattern that
resembles a natural tree, with each generation of squares being smaller than the previous
by the factor of 1 / root 2

The program takes two command-line arguments:
- L: side length base square
- N: the depth of recursion (how many generations to draw)

Example: ./PTree 100 8

### Features
Describe what your major decisions were and why you did things that way.

The core of the implementation uses recursion to build the tree. Each square spawns
two child squares of 45 angles, creating the characteristics fractal pattern. The recursion
naturally handles the decreasing size and increasing complexity at each level. 

Positioning algorithm
-----------------------
The children are positioned using a top-center approach rather than corner-based positionting:
1. find the parent square's top-center point by moving "up" (perpendicular to the base) by half its size
2. for each child, calculate its own "up" direction based on its rotation
3. position the child's center by moving from the parent's top-center along the child's "up" vector by the child's full size

This approach works regardless of the parent's rotation

Child Size Calculation
-----------------------
For a 45 deg. angle between children, both child squares have the same size: parent_size / root(2). This comes from the Pythagorean theorem - if two squares of equal size form the legs of a right triangle, the hypotenuse relates to the legs by the equation: l^2 * l^2 = hypotenuse^2, 
which simplifies to leg = hypotenuse / root(2)

Color Scheme
------------
The tree uses a sakura (cherry blossom) gradient:
- base square (iteration 1): dark brown trunk (RGB: 101, 67, 33)
- iterations 2+: gradient from reddish-brown -> deep pink -> light sakura pink
This creates a natural tree appearance with a brown trunk and pink blossoms

The gradient is calculated based on the current depth relative to the max depth, creating a smooth color transitions as the tree branches outward

Window Sizing
--------------
window is automatically sized to 6L * 4L to accommodate the full tree as specified 
in the assignment, ensuring the fractal fits perfectly regardless of the base size chose.

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

Initial geometry challenges
----------------------------
The most significant challenge was determining the correct positioning formual for child squares.
Early attempts used corner-based appoaches where I tried to place each child's corner at the parent's corner, but this led to incorrect positioning at recursion depths beyond 2. The squares
would bunch up toward the center instead of spreading outward.

The breakthrough came from studying a working implementation that used a "top-center plus offset" approach. That method was more robust because it doesn't depent on tracking which specific corner touches which - instead, it positions each child based on its own orientation vector. 

What I learned
---------------
- Importance of choosing the right geometric approach, sometimes the most intuitive method (corner-tocoern) isnt the most robust
- How roation transfomations work in 2d graphics
- The value of simplifying variable names and reducing code complexity for maintainability
- Practical applications of the Pythagorean theorem in fractal geometry 

### Extra Credit
Anything special you did.  This is required to earn bonus points.

Sakura Tree Coloring: implemented a griadent color scheme that transitions from dark brown
at the trunk to light pink at the outer branches, creating a really nice cherry blossom tree aesthetic. The gradient is carefully calculated to start at iteration 2 and smoothly transition
through multiple pink shades. 


## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.

- SFML 3.0 documentation https://www.sfml-dev.org/
- Assignment PDF for mathematical relationships and tree specifications
- Stack Overflow thread on color interpolation - https://stackoverflow.com/questions/13488957/interpolate-from-one-color-to-another

