#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Catedral: rayos verticales semejantes a vitrales.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float r=pow(max(0.0,sin(fragTexCoord.x*30.0+time*.25)),18.0)*.035;finalColor=vec4(c.rgb+vec3(r,r*.25,r*.4),c.a);}
