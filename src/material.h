#include "core/geometry.h"
#include "standard.h"
#include "opengl.h"

using namespace core;

#ifndef material_h
#define material_h

struct lighthdl;

struct materialhdl
{
	materialhdl();
	virtual ~materialhdl();

	string type;

	virtual void apply(const vector<lighthdl*> &lights) = 0;
	virtual materialhdl *clone() const = 0;
    
    static map<string,int> progmap;
};

struct whitehdl : materialhdl
{
	whitehdl();
	~whitehdl();

	static GLuint vertex;
	static GLuint fragment;
	static GLuint program;

	void apply(const vector<lighthdl*> &lights);
	materialhdl *clone() const;
};

struct gouraudhdl : materialhdl
{
	gouraudhdl();
	~gouraudhdl();

	vec3f emission;
	vec3f ambient;
	vec3f diffuse;
	vec3f specular;
	float shininess;

	static GLuint vertex;
	static GLuint fragment;
	static GLuint program;

	void apply(const vector<lighthdl*> &lights);
	materialhdl *clone() const;
};

struct phonghdl : materialhdl
{
	phonghdl();
	~phonghdl();

	vec3f emission;
	vec3f ambient;
	vec3f diffuse;
	vec3f specular;
	float shininess;

	static GLuint vertex;
	static GLuint fragment;
	static GLuint program;

	void apply(const vector<lighthdl*> &lights);
	materialhdl *clone() const;
};

struct brickhdl : materialhdl
{
	brickhdl();
	~brickhdl();

	static GLuint vertex;
	static GLuint fragment;
	static GLuint program;

	void apply(const vector<lighthdl*> &lights);
	materialhdl *clone() const;
};

struct texturehdl : materialhdl
{
	texturehdl();
	~texturehdl();

	float shininess;

	static GLuint vertex;
	static GLuint fragment;
	static GLuint program;

	static GLuint texture;
    
    void loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, double &u, double &v);
    
	void apply(const vector<lighthdl*> &lights);
	materialhdl *clone() const;
};

struct multitxhdl : materialhdl
{
    multitxhdl();
    ~multitxhdl();
    
    float shininess;
    
    static GLuint vertex;
    static GLuint fragment;
    static GLuint program;
    
    static GLuint texture;
    static GLuint text2;
    
    void loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string file);
    
    void apply(const vector<lighthdl*> &lights);
    materialhdl *clone() const;
};

struct normmaphdl : materialhdl
{
    normmaphdl();
    ~normmaphdl();
    
    float shininess;
    
    static GLuint vertex;
    static GLuint fragment;
    static GLuint program;
    
    static GLuint texture;
    static GLuint normalmap;
    
    void loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string file);
    void computeTangentBasis(
                             // inputs
                             vec3f & vertices,
                             vec2f & uvs,
                             vec3f & normals,
                             // outputs
                             vec3f & tangents,
                             vec3f & bitangents
                             );
    void apply(const vector<lighthdl*> &lights);
    materialhdl *clone() const;
};

struct bumpmaphdl : materialhdl
{
    bumpmaphdl();
    ~bumpmaphdl();
    
    float shininess;
    
    static GLuint vertex;
    static GLuint fragment;
    static GLuint program;
    
    static GLuint texture;
    static GLuint normalmap;
    
    void loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string file);
    void apply(const vector<lighthdl*> &lights);
    materialhdl *clone() const;
};

#endif
