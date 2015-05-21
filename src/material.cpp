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

GLuint multitxhdl::vertex = 0;
GLuint multitxhdl::fragment = 0;
GLuint multitxhdl::program = 0;
GLuint multitxhdl::texture = 0;
GLuint multitxhdl::text2 = 0;

GLuint normmaphdl::vertex = 0;
GLuint normmaphdl::fragment = 0;
GLuint normmaphdl::program = 0;
GLuint normmaphdl::texture = 0;
GLuint normmaphdl::normalmap = 0;

GLuint bumpmaphdl::vertex = 0;
GLuint bumpmaphdl::fragment = 0;
GLuint bumpmaphdl::program = 0;
GLuint bumpmaphdl::texture = 0;
GLuint bumpmaphdl::normalmap = 0;

GLuint toonhdl::vertex = 0;
GLuint toonhdl::fragment = 0;
GLuint toonhdl::program = 0;

GLuint billboardhdl::vertex = 0;
GLuint billboardhdl::fragment = 0;
GLuint billboardhdl::program = 0;
GLuint billboardhdl::texture = 0;

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
        //cout << "Program number:" << program <<endl;
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
            lights[j]->apply("gouraud", program);
        }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
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
        vertex = load_shader_file("res/phong.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/phong.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("phong", program));
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
    
    for (int j = 0; j < lights.size(); j++) {
        
        if (lights[j] != NULL) {
            lights[j]->apply("phong", program);
        }
        
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);

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
        //cout << "Program number:" << program <<endl;
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
        vertex = load_shader_file("res/brick.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/brick.ft", GL_FRAGMENT_SHADER);
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
    
    // Find the locations of the vertex, normal, and texcoord variables in the shader
    int brick_size_location = glGetUniformLocation(program, "brick_size");
    int brick_color_location = glGetUniformLocation(program, "brick_color");
    int mortar_color_location = glGetUniformLocation(program, "mortar_color");
    int brick_pct_location = glGetUniformLocation(program, "brick_pct");
    
    // Pass in the parameters for the brick shader
    glUniform1f(brick_size_location, 0.1);
    glUniform3f(brick_color_location, 0.8, 0.0, 0.0);
    glUniform3f(mortar_color_location, 0.4, 0.4, 0.4);
    glUniform2f(brick_pct_location, 0.9, 0.9);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("brick", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
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
        vertex = load_shader_file("res/texture.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/texture.ft", GL_FRAGMENT_SHADER);
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
        

        unsigned width, height;
        vector<unsigned char> image;
        double u,v;
        loadtexture(width, height, image, u, v);
        
        glGenTextures(1, &texture);
        cout << "Image data:" << image.data()[0] << " " << image.data()[1] << endl;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
	}
}

texturehdl::~texturehdl()
{

}

