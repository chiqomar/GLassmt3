#include "primitive.h"

/* boxhdl
 *
 * Generate the geometry and indices required to make a box.
 */
boxhdl::boxhdl(float width, float height, float depth, int shade_model)
{
	rigid.push_back(rigidhdl());

	rigid[0].material = "default";

	rigid[0].geometry.reserve(24);
	rigid[0].indices.reserve(36);

	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, -1.0,-width/2, -height/2, -depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, -1.0, width/2, -height/2, -depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, -1.0, width/2,  height/2, -depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, -1.0,-width/2,  height/2, -depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(0);
	rigid[0].indices.push_back(1);
	rigid[0].indices.push_back(2);
	rigid[0].indices.push_back(0);
	rigid[0].indices.push_back(2);
	rigid[0].indices.push_back(3);

	rigid[0].geometry.push_back(vec8f( -1.0, 0.0, 0.0,-width/2, -height/2,  depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( -1.0, 0.0, 0.0,-width/2, -height/2, -depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( -1.0, 0.0, 0.0,-width/2,  height/2, -depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( -1.0, 0.0, 0.0,-width/2,  height/2,  depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(4);
	rigid[0].indices.push_back(5);
	rigid[0].indices.push_back(6);
	rigid[0].indices.push_back(4);
	rigid[0].indices.push_back(6);
	rigid[0].indices.push_back(7);

	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0,-width/2, -height/2, -depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0, width/2, -height/2, -depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0, width/2, -height/2,  depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0,-width/2, -height/2,  depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(10);
	rigid[0].indices.push_back(9);
	rigid[0].indices.push_back(8);
	rigid[0].indices.push_back(11);
	rigid[0].indices.push_back(10);
	rigid[0].indices.push_back(8);

	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, 1.0, width/2,  height/2,  depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, 1.0, width/2, -height/2,  depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, 1.0,-width/2, -height/2,  depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 0.0, 1.0,-width/2,  height/2,  depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(12);
	rigid[0].indices.push_back(13);
	rigid[0].indices.push_back(14);
	rigid[0].indices.push_back(12);
	rigid[0].indices.push_back(14);
	rigid[0].indices.push_back(15);

	rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0, width/2,  height/2,  depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0,-width/2,  height/2,  depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0,-width/2,  height/2, -depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0, width/2,  height/2, -depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(16);
	rigid[0].indices.push_back(17);
	rigid[0].indices.push_back(18);
	rigid[0].indices.push_back(16);
	rigid[0].indices.push_back(18);
	rigid[0].indices.push_back(19);

	rigid[0].geometry.push_back(vec8f( 1.0, 0.0, 0.0, width/2,  height/2,  depth/2, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 1.0, 0.0, 0.0, width/2,  height/2, -depth/2, 1.0, 0.0));
	rigid[0].geometry.push_back(vec8f( 1.0, 0.0, 0.0, width/2, -height/2, -depth/2, 1.0, 1.0));
	rigid[0].geometry.push_back(vec8f( 1.0, 0.0, 0.0, width/2, -height/2,  depth/2, 0.0, 1.0));
	rigid[0].indices.push_back(20);
	rigid[0].indices.push_back(21);
	rigid[0].indices.push_back(22);
	rigid[0].indices.push_back(20);
	rigid[0].indices.push_back(22);
	rigid[0].indices.push_back(23);

	bound = vec6f(-width/2.0, width/2.0, -height/2.0, height/2.0, -depth/2.0, depth/2.0);

    if (shade_model == nosh)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == white)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == gouraud)
        material.insert(pair<string, materialhdl*>("default", new gouraudhdl()));
    if (shade_model == phong)
        material.insert(pair<string, materialhdl*>("default", new phonghdl()));
    if (shade_model == brick)
        material.insert(pair<string, materialhdl*>("default", new brickhdl()));
    if (shade_model == texture)
        material.insert(pair<string, materialhdl*>("default", new texturehdl()));
    if (shade_model == multitx)
        material.insert(pair<string, materialhdl*>("default", new multitxhdl()));
    if (shade_model == bumpmap)
        material.insert(pair<string, materialhdl*>("default", new bumpmaphdl()));
    if (shade_model == toon)
        material.insert(pair<string, materialhdl*>("default", new toonhdl()));
    if (shade_model == billboard)
        material.insert(pair<string, materialhdl*>("default", new billboardhdl()));
    
}

boxhdl::~boxhdl()
{

}

/* spherehdl
 *
 * Generate the geometry and indices required to make a sphere.
 */
spherehdl::spherehdl(float radius, int levels, int slices, int shade_model)
{
	rigid.push_back(rigidhdl());

	rigid[0].material = "default";

	rigid[0].geometry.reserve(2 + (levels-1)*slices);

    for (int i = 0; i <= levels; i++)
		for (int j = 0; j <= slices; j++)
		{
            
			vec3f dir(sin(m_pi*(float)i/(float)levels)*cos(2.0*m_pi*(float)j/(float)slices),
					  sin(m_pi*(float)i/(float)levels)*sin(2.0*m_pi*(float)j/(float)slices),
					  cos(m_pi*(float)i/(float)levels));
            
//            rigid[0].geometry.push_back(vec8f(dir[0], dir[1], dir[2], radius*dir[0], radius*dir[1], radius*dir[2], (float)j/(float)slices, 0.0));
            
            rigid[0].geometry.push_back(vec8f(dir[0], dir[1], dir[2], radius*dir[0], radius*dir[1], radius*dir[2], (float)j/(float)slices, (float)i/(float)levels));
		}

	for (int i = 0; i < levels; i++)
		for (int j = 0; j < slices; j++)
		{
			rigid[0].indices.push_back( i*(slices+1) + j);
			rigid[0].indices.push_back( i*(slices+1) + (j+1));
			rigid[0].indices.push_back( (i+1)*(slices+1) + j);

			rigid[0].indices.push_back( (i+1)*(slices+1) + j);
			rigid[0].indices.push_back( i*(slices+1) + (j+1));
			rigid[0].indices.push_back( (i+1)*(slices+1) + (j+1));
		}

	bound = vec6f(-radius, radius, -radius, radius, -radius, radius);

    if (shade_model == nosh)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == white)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == gouraud)
        material.insert(pair<string, materialhdl*>("default", new gouraudhdl()));
    if (shade_model == phong)
        material.insert(pair<string, materialhdl*>("default", new phonghdl()));
    if (shade_model == brick)
        material.insert(pair<string, materialhdl*>("default", new brickhdl()));
    if (shade_model == texture)
        material.insert(pair<string, materialhdl*>("default", new texturehdl()));
    if (shade_model == multitx)
        material.insert(pair<string, materialhdl*>("default", new multitxhdl()));
    if (shade_model == bumpmap)
        material.insert(pair<string, materialhdl*>("default", new bumpmaphdl()));
    if (shade_model == toon)
        material.insert(pair<string, materialhdl*>("default", new toonhdl()));
    if (shade_model == billboard)
        material.insert(pair<string, materialhdl*>("default", new billboardhdl()));
}

spherehdl::~spherehdl()
{

}

/* cylinderhdl
 *
 * Generate the geometry and indices required to make a cylinder.
 */
cylinderhdl::cylinderhdl(float radius, float height, int slices, int shade_model)
{
	rigid.push_back(rigidhdl());

	rigid[0].material = "default";

	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0,0.0, -height/2.0, 0.0, 0.0, 0.0));
	for (int i = 0; i < slices; i++)
		rigid[0].geometry.push_back(vec8f(0.0, -1.0, 0.0, radius*cos(2*m_pi*(float)i/(float)slices),-height/2.0,radius*sin(2*m_pi*(float)i/(float)slices),radius*sin(2*m_pi*(float)i/(float)slices), radius*cos(2*m_pi*(float)i/(float)slices)));
    
	for (int i = 0; i <= slices; i++)
		rigid[0].geometry.push_back(vec8f(cos(2*m_pi*(float)i/(float)slices),0.0,sin(2*m_pi*(float)i/(float)slices),radius*cos(2*m_pi*(float)i/(float)slices),-height/2.0,radius*sin(2*m_pi*(float)i/(float)slices), (float)i/(float)slices, 1.0));
    

	for (int i = 0; i <= slices; i++)
		rigid[0].geometry.push_back(vec8f(cos(2*m_pi*(float)i/(float)slices),0.0,sin(2*m_pi*(float)i/(float)slices),radius*cos(2*m_pi*(float)i/(float)slices),height/2.0,radius*sin(2*m_pi*(float)i/(float)slices),
								  (float)i/(float)slices, 0.0));
    
    

	for (int i = 0; i < slices; i++)
		rigid[0].geometry.push_back(vec8f(0.0, 1.0, 0.0,radius*cos(2*m_pi*(float)i/(float)slices),height/2.0,radius*sin(2*m_pi*(float)i/(float)slices),
                                          radius*sin(2*m_pi*(float)i/(float)slices), radius*cos(2*m_pi*(float)i/(float)slices)));

	rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0,0.0, height/2.0, 0.0, 0.0, 0.0));

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(1 + (i+1)%slices);
		rigid[0].indices.push_back(1 + i);
		rigid[0].indices.push_back(0);
	}

	for (int i = 0; i <= slices; i++)
	{
		rigid[0].indices.push_back((slices+1) + i);
		rigid[0].indices.push_back((slices+1) + (i+1));
		rigid[0].indices.push_back(2*(slices+1) + i);

		rigid[0].indices.push_back(2*(slices+1) + i);
		rigid[0].indices.push_back((slices+1) + (i+1));
		rigid[0].indices.push_back(2*(slices+1) + (i+1));
	}

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(3*(slices+1) + i);
		rigid[0].indices.push_back(3*(slices+1) + (i+1)%slices);
		rigid[0].indices.push_back(4*(slices + 1) - 1);
	}

	bound = vec6f(-radius, radius, -height/2.0, height/2.0, -radius, radius);

    if (shade_model == nosh)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == white)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == gouraud)
        material.insert(pair<string, materialhdl*>("default", new gouraudhdl()));
    if (shade_model == phong)
        material.insert(pair<string, materialhdl*>("default", new phonghdl()));
    if (shade_model == brick)
        material.insert(pair<string, materialhdl*>("default", new brickhdl()));
    if (shade_model == texture)
        material.insert(pair<string, materialhdl*>("default", new texturehdl()));
    if (shade_model == multitx)
        material.insert(pair<string, materialhdl*>("default", new multitxhdl()));
    if (shade_model == bumpmap)
        material.insert(pair<string, materialhdl*>("default", new bumpmaphdl()));
    if (shade_model == toon)
        material.insert(pair<string, materialhdl*>("default", new toonhdl()));
    if (shade_model == billboard)
        material.insert(pair<string, materialhdl*>("default", new billboardhdl()));
}

