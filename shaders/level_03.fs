#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Neon: separacion RGB animada.
void main(){vec2 u=fragTexCoord;float k=.0012+.0005*sin(time*2.0);vec4 c=texture(texture0,u);finalColor=vec4(texture(texture0,u+vec2(k,0)).r,c.g,texture(texture0,u-vec2(k,0)).b,c.a)*fragColor;}
