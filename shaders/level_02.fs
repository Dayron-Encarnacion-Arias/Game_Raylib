#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Fundicion: calor que ondula la parte inferior.
void main(){vec2 uv=fragTexCoord;uv.x+=sin(uv.y*38.0+time*2.0)*0.0018*smoothstep(.35,1.0,uv.y);vec4 c=texture(texture0,uv)*fragColor;finalColor=vec4(c.r*1.025,c.g*.99,c.b*.97,c.a);}
