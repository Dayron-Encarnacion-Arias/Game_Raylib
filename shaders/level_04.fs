#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Reactor: pulso radial desde el centro.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float p=.018*sin(length(fragTexCoord-vec2(.5))*35.0-time*3.0);finalColor=vec4(c.rgb+vec3(p,.002,0),c.a);}
