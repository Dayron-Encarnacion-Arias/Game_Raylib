#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Subsuelo: lineas de monitor muy suaves.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float scan=sin(fragTexCoord.y*resolution.y*1.2)*0.018;finalColor=vec4(c.rgb-scan,c.a);}
