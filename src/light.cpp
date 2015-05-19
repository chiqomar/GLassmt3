#include "light.h"
#include "object.h"
#include "opengl.h"

int directionalhdl::count = -1;
int spothdl::count = -1;
int pointhdl::count = -1;

lighthdl::lighthdl()
{
    model = NULL;
    type = "light";
}

lighthdl::lighthdl(const vec3f &ambient, const vec3f &diffuse, const vec3f &specular)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    model = NULL;
    type = "light";
}

lighthdl::~lighthdl()
{
    
}

directionalhdl::directionalhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
    type = "directional";
    count++;
}

directionalhdl::directionalhdl(const vec3f &direction, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular)
{
    type = "directional";
    count++;
}

directionalhdl::~directionalhdl()
{
    count--;
}

void directionalhdl::update()
{
    if (model != NULL)
    {
        glTranslatef(model->position[0],model->position[1],model->position[2]);
        glRotatef(model->orientation[0], 1.0, 0.0, 0.0);
        glRotatef(model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(model->orientation[2], 0.0, 0.0, 1.0);
        
        vec4f direction = vec4f(0.0,0.0,-1.0,0.0);
        float modelview_x[16];
        
        glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, modelview_x);

        mat4f model_tmp;
        mat4f normal_mat;
        
        model_tmp = mat4f((float)modelview_x[0],(float)modelview_x[1],(float)modelview_x[2],(float)modelview_x[3],(float)modelview_x[4],(float)modelview_x[5],(float)modelview_x[6],(float)modelview_x[7],(float)modelview_x[8],(float)modelview_x[9],(float)modelview_x[10],(float)modelview_x[11],(float)modelview_x[12],(float)modelview_x[13],(float)modelview_x[14],(float)modelview_x[15]);
        
        normal_mat = inverse(transpose(model_tmp));
        this->direction = normal_mat * direction;
        
//        int program;
//        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
//        string dir_str = di + to_string(count) + "].direction";
//        int dir_loc = glGetUniformLocation(program, dir_str.c_str());
//        glUniform3f(dir_loc, direction[0], direction[1],direction[2]);

        glRotatef(-model->orientation[2], 0.0, 0.0, 1.0);
        glRotatef(-model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(-model->orientation[0], 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0],-model->position[1],-model->position[2]);
    }
}

void directionalhdl::apply(string name, GLuint program)
{
    /* TODO Assignment 3: Pass all necessary uniforms to the shaders for the directional light.
     */
    if (name == "white") {
        
        glUseProgram(program);
        
    } else {
        glUseProgram(program);
        string dir_str = di + to_string(count) + "].direction";
        string amb_str = di + to_string(count) + "].ambient";
        string spec_str = di + to_string(count) + "].specular";
        string dif_str = di + to_string(count) + "].diffuse";
        
        int dir_loc = glGetUniformLocation(program, dir_str.c_str());
        int amb_loc = glGetUniformLocation(program, amb_str.c_str());
        int spec_loc = glGetUniformLocation(program, spec_str.c_str());
        int dif_loc = glGetUniformLocation(program, dif_str.c_str());
        
        glUniform3f(dir_loc, direction[0], direction[1],direction[2]);
        glUniform3f(amb_loc, ambient[0] ,ambient[1] ,ambient[2]);
        glUniform3f(spec_loc, specular[0], specular[1], specular[2]);
        glUniform3f(dif_loc, diffuse[0], diffuse[1],diffuse[2]);
        //cout << "Direction:" <<this->direction[0] << endl;
    }
}

pointhdl::pointhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
    this->attenuation = vec3f(1.0, 0.14, 0.7);
    type = "point";
    count++;
}

pointhdl::pointhdl(const vec3f &position, const vec3f &attenuation, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
    this->attenuation = attenuation;
    type = "point";
    count++;
}

pointhdl::~pointhdl()
{
    count--;
}

void pointhdl::update()
{
    if (model != NULL)
    {
        glTranslatef(model->position[0],model->position[1],model->position[2]);
        glRotatef(model->orientation[0], 1.0, 0.0, 0.0);
        glRotatef(model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(model->orientation[2], 0.0, 0.0, 1.0);
        
        
        vec4f position = vec4f(0.0, 0.0, 0.0, 1.0);
        float modelview_x[16];
        
        glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, modelview_x);
        
        mat4f model_tmp;
        mat4f normal_mat;
        
        model_tmp = mat4f((float)modelview_x[0],(float)modelview_x[1],(float)modelview_x[2],(float)modelview_x[3],(float)modelview_x[4],(float)modelview_x[5],(float)modelview_x[6],(float)modelview_x[7],(float)modelview_x[8],(float)modelview_x[9],(float)modelview_x[10],(float)modelview_x[11],(float)modelview_x[12],(float)modelview_x[13],(float)modelview_x[14],(float)modelview_x[15]);
        
        vec4f p = model_tmp * position;
        
        this->position = p(0,3)/p[3];

        glRotatef(-model->orientation[2], 0.0, 0.0, 1.0);
        glRotatef(-model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(-model->orientation[0], 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0],-model->position[1],-model->position[2]);
    }
}

