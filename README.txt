# Dalton Hildreth, ***REMOVED*** #

OS: Ubunutu 14.04 LTS 
🐧 <- this is a penguin in unicode, if you can't see it.
If you haven't looked it up yet, look it up. It's kinda cute.

Compile:
Run "make raytrace" (or just "make" apparently) in the local directory. It will
place the "raytrace" executable in a bin directory, and the *.o files in an obj
directory.


How to Run:

raytrace [input_file] [output_file]

The input and output files both require the extensions .txt and .ppm
respectively. If no output file is specified it is assumed to be named the same
as the input_file except with the .ppm extension. If neither parameter is given
the program will prompt for the input file and output file.


Some things of note:
I added a few extra keywords to the input file format:
	textureoff : stops applying the most recent texture
	softshadow : applies soft shadows to the scene
Depth of field was also implemented using the standard "viewdist d" format.


Scenes:
DOF.txt : Shows the effects of Depth of field
soft.txt : Shows the effects of Soft Shadows
TIR.txt : Shows the effects of Total Internal Reflection (This image was an
  accident of my modeling. I didn't mean to put the camera in the box)
TransShadow.txt : shows a simple transparent sphere with a shadow cast through it
Cube.txt : Shows a simple transparent "glass" cube



Crystal.txt : Entirely Creative scene 1 : NOT INCLUDED IN THIS VERSION 
ForTheSword.txt : Entirely Creative scene 2 : NOT INCLUDED IN THIS VERSION 


