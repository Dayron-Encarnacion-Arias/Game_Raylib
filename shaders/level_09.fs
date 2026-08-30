#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Abismo: niebla fria desde el borde inferior.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float f=smoothstep(.72,1.0,fragTexCoord.y)*(.025+.012*sin(fragTexCoord.x*25.0+time));finalColor=vec4(c.rgb+vec3(0,f*.35,f),c.a);}