void pointhdl::apply(string name, GLuint program)
{
    /* TODO Assignment 3: Pass all necessary uniforms to the shaders for point lights.
     */
    if (name == "white") {
        
        glUseProgram(program);
        
    } else {
        glUseProgram(program);
        string pos_str = pt + to_string(count) + "].position";
        string amb_str = pt + to_string(count) + "].ambient";
        string spec_str = pt + to_string(count) + "].specular";
        string dif_str = pt + to_string(count) + "].diffuse";
        string atten_str = pt + to_string(count) + "].attenuation";
        
        int pos_loc = glGetUniformLocation(program, pos_str.c_str());
        int amb_loc = glGetUniformLocation(program, amb_str.c_str());
        int spec_loc = glGetUniformLocation(program, spec_str.c_str());
        int dif_loc = glGetUniformLocation(program, dif_str.c_str());
        int atten_loc = glGetUniformLocation(program, atten_str.c_str());
        
        glUniform3f(pos_loc, position[0] ,position[1] ,position[2]);
        glUniform3f(amb_loc, ambient[0] ,ambient[1] ,ambient[2]);
        glUniform3f(spec_loc, specular[0], specular[1], specular[2]);
        glUniform3f(dif_loc, diffuse[0], diffuse[1],diffuse[2]);
        glUniform3f(atten_loc, attenuation[0] ,attenuation[1] ,attenuation[2]);
    }
}

spothdl::spothdl() : lighthdl(white*0.1f, white*0.5f, white)
{
    this->attenuation = vec3f(1.0, 0.14, 0.7);
    this->cutoff = 0.5;
    this->exponent = 1.0;
    type = "spot";
    count++;
}

spothdl::spothdl(const vec3f &attenuation, const float &cutoff, const float &exponent, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
    this->attenuation = attenuation;
    this->cutoff = cutoff;
    this->exponent = exponent;
    type = "spot";
    count++;
}

spothdl::~spothdl()
{
    count--;
}

void spothdl::update()
{
    if (model != NULL)
    {
        glTranslatef(model->position[0],model->position[1],model->position[2]);
        glRotatef(model->orientation[0], 1.0, 0.0, 0.0);
        glRotatef(model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(model->orientation[2], 0.0, 0.0, 1.0);
        
        vec4f position = vec4f(0.0, 0.0, 0.0, 1.0);
        vec4f direction = vec4f(0.0,0.0,-1.0,0.0);

        float modelview_x[16];
        
        glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, modelview_x);
        
        mat4f model_tmp;
        mat4f normal_mat;
        
        model_tmp = mat4f((float)modelview_x[0],(float)modelview_x[1],(float)modelview_x[2],(float)modelview_x[3],(float)modelview_x[4],(float)modelview_x[5],(float)modelview_x[6],(float)modelview_x[7],(float)modelview_x[8],(float)modelview_x[9],(float)modelview_x[10],(float)modelview_x[11],(float)modelview_x[12],(float)modelview_x[13],(float)modelview_x[14],(float)modelview_x[15]);
        
        vec4f p = model_tmp * position;
        
        this->position = p(0,3)/p[3];
        
        normal_mat = inverse(transpose(model_tmp));
        this->direction = normal_mat * direction;
        
        glRotatef(-model->orientation[2], 0.0, 0.0, 1.0);
        glRotatef(-model->orientation[1], 0.0, 1.0, 0.0);
        glRotatef(-model->orientation[0], 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0],-model->position[1],-model->position[2]);
    }
}

void spothdl::apply(string name, GLuint program)
{
    /* TODO Assignment 3: Pass all necessary uniforms to the shaders for spot lights.
     */
    if (name == "white") {
        
        glUseProgram(program);
        
    } else {
        glUseProgram(program);
        
        string dir_str = sp + to_string(count) + "].direction";
        string pos_str = sp + to_string(count) + "].position";
        string amb_str = sp + to_string(count) + "].ambient";
        string spec_str = sp + to_string(count) + "].specular";
        string dif_str = sp + to_string(count) + "].diffuse";
        string atten_str = sp + to_string(count) + "].attenuation";
        string cut_str = sp + to_string(count) + "].cutoff";
        string exp_str = sp + to_string(count) + "].exponent";
        
        int dir_loc = glGetUniformLocation(program, dir_str.c_str());
        int pos_loc = glGetUniformLocation(program, pos_str.c_str());
        int amb_loc = glGetUniformLocation(program, amb_str.c_str());
        int spec_loc = glGetUniformLocation(program, spec_str.c_str());
        int dif_loc = glGetUniformLocation(program, dif_str.c_str());
        int atten_loc = glGetUniformLocation(program, atten_str.c_str());
        int cut_loc = glGetUniformLocation(program, cut_str.c_str());
        int exp_loc = glGetUniformLocation(program, exp_str.c_str());
        
        glUniform3f(dir_loc, direction[0], direction[1],direction[2]);
        glUniform3f(pos_loc, position[0] ,position[1] ,position[2]);
        glUniform3f(amb_loc, ambient[0] ,ambient[1] ,ambient[2]);
        glUniform3f(spec_loc, specular[0], specular[1], specular[2]);
        glUniform3f(dif_loc, diffuse[0], diffuse[1],diffuse[2]);
        glUniform3f(atten_loc, attenuation[0] ,attenuation[1] ,attenuation[2]);
        glUniform1f(cut_loc, cutoff);
        glUniform1f(exp_loc, exponent);
        
    }
}
