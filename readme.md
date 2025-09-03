# Toy Renderer

Learning how to do rendering from scratch.

## Progress

### Day 1
First attempt at rendering wireframe.

![Diablo Pose](progress/day1.png)

### Day 2
Found mistake in interpretation of Waveform obj format and fix.

![Diablo Pose](progress/day2-1.png)

First attempt at "filling" triangles

![Jank triangles](progress/day2-2.png)

Finished attempt for filling triangles

![Filled triangles](progress/day2-3.png)

### Day 3
Drawing model with filled triangles

![Filled model](progress/day3-1.png)

Playing around with colors and alpha values

![Rainbow triangle](progress/day3-2.png)

"Wireframe" without calling line()

![Wireframe rainbow triangle](progress/day3-3.png)

### Day 4
Cull triangles that are backfacing

![Grayscale Depth Buffer](progress/day4-1.png)

Rotate model

![Rotated model](progress/day4-2.png)

### Day 5
Hand of the model is clipped after central projection

![Clipped model](progress/day5-1.png)

Applying rotation later helped abit but still clipping abit

![Less clipped model](progress/day5-2.png)

Store z buffer as a vector of floats instead

![Diablo model](progress/day5-3.png)