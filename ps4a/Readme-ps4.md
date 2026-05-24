# PS4: AniPlayer

## Contact
Name: Salma Boukdad
Section: 203

Partner Name: Jeannine Al Attal
Partner Section: 203

Time to Complete: 7 days

## Description
Explain what the project does.

This project is an animation player that loads a static scene from a JSON file
and applies motion through tweening between keyframes. The application uses
linear interpolation to calculate intermediate positions, rotations, and
other properties to create fluid movement over a 15-45 second duration.

### Features
Describe what your major decisions were and why you did things that way.

Part A
-------
The main design decision was using the Composite pattern for the component hierarchy.
This made it so that CompositeComponent and all the leaf components could be
treated the same way through the base Component class. The fromJson() factory
method was used to keep all the type-dispatching logic in one place so
that neither the AniPlayer nor the CompositeComponent needed to know about 
specific subclasses.

#### Part a
- JSON Parsing: The AniPlayer constructor opens the input file and uses the nlohmann::json
library to extract window dimensions and initialize the root component of the scene.

- Factory Pattern: We implemented a static Component::fromJson factory method. This
allows the program to instantiate the correct subclass (Circle, Rectangle, etc)
at runtime based on the "shape" field in the JSON without the caller needing to
know the specific types.

- Asset Management: Image textures and fonts are loaded during the construction
of their respective components to prevent performance lag during the rendering loop.

#### Part b

### Memory
Describe how you decided to store the component data including whether you used smart pointers.

- Smart Pointers: We used std::unique_ptr<Component> for all scene components.
This ensures that when AniPlayer or a CompositeComponent is destroyed, the entire
tree of children is automatically and safely deallocated, preventing memory leaks.

- Hierarchal Storage: CompositeComponent stores its children in a
std::vector<std::unique_ptr<Component>>, which facilitates the recursive calling
of draw() and tween() methods across the tree.

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

- Integer Type Safety: During development, we encountered linting errors when
using unsigned long for hex color parsing. We were able to resolve this by utilizing
uint64_t for the intermediate hex value and std::uint8_t for individual RGBA color channels.

- Interpolation Edge Cases: We had to account for "jump cuts" where two keyframes
share the same timestamp (t). We added a check in KeyFrame::tween to handle
potential divisioin by zero by defaulting the interpolation ration to 0.0 if
the time range between frames is zero.

### Extra Credit
Anything special you did.  This is required to earn bonus points.

- Composite Hierarchy: Our implementation fully supports CompositeComponent,
allowing for complex scenes where a parent's movement (translation, rotation, or scale)
is inherited by all of its children.

## Pair Programming
How did working with a partner affect your work?

- Working as a pair was essential for debugging the recursive draw calls. We often
switched roles, with one partner focused on the mathematical logic of the KeyFrame 
interpolation and the other ensuring the JSON parsing and factory patterns were 
robustly implemented.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.

nlohmann/json: For the JSON parsing framework

SFML documentation for guidance on tranform and drawable: https://www.sfml-dev.org/tutorials/3.0/ 

Kenney Shape Characters (CC0): https://kenney.nl/assets/shape-characters

"Morning Mood" By Edvard Grieg, performed by Musopen Symphony Orchestra (CC-PDM): https://en.wikipedia.org/wiki/Morning_Mood

Roboto font by Christian Robertson (SIL Open Font License): https://fonts.google.com/specimen/Roboto/license
