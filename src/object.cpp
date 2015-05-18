#include "object.h"

rigidhdl::rigidhdl()
{

}

rigidhdl::~rigidhdl()
{

}

/* draw
 *
 * Draw a rigid body.
 */
void rigidhdl::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
       glEnableClientState(GL_NORMAL_ARRAY);
    //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*8, geometry.data() );
    
    //for vec8f
    
    glInterleavedArrays(GL_N3F_V3F, sizeof(GLfloat)*8, geometry.data());

    //glNormalPointer(GL_FLOAT, sizeof(GLfloat)*3, this->normals.data());
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());
    glDisableClientState(GL_VERTEX_ARRAY);
    
       glDisableClientState(GL_NORMAL_ARRAY);
    //  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}



//void rigidhdl::loadNormals()
//{
//    for (int j = 0; j < this->indices.size(); j+=3)
//    {
//        vec3f normal = norm((vec3f)this->geometry[this->indices[j + 0]](3,6) +
//                            (vec3f)this->geometry[this->indices[j + 1]](3,6) +
//                            (vec3f)this->geometry[this->indices[j + 2]](3,6));
////        vec3f vector = ((vec3f)this->geometry[this->indices[j + 0]](0,3) +
////                        (vec3f)this->geometry[this->indices[j + 1]](0,3) +
////                        (vec3f)this->geometry[this->indices[j + 2]](0,3))/3.0f;
//        //gonna switch positions of the normals and vectors...hopefully the indices are good
////        vec6f switchup;
////        switchup.set(0,3,normal);
////        switchup.set(3,6,vector);
//        this->normals.push_back(normal);
//    }
//    
//}



objecthdl::objecthdl()
{
	position = vec3f(0.0, 0.0, 0.0);
	orientation = vec3f(0.0, 0.0, 0.0);
	bound = vec6f(1.0e6, -1.0e6, 1.0e6, -1.0e6, 1.0e6, -1.0e6);
	scale = 1.0;
}

objecthdl::objecthdl(const objecthdl &o)
{
	position = o.position;
	orientation = o.orientation;
	bound = o.bound;
	scale = o.scale;
	rigid = o.rigid;
	for (map<string, materialhdl*>::const_iterator i = o.material.begin(); i != o.material.end(); i++)
		material.insert(pair<string, materialhdl*>(i->first, i->second->clone()));
}

objecthdl::~objecthdl()
{
	for (map<string, materialhdl*>::iterator i = material.begin(); i != material.end(); i++)
		if (i->second != NULL)
		{
			delete i->second;
			i->second = NULL;
		}

	material.clear();
}

void objecthdl::choose_shader(int shade_model) {
    
}

void objecthdl::bound_draw(	vector<vec8f> &geometry, vector<int> &indices)
{
    int curr_prog;
    glGetIntegerv(GL_CURRENT_PROGRAM,&curr_prog);
    rigid.back().material.clear();
    rigid.back().material = "default";
    if (curr_prog != materialhdl::progmap["white"])
        glUseProgram(materialhdl::progmap["white"]);
    //cout << "Set shader:" << materialhdl::progmap["white"] << " Curr shader: " << curr_prog <<endl;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*8, geometry.data());
    glDrawElements(GL_LINES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());
    geometry.clear();
    indices.clear();
    if (curr_prog != materialhdl::progmap["white"])
        glUseProgram(curr_prog);
}

/* draw
 *
 * Draw the model. Don't forget to apply the transformations necessary
 * for position, orientation, and scale.
 */