void texturehdl::loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, double &u, double &v)
{
    // Load file and decode image.
    unsigned error = lodepng::decode(image, width, height, "res/img/rocks.png");
    
    // If there's an error, display it.
    if(error != 0)
    {
        cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    
}

void texturehdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint baseImageLoc = glGetUniformLocation(program, "texture");
    glUniform1i(baseImageLoc, 0);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("texture", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    

}

materialhdl *texturehdl::clone() const
{
	texturehdl *result = new texturehdl();
	result->type = type;
	result->shininess = shininess;
	return result;
}

multitxhdl::multitxhdl()
{
    type = "multitx";
    
    shininess = 1.0;
    
    if (vertex == 0 && fragment == 0 && program == 0)
    {
        /* TODO Assignment 3: Load and link the shaders and load the texture Keep in mind that vertex, fragment,
         * and program are static variables meaning they are *shared across all instances of
         * this class. So you only have to initialize them once when the first instance of
         * the class is created.
         */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/multitx.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/multitx.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("multitx", program));
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
            cout << "multitx V shader is good";
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
        
        
        unsigned width, height ,w2, h2;
        vector<unsigned char> image,other;
        loadtexture(width, height, image, "res/img/rocks.png");
        loadtexture(w2, h2, other, "res/img/multi.png");
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glGenTextures(1, &text2);
        glBindTexture(GL_TEXTURE_2D, text2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0, GL_RGBA, GL_UNSIGNED_BYTE, other.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

multitxhdl::~multitxhdl()
{
    
}

void multitxhdl::loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string filename)
{
    // Load file and decode image.
    unsigned error = lodepng::decode(image, width, height, filename.c_str());
    
    // If there's an error, display it.
    if(error != 0)
    {
        cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    
}



void multitxhdl::apply(const vector<lighthdl*> &lights)
{
    // TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint baseImageLoc = glGetUniformLocation(program, "texture");
    glUniform1i(baseImageLoc, 0);
    
    glActiveTexture(GL_TEXTURE1 + 0);
    glBindTexture(GL_TEXTURE_2D, text2);
    GLint secondImgLoc = glGetUniformLocation(program, "othertx");
    glUniform1i(secondImgLoc, 1);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("texture", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    
    
}

materialhdl *multitxhdl::clone() const
{
    multitxhdl *result = new multitxhdl();
    result->type = type;
    result->shininess = shininess;
    return result;
}

//void normmaphdl::computeTangentBasis(
//                                   // inputs
//                                   vec3f & vertices,
//                                   vec2f & uvs,
//                                   vec3f & normals,
//                                   // outputs
//                                   vec3f & tangents,
//                                   vec3f & bitangents)
//{
//for(int t = 0; t < 12; t++)
//{
//    int ia = t * 3, ib = ia + 1, ic = ib + 1;
//    
//    vec3 vdab = VertexArray[ib].Vertex - VertexArray[ia].Vertex;
//    vec3 vdac = VertexArray[ic].Vertex - VertexArray[ia].Vertex;
//    
//    vec2 tcdab = VertexArray[ib].TexCoord - VertexArray[ia].TexCoord;
//    vec2 tcdac = VertexArray[ic].TexCoord - VertexArray[ia].TexCoord;
//    
//    float r = 1.0f / (tcdab.x * tcdac.y - tcdab.y * tcdac.x);
//    
//    vec3 Normal = normalize(cross(vdab, vdac));
//    vec3 Tangent = normalize((vdab * tcdac.y  - vdac * tcdab.y) * r);
//    vec3 Bitangent = normalize((vdac * tcdab.x  - vdab * tcdac.x) * r);
//    
//    VertexArray[ia].Normal = Normal;
//    VertexArray[ia].Tangent = Tangent;
//    VertexArray[ia].Bitangent = Bitangent;
//    
//    VertexArray[ib].Normal = Normal;
//    VertexArray[ib].Tangent = Tangent;
//    VertexArray[ib].Bitangent = Bitangent;
//    
//    VertexArray[ic].Normal = Normal;
//    VertexArray[ic].Tangent = Tangent;
//    VertexArray[ic].Bitangent = Bitangent;
//}
//}

normmaphdl::normmaphdl()
{
    type = "normmap";
    
    shininess = 1.0;
    
    if (vertex == 0 && fragment == 0 && program == 0)
    {
        /* TODO Assignment 3: Load and link the shaders and load the texture Keep in mind that vertex, fragment,
         * and program are static variables meaning they are *shared across all instances of
         * this class. So you only have to initialize them once when the first instance of
         * the class is created.
         */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/normmap.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/normmap.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("normmap", program));
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
            cout << "normmap V shader is good";
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
        
        
        unsigned width, height ,w2, h2;
        vector<unsigned char> image,other;
        loadtexture(width, height, image, "res/img/rocks.png");
        loadtexture(w2, h2, other, "res/img/multi.png");
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);

    }
}

normmaphdl::~normmaphdl()
{
    
}

void normmaphdl::loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string filename)
{
    // Load file and decode image.
    unsigned error = lodepng::decode(image, width, height, filename.c_str());
    
    // If there's an error, display it.
    if(error != 0)
    {
        cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    
}

void normmaphdl::apply(const vector<lighthdl*> &lights)
{
    // TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint baseImageLoc = glGetUniformLocation(program, "texture");
    glUniform1i(baseImageLoc, 0);
    
    glActiveTexture(GL_TEXTURE1 + 0);
    glBindTexture(GL_TEXTURE_2D, normalmap);
    GLint secondImgLoc = glGetUniformLocation(program, "othertx");
    glUniform1i(secondImgLoc, 1);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("texture", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    
    
}

materialhdl *normmaphdl::clone() const
{
    normmaphdl *result = new normmaphdl();
    result->type = type;
    result->shininess = shininess;
    return result;
}

bumpmaphdl::bumpmaphdl()
{
    type = "bumpmap";
    
    shininess = 1.0;
    
    if (vertex == 0 && fragment == 0 && program == 0)
    {
        /* TODO Assignment 3: Load and link the shaders and load the texture Keep in mind that vertex, fragment,
         * and program are static variables meaning they are *shared across all instances of
         * this class. So you only have to initialize them once when the first instance of
         * the class is created.
         */
        glEnable(GL_DEPTH_TEST);
        vertex = load_shader_file("res/bumpmap.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/bumpmap.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("bumpmap", program));
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
            cout << "normmap V shader is good";
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
        
        
        unsigned width, height ,w2, h2;
        vector<unsigned char> image,other;
        loadtexture(width, height, image, "res/img/chest.png");
        loadtexture(w2, h2, other, "res/img/chest_nm.png");
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glGenTextures(1, &normalmap);
        glBindTexture(GL_TEXTURE_2D, normalmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0, GL_RGBA, GL_UNSIGNED_BYTE, other.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        
        
    }
}

bumpmaphdl::~bumpmaphdl()
{
    
}

void bumpmaphdl::loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string filename)
{
    // Load file and decode image.
    unsigned error = lodepng::decode(image, width, height, filename.c_str());
    
    // If there's an error, display it.
    if(error != 0)
    {
        cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    
}

void bumpmaphdl::apply(const vector<lighthdl*> &lights)
{
    // TODO Assignment 3: Apply the shader program and pass it the necessary uniform values
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    GLint baseImageLoc = glGetUniformLocation(program, "texture");
    glUniform1i(baseImageLoc, 0);
    
    glActiveTexture(GL_TEXTURE1 + 0);
    glBindTexture(GL_TEXTURE_2D, normalmap);
    glEnable(GL_TEXTURE_2D);
    GLint secondImgLoc = glGetUniformLocation(program, "normalmap");
    glUniform1i(secondImgLoc, 1);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("bumpmap", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    
    
}

materialhdl *bumpmaphdl::clone() const
{
    bumpmaphdl *result = new bumpmaphdl();
    result->type = type;
    result->shininess = shininess;
    return result;
}

toonhdl::toonhdl()
{
    type = "toon";
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
        vertex = load_shader_file("res/toon.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/toon.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("toon", program));
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
            cout << "toon V shader is good";
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

toonhdl::~toonhdl()
{
    
}

void toonhdl::apply(const vector<lighthdl*> &lights)
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
    
    for (int j = 0; j < lights.size(); j++) {
        
        if (lights[j] != NULL) {
            lights[j]->apply("toon", program);
        }
        
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    
}

materialhdl *toonhdl::clone() const
{
    toonhdl *result = new toonhdl();
    result->type = type;
    result->emission = emission;
    result->ambient = ambient;
    result->diffuse = diffuse;
    result->specular = specular;
    result->shininess = shininess;
    return result;
}

billboardhdl::billboardhdl()
{
    type = "billboard";
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
        vertex = load_shader_file("res/billboard.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file("res/billboard.ft", GL_FRAGMENT_SHADER);
        program = glCreateProgram();
        progmap.insert(pair<string, int>("toon", program));
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
            cout << "billboard V shader is good";
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
        
        unsigned width, height;
        vector<unsigned char> image;
        loadtexture(width, height, image, "res/img/tree.png");
        
        glGenTextures(1, &texture);
        cout << "Image data:" << image.data()[0] << " " << image.data()[1] << endl;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

billboardhdl::~billboardhdl()
{
    
}

void billboardhdl::loadtexture(unsigned &width, unsigned &height, vector<unsigned char> &image, string filename)
{
    // Load file and decode image.
    unsigned error = lodepng::decode(image, width, height, filename.c_str());
    
    // If there's an error, display it.
    if(error != 0)
    {
        cout << "error " << error << ": " << lodepng_error_text(error) << endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    
}

void billboardhdl::apply(const vector<lighthdl*> &lights)
{
    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint baseImageLoc = glGetUniformLocation(program, "texture");
    glUniform1i(baseImageLoc, 0);
    
    // Draw the triangles
    for (int j = 0; j < lights.size(); j++) {
        if (lights[j] != NULL)
            lights[j]->apply("billboard", program);
    }
    int dirnum_loc = glGetUniformLocation(program, "dirNum");
    int spotnum_loc = glGetUniformLocation(program, "spotNum");
    int ptnum_loc = glGetUniformLocation(program, "ptNum");
    glUniform1i(dirnum_loc, directionalhdl::count);
    glUniform1i(spotnum_loc, spothdl::count);
    glUniform1i(ptnum_loc, pointhdl::count);
    
}

materialhdl *billboardhdl::clone() const
{
    billboardhdl *result = new billboardhdl();
    result->type = type;
    result->emission = emission;
    result->ambient = ambient;
    result->diffuse = diffuse;
    result->specular = specular;
    result->shininess = shininess;
    return result;
}
