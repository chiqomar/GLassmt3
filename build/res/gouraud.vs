#version 120

struct direction {
vec3 direction;
vec3 ambient;
vec3 specular;
vec3 diffuse;
};

struct point {
vec3 position;
vec3 ambient;
vec3 specular;
vec3 diffuse;
vec3 attenuation;
};

struct spot {
vec3 position;
vec3 direction;
vec3 ambient;
vec3 specular;
vec3 diffuse;
vec3 attenuation;
float cutoff;
float exponent;
};

//UNIFORMS START
uniform int dirNum;
uniform int ptNum;
uniform int spotNum;
uniform direction di[3];
uniform point pt[3];
uniform spot sp[3];

void direction_light(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess, direction dire)
{
	vec3 eye_direction = -vertex;
	float eye_distance = length(eye_direction);
	eye_direction /= eye_distance;

	vec3 half_vector = normalize(dire.direction + eye_direction);

	float normal_o_light_direction = max(0.0, dot(normal, dire.direction));
	float normal_o_half_vector = max(0.0, dot(normal, half_vector));

	float power_factor = 0.0;
	if (normal_o_light_direction > 0.0)
		power_factor = pow(normal_o_half_vector, shininess);

	ambient += dire.ambient;
	diffuse += dire.diffuse*normal_o_light_direction;
	specular += dire.specular*power_factor;
}




void point_light(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess, point pnt)
{
	vec3 light_direction = pnt.position - vertex;
	float light_distance = length(light_direction);
	light_direction /= light_distance;

	vec3 eye_direction = -vertex;
	float eye_distance = length(eye_direction);
	eye_direction /= eye_distance;

	float att = 1.0/(pnt.attenuation[0] + pnt.attenuation[1]*light_distance + pnt.attenuation[2]*light_distance*light_distance);

	vec3 half_vector = normalize(light_direction + eye_direction);

	float normal_o_light_direction = max(0.0f, dot(normal, light_direction));
	float normal_o_half_vector = max(0.0f, dot(normal, half_vector));

	float power_factor = 0.0;
	if (normal_o_light_direction > 0.0)
		power_factor = pow(normal_o_half_vector, shininess);

	ambient += pnt.ambient*att;
	diffuse += pnt.diffuse*normal_o_light_direction*att;
	specular += pnt.specular*power_factor*att;
}


void spot_light(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess, spot spt)
{
	vec3 light_direction = spt.position - vertex;
	float light_distance = length(light_direction);
	light_direction /= light_distance;

	vec3 eye_direction = -vertex;
	float eye_distance = length(eye_direction);
	eye_direction /= eye_distance;

	float att = 1.0/(spt.attenuation[0] + spt.attenuation[1]*light_distance + spt.attenuation[2]*light_distance*light_distance);
	float spotdot = dot(-light_direction, spt.direction);

	float spotatt = 0.0;
	if (spotdot >= spt.cutoff)
		spotatt = pow(spotdot, spt.exponent);

	att *= spotatt;

	vec3 half_vector = normalize(light_direction + normalize(-vertex));
	float normal_o_light_direction = max(0.0f, dot(normal, light_direction));
	float normal_o_half_vector = max(0.0f, dot(normal, half_vector));

	float power_factor = 0.0;
	if (normal_o_light_direction > 0.0)
		power_factor = pow(normal_o_half_vector, shininess);

	ambient += spt.ambient*att;
	diffuse += spt.diffuse*normal_o_light_direction*att;
	specular += spt.specular*power_factor*att;
}

vec3 shadeit(vec3 em, vec3 amb, vec3 dif, vec3 spec, float shiny, vec3 vertex, vec3 normal) {
  vec3 ambLight = vec3(0.0, 0.0, 0.0);
	vec3 difLight = vec3(0.0, 0.0, 0.0);
	vec3 specLight = vec3(0.0, 0.0, 0.0);

  for (int i = 0; i < dirNum; i++) {
    direction_light(ambLight,difLight,specLight,vertex,normal,shiny,di[i]);
}
for (int i = 0; i < ptNum; i++) {
  point_light(ambLight,difLight,specLight,vertex,normal,shiny,pt[i]);
}
for (int i = 0; i < spotNum; i++) {
  spot_light(ambLight,difLight,specLight,vertex,normal,shiny,sp[i]);
}
vec3 lights = em + amb * ambLight + dif * difLight + spec * specLight;

return clamp(lights,0.0,1.0);

}

uniform vec3 emission;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
varying vec4 color;

void main()
{
  vec3 eye_space_normal = gl_NormalMatrix * gl_Normal;
  vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
  vec3 eye_space_vertex = vertex.xyz;
  color = vec4(shadeit(emission,ambient,diffuse,specular,shininess,eye_space_vertex,eye_space_normal),1.0);
  //color = vec4(1.0,3.0,1.0,0.6);
    gl_Position = gl_ProjectionMatrix * vertex;
}
