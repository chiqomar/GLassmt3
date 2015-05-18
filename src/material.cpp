#include "material.h"
#include "light.h"
#include "lodepng.h"

GLuint whitehdl::vertex = 0;
GLuint whitehdl::fragment = 0;
GLuint whitehdl::program = 0;

GLuint phonghdl::vertex = 0;
GLuint phonghdl::fragment = 0;
GLuint phonghdl::program = 0;

GLuint gouraudhdl::vertex = 0;
GLuint gouraudhdl::fragment = 0;
GLuint gouraudhdl::program = 0;

GLuint brickhdl::vertex = 0;
GLuint brickhdl::fragment = 0;
GLuint brickhdl::program = 0;

GLuint texturehdl::vertex = 0;
GLuint texturehdl::fragment = 0;
GLuint texturehdl::program = 0;
GLuint texturehdl::texture = 0;

map<string,int> materialhdl::progmap;

extern string working_directory;

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

materialhdl::materialhdl()
{
	type = "material";
}

materialhdl::~materialhdl()
{
}

gouraudhdl::gouraudhdl()
{
	type = "gouraud";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 3: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/gouraud.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/gouraud.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("gouraud", program));
        cout << "Program number:" << program <<endl;
        printProgramInfoLog(program);
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint vok = 0;
        GLint fok = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &vok);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &fok);
        if (vok == GL_FALSE)
            cout << "Something in the vertex shader fed up";
        if (vok == GL_TRUE)
            cout << "gouraud V shader is good";
        if (fok == GL_FALSE)
            cout << "Something in the fragment shader fed up";
        if (vok == GL_TRUE)
            cout << "F shader is good";
        printShaderInfoLog(vertex);
        printShaderInfoLog(fragment);
        glLinkProgram(program);
        pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
	}
}

gouraudhdl::~gouraudhdl()
{

}

void gouraudhdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);
    
    int emission_location = glGetUniformLocation(program, "emission");
    int ambient_location = glGetUniformLocation(program, "ambient");
    int diffuse_location = glGetUniformLocation(program, "diffuse");
    int specular_location = glGetUniformLocation(program, "specular");
    int shininess_location = glGetUniformLocation(program, "shininess");
    
    glUniform3f(emission_location, emission[0], emission[1],emission[2]);
    glUniform3f(ambient_location, ambient[0] ,ambient[1] ,ambient[2]);
    glUniform3f(diffuse_location, diffuse[0], diffuse[1], diffuse[2]);
    glUniform3f(specular_location, specular[0], specular[1], specular[2]);
    glUniform1f(shininess_location, shininess);
    
    for (int j = 0; j < lights.size(); j++)
        if (lights[j] != NULL) {
            if (lights[j]->type == "directional")
                directionalhdl::count++;
            if (lights[j]->type == "spot")
                spothdl::count++;
            if (lights[j]->type == "point")
                pointhdl::count++;
            lights[j]->apply("gouraud", program);
        }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, dirnum_loc);
    glUniform1i(spotnum_loc, spotnum_loc);
    glUniform1i(ptnum_loc, ptnum_loc);
    
    directionalhdl::count = -1;
    spothdl::count = -1;
    pointhdl::count = -1;
}

materialhdl *gouraudhdl::clone() const
{
	gouraudhdl *result = new gouraudhdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

phonghdl::phonghdl()
{
	type = "phong";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 3: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/white.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/white.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("gouraud", program));
        printProgramInfoLog(program);
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint vok = 0;
        GLint fok = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &vok);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &fok);
        if (vok == GL_FALSE)
            cout << "Something in the vertex shader fed up";
        if (vok == GL_TRUE)
            cout << "phong V shader is good";
        if (fok == GL_FALSE)
            cout << "Something in the fragment shader fed up";
        if (vok == GL_TRUE)
            cout << "F shader is good";
        printShaderInfoLog(vertex);
        printShaderInfoLog(fragment);
        glLinkProgram(program);
        pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
	}
}

phonghdl::~phonghdl()
{

}

void phonghdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);

}

materialhdl *phonghdl::clone() const
{
	phonghdl *result = new phonghdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

whitehdl::whitehdl()
{
	type = "white";

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 3: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/white.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/white.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("white", program));
        cout << "Program number:" << program <<endl;
        printProgramInfoLog(program);
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint vok = 0;
        GLint fok = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &vok);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &fok);
        if (vok == GL_FALSE)
            cout << "Something in the vertex shader fed up";
        if (vok == GL_TRUE)
            cout << "white V shader is good";
        if (fok == GL_FALSE)
            cout << "Something in the fragment shader fed up";
        if (vok == GL_TRUE)
            cout << "F shader is good";
        printShaderInfoLog(vertex);
        printShaderInfoLog(fragment);
        glLinkProgram(program);
        pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
	}
}

whitehdl::~whitehdl()
{

}

void whitehdl::apply(const vector<lighthdl*> &lights)
{
	glUseProgram(program);
    int white_color_location = glGetUniformLocation(program, "white_color");
    glUniform4f(white_color_location, 0.0, 0.5, 0.5, 1.0);
}

materialhdl *whitehdl::clone() const
{
	whitehdl *result = new whitehdl();
	result->type = type;
	return result;
}

brickhdl::brickhdl()
{
	type = "brick";

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 3: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/white.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/white.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("brick", program));
        printProgramInfoLog(program);
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint vok = 0;
        GLint fok = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &vok);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &fok);
        if (vok == GL_FALSE)
            cout << "Something in the vertex shader fed up";
        if (vok == GL_TRUE)
            cout << "brick V shader is good";
        if (fok == GL_FALSE)
            cout << "Something in the fragment shader fed up";
        if (vok == GL_TRUE)
            cout << "F shader is good";
        printShaderInfoLog(vertex);
        printShaderInfoLog(fragment);
        glLinkProgram(program);
        pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
	}
}

brickhdl::~brickhdl()
{

}

void brickhdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);

}

materialhdl *brickhdl::clone() const
{
	brickhdl *result = new brickhdl();
	result->type = type;
	return result;
}

texturehdl::texturehdl()
{
	type = "texture";

	shininess = 1.0;

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 3: Load and link the shaders and load the texture Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/white.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/white.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("texture", program));
        printProgramInfoLog(program);
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        GLint vok = 0;
        GLint fok = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &vok);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &fok);
        if (vok == GL_FALSE)
            cout << "Something in the vertex shader fed up";
        if (vok == GL_TRUE)
            cout << "texture V shader is good";
        if (fok == GL_FALSE)
            cout << "Something in the fragment shader fed up";
        if (vok == GL_TRUE)
            cout << "F shader is good";
        printShaderInfoLog(vertex);
        printShaderInfoLog(fragment);
        glLinkProgram(program);
        pok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &pok);
        if (pok == GL_FALSE)
            cout << "program bad" << endl;
        if (pok == GL_TRUE)
            cout << "program good" << endl;
	}
}

texturehdl::~texturehdl()
{

}

void texturehdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);

}

materialhdl *texturehdl::clone() const
{
	texturehdl *result = new texturehdl();
	result->type = type;
	result->shininess = shininess;
	return result;
}
