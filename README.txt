Mcmaster University Comp Sci 3GC3
Final project. Planet model.
Due Friday Dec 20, 2016 at 12:00pm

Name : Hyonwoo Kee, Peng Zheng, Antonio Cordeiro
Macid : keeh, zhengp2, bastosa

This program uses C/C++ and OpenGL to produce and display an interactive terrain.

Run this program from commandline (linux)
$ make -f makefile
This will print available commands on terminal.

Run this program from executable
$ ./asgfinal

Commands:
(W/A/S/D) change camera position - free navigate
(J/L) scene rotation - rotate the globe horizontally
(C) reset scene rotation
(R) randomize scene
(Q) -> exit application

Notes:
Basic functionality: Implementation of height map uses unique approach.
Basic functionality: 2D height array maps into UV-map, as (size) ring * (size) vertices.
Basic functionality: Fault algorithm applied with a tweak, tryB module.
Basic functionality: Normal calculation uses unique approach.
Basic functionality: Instead of using gridBox, and vertex directly, map into matrix.
Lighting: materials were used independently
Lighting: on windows system, given 3 vertex for position cause error. giving 4th 1.0f resolved this.
Alpha blending: N/A
Particle Systems: N/A
Animated Characters: N/A
Picking: N/A
Advanced Camera control: swim-mode / scene rotation
Shaders: Shading, height dependent.
To be done: (K/I) scene rotation - rotate the globe vertically
height dependent texture mapping,
normal maps and texture maps to be directly imported from gridBox.

Reference:
OpenGL UV mapping for a sphere (having seam)
http://stackoverflow.com/questions/20155278/opengl-uv-mapping-for-a-sphere-having-seam

How to transform a plane into a sphere? [SOLVED] [closed]
http://mathoverflow.net/questions/9968/how-to-transform-a-plane-into-a-sphere-solved

Loading images for textures under Linux 
https://www.opengl.org/discussion_boards/showthread.php/158563-Loading-images-for-textures-under-Linux

MAIN MUST RETURN INT error in C++? 
https://answers.yahoo.com/question/index?qid=20110218060526AA7d7zB

UV mapping
http://en.wikipedia.org/wiki/UV_mapping

