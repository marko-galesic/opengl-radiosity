opengl-radiosity
================

A Radiosity Implementation with Hemicubes and OpenGL

###### Compile (G++): 
g++ -o radiosity radiosity.cpp struct.cpp vector.cpp flux.cpp tiny_obj_loader.cc -lGLU -lGL -lglut

###### Run
`./radiosity <geometry file> <number of iterations> <debug>`


Based on http://freespace.virgin.net/hugo.elias/radiosity/radiosity.htm
