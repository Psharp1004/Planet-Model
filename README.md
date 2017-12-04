# Planet-Model
CS3GC3 Project

This program uses C/C++ and OpenGL(GLUT) to produce and display an interactive terrain.

## Tools
Visual Studio IDE

## Insturction
Run this program from commandline (linux)
$ make -f makefile
This will print available commands on terminal.

Run this program from executable (asgfinal.exe)
$ ./asgfinal

Commands:

(Mouse) rotate camera angle

(W/S) change camera position - zoom in(W) and out(S)

(A/D) change camera position - strafe camera to left(A) and right(D) 

(J/L) scene rotation - rotate the globe horizontally

(C) reset scene rotation

(R) randomize scene

(Q) exit application

## Note
Basic functionality: Implementation of height map uses unique approach.

Basic functionality: 2D height array maps into UV-map, as (size) ring * (size) vertices.

Basic functionality: Fault algorithm applied with a tweak, tryB module.

Basic functionality: Normal calculation uses unique approach.

Basic functionality: Instead of using gridBox, and vertex directly, map into matrix.

Lighting: materials were used independently

Lighting: on windows system, given 3 vertex for position cause error. giving 4th 1.0f resolved this.

Advanced Camera control: swim-mode / scene rotation

Shaders: Shading, height dependent.

## Things To be done: 
1. (K/I) scene rotation - rotate the globe vertically
2. Height dependent texture mapping
3. Normal maps and texture maps to be directly imported from gridBox

## Random Scene 1
![alt text](https://github.com/Psharp1004/Planet-Model/blob/master/screenshot1.JPG)

## Random Scene 2 (Zoomed Out)
![alt text](https://github.com/Psharp1004/Planet-Model/blob/master/screenshot2.JPG)
