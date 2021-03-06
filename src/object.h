#include "core/geometry.h"
#include "standard.h"
#include "opengl.h"

#include "material.h"

using namespace core;

#ifndef object_h
#define object_h

struct lighthdl;

/* This represents a rigid body, which
 * is just a group of geometry to be
 * rendered together. Its grouped in
 * this way so that you can apply different
 * materials to different parts of the
 * same model.
 */
struct rigidhdl
{
	rigidhdl();
	~rigidhdl();

	vector<vec8f> geometry;
	vector<int> indices;
    
    //edits
    vector<vec3f> normals;
    
    //edits
    
	string material;

	void draw();
    void loadNormals();
};

struct objecthdl
{
	objecthdl();
	objecthdl(const objecthdl &o);
	virtual ~objecthdl();

	vector<rigidhdl> rigid;
	map<string, materialhdl*> material;

    enum
    {
        nosh = 0,
        white = 1,
        gouraud = 2,
        phong = 3,
        brick = 4,
        texture = 5,
        multitx = 6,
        bumpmap = 7,
        toon = 8,
        billboard = 9
    };
    
	vec3f position;
	vec3f orientation;
	float scale;

	// The bounding box of this object
	// (left, right, bottom, top, front, back)
	vec6f bound;

	void draw(const vector<lighthdl*> &lights);
    void bound_draw(vector<vec8f> &geometry, vector<int> &indices);
	void draw_bound();
	void draw_normals(bool face = false);
    void choose_shader(int shade_model);
};

#endif
