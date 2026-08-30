#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Fabrica: cuadricula digital transparente.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;vec2 q=fract(fragTexCoord*vec2(80,45));float g=(step(q.x,.035)+step(q.y,.035))*.012;finalColor=vec4(c.rgb+vec3(g,g*.25,g*.2),c.a);}
