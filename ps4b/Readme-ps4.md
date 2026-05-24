# PS4: AniPlayer

## Contact
Name: Salma Boukdad
Section: 203

Partner Name: Jeannine Al Attal
Partner Section: 203

Time to Complete: 7 days

## Description
This project is an animation player that loads a scene from a JSON file
and applies motion through tweening between keyframes. The application uses
linear interpolation to calculate intermediate positions, rotations, and
other properties to create fluid movement. The player supports pause, resume,
and restart controls, and holds the final frame when the animation ends.

### Features

Part A
-------
The main design decision was using the Composite pattern for the component hierarchy.
This made it so that CompositeComponent and all the leaf components could be
treated the same way through the base Component class. The fromJson() factory
method was used to keep all the type-dispatching logic in one place so
that neither the AniPlayer nor the CompositeComponent needed to know about
specific subclasses.

Part B
-------
Tweening was implemented in KeyFrame::tween() using linear interpolation.
The function currFrame() finds the two keyframes that bracket the current time
and calls tween() between them. If the time is before the first keyframe it
clamps to the first frame, and if it's past the last keyframe it locks to the
last keyframe so the animation holds its final position rather than disappearing.

Timekeeping was handled by sf::Clock in main.cpp. The elapsed time is passed to
player.tween() each frame, which propagates through the entire component tree.
Pause and resume work by calling timer.stop() and timer.start(), and restart resets
the clock with timer.reset() followed by timer.start().

#### Part a
- JSON Parsing: The AniPlayer constructor opens the input file and uses the nlohmann::json
  library to extract window dimensions and initialize the root component of the scene.

- Factory Pattern: We implemented a static Component::fromJson factory method. This
  allows the program to instantiate the correct subclass (Circle, Rectangle, etc)
  at runtime based on the "shape" field in the JSON without the caller needing to
  know the specific types.

- Asset Management: Image textures and fonts are loaded during the construction
  of their respective components to prevent performance lag during the rendering loop.

- Named Colors: The Component constructor checks if the color string is a named
  W3C color (e.g. "Red", "Navy") and maps it to its hex equivalent before parsing.
  This is done using an unordered_map lookup before the hex parsing logic.

#### Part b
- Temporal Logic: The Component::tween method propagates the current playback time from
  the AniPlayer down through the entire component tree.

- Linear Interpolation (LERP): To achieve fluid motion, we implemented a lerp helper
  in the KeyFrame class. It calculates the intermediate value between two keyframes based
  on the ratio of elapsed time to the total duration between frames. The interpolation
  ratio is clamped to [0, 1] to prevent extrapolation past keyframe endpoints.

- State Management: The player handles three distinct temporal states: locking to
  the first frame if the time is <= the first keyframe, locking to the last frame if
  the time is >= the final keyframe, or interpolating between two bounding frames if
  the time falls in between.

- Divide by Zero Guard: KeyFrame::tween checks if the time range between two keyframes
  is zero (same timestamp) and returns the start frame immediately to avoid a crash.

#### Extra Credit
- Keyframe Carry Over: When a keyframe omits a field (e.g. no "x" specified), the
  Component constructor now carries over the value from the previous keyframe instead
  of defaulting to 0. This is implemented in the keyframe loading loop in Component.cpp
  by checking which fields are absent in the JSON and patching the new KeyFrame with
  the previous keyframe's values using the setter methods on KeyFrame.

### Memory
- Smart Pointers: We used std::unique_ptr<Component> for all scene components.
  This ensures that when AniPlayer or a CompositeComponent is destroyed, the entire
  tree of children is automatically and safely deallocated, preventing memory leaks.

- Hierarchical Storage: CompositeComponent stores its children in a
  std::vector<std::unique_ptr<Component>>, which facilitates the recursive calling
  of draw() and tween() methods across the tree.

### Issues
- Integer Type Safety: During development, we encountered linting errors when
  using unsigned long for hex color parsing. We resolved this by using uint64_t
  for the intermediate hex value and std::uint8_t for individual RGBA color channels.

- Interpolation Edge Cases: We had to account for "jump cuts" where two keyframes
  share the same timestamp (t). We added a check in KeyFrame::tween to handle
  potential division by zero by returning the start frame immediately if the time
  range between frames is zero.

- Divide by Zero in Boundary Tests: The autograder's reference implementation crashed
  when tweening a frame with itself at the same timestamp. We fixed this by adding an
  early return in KeyFrame::tween when range == 0.0f.

### Extra Credit
- Keyframe Carry Over: When a keyframe is missing a field, the value is inherited
  from the previous keyframe rather than defaulting to 0. This makes JSON files easier
  to write since you only need to specify values that change between keyframes.

- Named Colors: The Component constructor supports W3C named colors (White, Black,
  Red, Green, Blue, Yellow, Cyan, Magenta, Maroon, Lime, Navy, Purple, Teal, Silver,
  Gray, Olive, Fuchsia, Aqua) in addition to hex color codes.

- Composite Hierarchy: Our implementation fully supports CompositeComponent,
  allowing for complex scenes where a parent's movement (translation, rotation, or scale)
  is inherited by all of its children.

## Pair Programming
Working as a pair was essential for debugging the recursive draw calls. We often
switched roles, with one partner focused on the mathematical logic of the KeyFrame
interpolation and the other ensuring the JSON parsing and factory patterns were
robustly implemented.

## CHANGES
We did not submit on time due to a religious holiday and personal issues (chronic
illness and other).

## Acknowledgements
nlohmann/json: For the JSON parsing framework

SFML documentation for guidance on transform and drawable: https://www.sfml-dev.org/tutorials/3.0/

Kenney Shape Characters (CC0): https://kenney.nl/assets/shape-characters

"Morning Mood" By Edvard Grieg, performed by Musopen Symphony Orchestra (CC-PDM): https://en.wikipedia.org/wiki/Morning_Mood

Roboto font by Christian Robertson (SIL Open Font License): https://fonts.google.com/specimen/Roboto/license