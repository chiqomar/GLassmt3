#include "object.h"

#ifndef primitive_h
#define primitive_h

struct boxhdl : objecthdl
{
    boxhdl(float width, float height, float depth, int shade_model = white);
	~boxhdl();
};

struct spherehdl : objecthdl
{
	spherehdl(float radius, int levels, int slices, int shade_model = phong);
	~spherehdl();
};

struct cylinderhdl : objecthdl
{
	cylinderhdl(float radius, float height, int slices, int shade_model = phong);
	~cylinderhdl();
};

struct pyramidhdl : objecthdl
{
	pyramidhdl(float radius, float height, int slices, int shade_model = phong);
	~pyramidhdl();
};

#endif
