#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Tormenta: destello ocasional que no oculta el tablero.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float f=pow(max(0.0,sin(time*.83)),32.0)*.11;finalColor=vec4(c.rgb+vec3(f*.7,f*.75,f),c.a);}
