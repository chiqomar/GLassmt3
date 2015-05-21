#version 120
uniform sampler2D texture; // color map
varying texcoord;
 
void main()
{

    vec3 norm = normalize(eye_space_normal);
  vec3 empty = vec3(0.0,0.0,0.0);
  vec4 color = texture2D(texture, texcoord).rgba;
 gl_FragColor = vec4(shadeit(empty,color,color,color,color.x,eye_space_vertex,norm),1.0);
  

}