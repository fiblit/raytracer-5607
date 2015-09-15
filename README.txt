# Made by Dalton Hildreth, ***REMOVED*** #

This program will generate an (interesting) ASCII ppm image of variable width and height as determined by an input file.

The program will start by prompting for the input file's name. Please include ".txt" at the end of the name for the program to function. For example you could enter "myfile.txt" and the program would attempt to process it, but if you input "myfile" it will give you an error message and exit. If the file doesn't exist, give you an error message and exit.

Once the file is deemed to exist, the program will check if the contents are properly formatted. The first line of the input file should contain the following: "imsize <width> <height>". Where the items in brackets represent integer parameters.  Anything past this information will be ignored. If the header is incorrect in any way, the program will give you the proper error message and exit.

The program will then create the proper header for an ASCII ppm file, which is titled the same as the input file. (With the .ppm instead .txt extension, of course)

It will then fill the contents of the ASCII ppm file. If opened with GIMP, or a similar image processing program, the image should look like a color spectrum, with red on the y axis, and blue on the x axis. (Green is set to one intensity (128/255) throughout.)


