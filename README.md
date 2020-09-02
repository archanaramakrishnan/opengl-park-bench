# OpenGL Park Bench Scene
With the necessary skills to create a 3D scene, I really wanted to model a real-life outdoor view.
![Image of First 3D OpenGL Project](https://github.com/archanaramakrishnan/opengl-park-bench/blob/master/opengl_park_bench.png)

I love nature, trees and spending time outdoors, which is why I chose the scene of a park bench under some trees on a spread of lush green grass. I designed the scene by putting together simple shapes in a modular way
 - cuboidal blocks: bench
 - conical tetrahedron: trees
 - cylinder: trunk
 - mini tetrahedrons: blades of grass

## How to Use
Run instructions for Mac:
1. Clone the repository and execute the following commands:<br>
    `cd opengl-park-bench/project2/` <br>
    `make` <br>
    `export DYLD_LIBRARY_PATH=../lib` <br>
    `./main` <br>
2. A window will show up with the scene
3. Click and move the cursor around to navigate. Press down shift while navigating to move around the model.

## Implementation and 3D Modelling

**Tree**<br>
A cylinder for the trunk and tetrahedron leaves, with a constructor that takes in the height and the center coordinate of the tree’s trunk. I set the height of the trunk to be 3/8th of the total height of the tree to achieve a realistic model. I added eight trees close together to create a realistic scene of a natural park. With an average height of 20-30 ft., there are four of them on z=0 and four on z=15.

**Grass**<br>
To simplify understanding and creating the geometry, I used a circle around the three points of the base of the blade of grass with the vertices at 0, 120 and 240 degrees to be equally positioned and the height vertex to be positioned in the center of the circle. I added a random offset when I added each model to make it look more natural, rather than a uniform looking rows and columns of a carpet-like grass.

**Earth** <br>
The ground is on the XZ plane, which is where I generated the patch of grass. I started out with 100x100 points but spaced it out 5*x which was too much. I tried a 20x20 loop instead 100x100 turned out to be better for a denser patch of grass.

## Challenges
 - **Eye view:** My initial eye had to be adjusted by adding a vector to have a higher line of sight, so it looked a bit like looking down on the view.
- **Natural look of grass:** Randomizing my grass within the bounds of the other objects was hard because I was having trouble deciding which values to tweak to keep them contained within the patch.
- **Toggle pins window:** I thought creating the 4 legs of the bench would be complicated. However, using an initial coordinate and setting constant lengths and width dimensions were very helpful.

## Screenshots
![Image of First 3D OpenGL Project](https://github.com/archanaramakrishnan/opengl-park-bench/blob/master/Zoomed_Out.png)
![Image of First 3D OpenGL Project](https://github.com/archanaramakrishnan/opengl-park-bench/blob/master/Top_View_Bench.png)
![Image of First 3D OpenGL Project](https://github.com/archanaramakrishnan/opengl-park-bench/blob/master/Diagonal_View_Bench.png)