void objecthdl::draw(const vector<lighthdl*> &lights)
{
	glTranslatef(position[0],position[1],position[2]);
	glRotatef(orientation[0], 1.0, 0.0, 0.0);
	glRotatef(orientation[1], 0.0, 1.0, 0.0);
	glRotatef(orientation[2], 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	for (int i = 0; i < rigid.size(); i++)
	{
        if (material[rigid[i].material] == NULL)
            material.insert(pair<string, materialhdl*>("default", new gouraudhdl()));
        material["default"]->apply(lights);
        rigid[i].draw();
    }

	glScalef(1.0/scale, 1.0/scale, 1.0/scale);
	glRotatef(-orientation[2], 0.0, 0.0, 1.0);
	glRotatef(-orientation[1], 0.0, 1.0, 0.0);
	glRotatef(-orientation[0], 1.0, 0.0, 0.0);
	glTranslatef(-position[0],-position[1],-position[2]);
}

/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound()
{
	glTranslatef(position[0],position[1],position[2]);
	glRotatef(orientation[0], 1.0, 0.0, 0.0);
	glRotatef(orientation[1], 0.0, 1.0, 0.0);
	glRotatef(orientation[2], 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);
	vector<vec8f> bound_geometry;
	vector<int> bound_indices;
	bound_geometry.reserve(8);
	bound_geometry.push_back(vec8f(bound[0], bound[2], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[2], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[3], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[3], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[2], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[2], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[3], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[3], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_indices.reserve(24);
	for (int i = 0; i < 4; i++)
	{
		bound_indices.push_back(i);
		bound_indices.push_back((i+1)%4);
		bound_indices.push_back(4+i);
		bound_indices.push_back(4+(i+1)%4);
		bound_indices.push_back(i);
		bound_indices.push_back(4+i);
	}
    
    bound_draw(bound_geometry, bound_indices);
    
	glScalef(1.0/scale, 1.0/scale, 1.0/scale);
	glRotatef(-orientation[2], 0.0, 0.0, 1.0);
	glRotatef(-orientation[1], 0.0, 1.0, 0.0);
	glRotatef(-orientation[0], 1.0, 0.0, 0.0);
	glTranslatef(-position[0],-position[1],-position[2]);
}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(bool face)
{
	float radius = 0.0;
	for (int i = 0; i < 6; i++)
		if (abs(bound[i]) > radius)
			radius = abs(bound[i]);

	vector<vec8f> normal_geometry;
	vector<int> normal_indices;

    glTranslatef(position[0],position[1],position[2]);
    glRotatef(orientation[0], 1.0, 0.0, 0.0);
    glRotatef(orientation[1], 0.0, 1.0, 0.0);
    glRotatef(orientation[2], 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
	for (int i = 0; i < rigid.size(); i++)
	{
		if (!face)
		{
			for (int j = 0; j < rigid[i].geometry.size(); j++)
			{
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(rigid[i].geometry[j]);
				normal_geometry.back().set(3,6,vec3f(0.0, 0.0, 0.0));
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(rigid[i].geometry[j]);
				normal_geometry.back().set(0,3,(vec3f)(normal_geometry.back()(0,3) + radius*0.1f*normal_geometry.back()(3,6)));
				normal_geometry.back().set(3,6,vec3f(0.0, 0.0, 0.0));
			}
		}
		else
		{
			for (int j = 0; j < rigid[i].indices.size(); j+=3)
			{
				vec3f normal = norm((vec3f)rigid[i].geometry[rigid[i].indices[j + 0]](0,3) +
									(vec3f)rigid[i].geometry[rigid[i].indices[j + 1]](0,3) +
									(vec3f)rigid[i].geometry[rigid[i].indices[j + 2]](0,3));
				vec3f center = ((vec3f)rigid[i].geometry[rigid[i].indices[j + 0]](3,6) +
								(vec3f)rigid[i].geometry[rigid[i].indices[j + 1]](3,6) +
								(vec3f)rigid[i].geometry[rigid[i].indices[j + 2]](3,6))/3.0f;
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(center);
				normal_geometry.back().set(3,8,vec5f(0.0, 0.0, 0.0, 0.0, 0.0));
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(center + radius*0.1f*normal);
				normal_geometry.back().set(3,8,vec5f(0.0, 0.0, 0.0, 0.0, 0.0));
			}
		}

        bound_draw(normal_geometry, normal_indices);
	}
    glScalef(1.0/scale, 1.0/scale, 1.0/scale);
    glRotatef(-orientation[2], 0.0, 0.0, 1.0);
    glRotatef(-orientation[1], 0.0, 1.0, 0.0);
    glRotatef(-orientation[0], 1.0, 0.0, 0.0);
    glTranslatef(-position[0],-position[1],-position[2]);
}
