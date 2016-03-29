# Some implementation details

Tilemap for map data (collision and rendering) (keep a thing for rooms)

Entities managed using entity component system

Active entities are within some radius of player position, and inactive enemies are placed in a priority queue

Should particles be part of ECS? Probably not

Systems
  * Animations
  * Player input (blocking *if no animations*; if nonblocking and no input, skip rest)
  * AI
  * Input status update (movement and collision)
  * Rendering

Components (all have `id` field)
  * (Graphical) Position (x, y)
  * Collision (tile x, y, size)
  * Input (probably a tagged union)
  * Equipment
  * Inventory (should be a pointer so as not to waste memory)
  * Stats (HP can be displayed if graphical) (Maybe status effects)
  * Graphical (sprites, etc).
  * (Maybe an event listener? For example, if it collides with something, it could reveal itself or something)

Entities stored as array of bitmasks (`int` probably works) and flat arrays of components

Keep an index of next free space, and when making new entity:
  * Start at `free % MAX_ENTITIES`
  * While `component_mask[free]`
    * Increment `free`
    * If `free >= MAX_ENTITIES`, panic!
  * Return `free++`

Some entities:
  * Player
  * Enemy
  * Door
  * Item
  * Treasure
  * Trap

Arrows and spells aren't entities; they're just functions that are called by the input system

Map Generation
  * Generate a whole bunch of rooms relatively near each other (rectangles and L-shapes)
  * Connect randomly
  * Pick a good starting point (a corner maybe)
  * Gradually increase difficulty as you traverse a path (no repeats); maybe randomly restart somewhere along the way and put a locked door
  * Other rooms should have difficulty quite higher than those next to them
  * Put doors randomly
  * Populate rooms according to difficulty

To center the screen:
  * Locate current room
  * If in room:
    * If room size is less than window size, expand dimensions accordingly (center on room)
    * Center display on player
    * Move display so that it is contained in the expanded room dimensions
  * Else center display on player
