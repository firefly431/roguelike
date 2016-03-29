# Some implementation details

Tilemap for map data (collision and rendering)

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
  * Position (x, y)
  * Collision (size)
  * Input (probably a tagged union)
  * Equipment
  * Inventory (should be a pointer so as not to waste memory)
  * Stats (HP can be displayed if graphical)
  * Graphical (sprites, etc).

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
