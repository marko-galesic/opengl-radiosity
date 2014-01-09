#include "GL/freeglut.h"
#include "GL/gl.h"
#include "struct.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <pngwriter.h>

using namespace std;
using namespace radiosity_structs;

int WINDOW_HEIGHT = 512;
int WINDOW_WIDTH = 512;

enum HEMICUBE_VIEW { LEFT, RIGHT, TOP, BOTTOM, FRONT };

void 	checkWhetherPatchHasComponentMax(patch * p, light * totalLight);
void renderHemicubeView(point * center, vector * up, vector * direction, HEMICUBE_VIEW view);
light * calculauteIncidentLight(point * center, vector * normal, vector * up);
void getHemicubePixels();
hemicube * applyMultiplierHemicube(hemicube * h);
unsigned char * applyMultiplier(unsigned char * buffer, HEMICUBE_VIEW view);
unsigned char * getHemicubePixels(HEMICUBE_VIEW view);
light * getTotalLightOfView(hemicube * h, HEMICUBE_VIEW view);
void renderScene();
int iterations = 18;
bool running = true;
