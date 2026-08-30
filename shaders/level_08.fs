#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Vacio: aberracion radial alrededor del centro.
void main(){vec2 p=fragTexCoord-.5;vec2 u=.5+p*(1.0+dot(p,p)*.012);vec4 c=texture(texture0,u)*fragColor;finalColor=vec4(c.rgb*vec3(.99,.985,1.025),c.a);}
