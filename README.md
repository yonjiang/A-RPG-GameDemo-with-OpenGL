# simple_Demo
This is a 2d maze game completed with opengl, in which the more difficult part is the random maze preparation and material loading, and then the logic part of the program, for your reference

 
System environment:
Win10 VS2018 under the opengl environment
Design principle:
Use the drawing function of opengl to draw a random dungeon, and use black to represent the wall.
Draw an exclusive color for the area where the NPC is located, immediately read the color of the eight directions around the protagonist, and judge the location of the wall and the NPC
The collision detection and interactive judgment can be achieved by limiting the movement of the perspective through judgment
The entire scene is then redrawn on top of the original
Each function module's main realization procedure
1. Randomly generate maps by prim algorithm and store them in a two-dimensional vector array;
There are three main algorithms for generating mazes:
1.Recursive backtracker
2.Randomized Prim's algorithm (Randomized Prim algorithm, reminds me of the Prim algorithm in the minimum spanning tree)
3.Recursive division
Using the prim algorithm, there are more branch roads and they are more natural
2. Draw the pre-determination area. We make the determination by reading the surrounding colors
Draw the generated random map. When the value of the two-dimensional array is 1, draw a black square.
Generate map NPC at random locations, function Draw_Ghost();Draw a blue square
The four corners randomly generate endpoint NPCS, the Draw_Alice() function;Draw a green square;
3. Move and collision determination
Interactive judgment:
When the game is in the start screen, read the mouse position, and then judge the start
Because the protagonist is located in the center of the map, that is, the color of eight points around the protagonist is read, up, down, left, right, top left, bottom left, bottom right, the color of eight points is read, to judge whether there are NPCS around, and change the global function value; 
Similarly, the wall can be judged by reading the surrounding color
If it is a wall, move the character to the opposite direction of the button
In the middle, movieStyle is another mode of operation that I use. This mode will produce an acceleration when the button is pressed and will not stop immediately.
Since this is a continuous read, press the command once per generation, leaving key_time+=0.1
The effect is that the character can press the key according to the length of time to produce different speed;
Finally, the edge() function is run to restrict the movement.
4. Map
Character map:
A. protagonist b. terminal NPC c. map NPC
D. wall (not adopted)
Map is painted by myself!!!!!!
Implementation function is in the original decision function with map;
When I need to point out, my map is drawn based on a group of m*n Numbers,x and y represent the coordinates of the array, and then draw a quadrilateral with side size;
From the Internet to find a text implementation code
Using the MCI Api, and then from the Internet to find a horror music, imported in
Call other functions inside myDisplay()
BeginGame is the judgment of whether the opening start has a click or not
Then draw the decision area, map it, and then draw the text
Because many interactive judgments in "meet()" will affect the global bool variables, which are used to determine the stage of the game and thus affect the display of the dialogue
Curriculum design summary and experience
The forehead...
Mainly feel the gap between the ideal and the reality...
The original idea was to make a complete dungeon game, the protagonist can have animation, can have walking posture, there are two or three NPCS, can be given a flashlight and map, but also want to make a miniature version of the map, can be expanded
And the main way to do that is to think about it
Did not achieve now, main reason is to spend time very much, do not have time to do near exam forehead
Secondly, my code habit is very bad. I should analyze each module in advance and then analyze the parameters of each module
For example, a map should be a module that determines which attributes of the map to generate
NPCS are modules that have attributes of NPCS and so on
And then you call these modules, and you don't, and you just write what
Such code is not very good, not high efficiency
But fortunately the general thing or do out, or pretty happy.