cylinderhdl::~cylinderhdl()
{

}

/* pyramidhdl
 *
 * Generate the geometry and indices required to make a pyramid.
 */
pyramidhdl::pyramidhdl(float radius, float height, int slices, int shade_model)
{
	rigid.push_back(rigidhdl());

	rigid[0].material = "default";

	float nheight = sqrt(1.0f/(1.0f + (height*height)/(radius*radius)));
	float nlength = height*nheight/radius;

	rigid[0].geometry.push_back(vec8f( 0.0, -1.0, 0.0,0.0, -height/2.0, 0.0, 0.0, 0.0));
	for (int i = 0; i < slices; i++)
		rigid[0].geometry.push_back(vec8f(0.0, -1.0, 0.0,
            radius*cos(2*m_pi*(float)i/(float)slices),
            -height/2.0,
            radius*sin(2*m_pi*(float)i/(float)slices),
            radius*sin(2*m_pi*(float)i/(float)slices),
            radius*cos(2*m_pi*(float)i/(float)slices)));

	for (int i = 0; i <= slices; i++)
		rigid[0].geometry.push_back(vec8f(
            nlength*cos(2*m_pi*(float)i/(float)slices),
            nheight,
            nlength*sin(2*m_pi*(float)i/(float)slices),
            radius*cos(2*m_pi*(float)i/(float)slices),
            -height/2.0,
            radius*sin(2*m_pi*(float)i/(float)slices),
            3*(float)i/(float)slices, 1.0));

	for (int i = 0; i < slices; i++)
		rigid[0].geometry.push_back(
            vec8f(
            nlength*cos(2*m_pi*((float)i + 0.5)/(float)slices),
            nheight,
            nlength*sin(2*m_pi*((float)i + 0.5)/(float)slices),
            0.0,
            height/2.0,
            0.0,
            3*((float)i + 0.5)/(float)slices,
            0.0));
	//rigid[0].geometry.push_back(vec8f( 0.0, 1.0, 0.0,0.0, height/2.0, 0.0, 0.0, 0.0));

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(1 + (i+1)%slices);
		rigid[0].indices.push_back(1 + i);
		rigid[0].indices.push_back(0);
	}

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(slices+1 + i);
		rigid[0].indices.push_back(slices+1 + (i+1));
		rigid[0].indices.push_back(2*(slices+1) + i);
	}

	bound = vec6f(-radius, radius, -height/2.0, height/2.0, -radius, radius);

    if (shade_model == nosh)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == white)
        material.insert(pair<string, materialhdl*>("default", new whitehdl()));
    if (shade_model == gouraud)
        material.insert(pair<string, materialhdl*>("default", new gouraudhdl()));
    if (shade_model == phong)
        material.insert(pair<string, materialhdl*>("default", new phonghdl()));
    if (shade_model == brick)
        material.insert(pair<string, materialhdl*>("default", new brickhdl()));
    if (shade_model == texture)
        material.insert(pair<string, materialhdl*>("default", new texturehdl()));
    if (shade_model == multitx)
        material.insert(pair<string, materialhdl*>("default", new multitxhdl()));
    if (shade_model == bumpmap)
        material.insert(pair<string, materialhdl*>("default", new bumpmaphdl()));
    if (shade_model == toon)
        material.insert(pair<string, materialhdl*>("default", new toonhdl()));
    if (shade_model == billboard)
        material.insert(pair<string, materialhdl*>("default", new billboardhdl()));
}

pyramidhdl::~pyramidhdl()
{

}
