# LOZ-Gen
This is a randomly generating zelda-esc dungeon.
I know that the quality of the game itself is poor. But this is more a proof of concept.

To make the generating easier, every dungeon follows the same idea: you have one main room with several branches each with their own switch at the end and you have to get out of the last room.

When I generate a dungeon, it chooses a random sequence of switches to toggle and saves what branches you need to pass through (the same ones as the switches).
As it does so, it saves which branches should be traversable in a truth table.
After it finishes with the correct path, it randomly makes branch points with the same system.
After the truth table is complete, it simplifies their boolean expression.
This is then interpretted into the branches' structures. OR's become break points, AND's become sequential rooms, doors are placed which open/close according to their switches.
After that, the dungeon is completely generated and you can traverse the labrynth of changing states.
