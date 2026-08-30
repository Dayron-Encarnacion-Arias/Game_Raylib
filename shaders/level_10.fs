#version 330
in vec2 fragTexCoord; in vec4 fragColor; out vec4 finalColor;
uniform sampler2D texture0; uniform float time; uniform vec2 resolution;
// Corona: vignette dinamica y pulso rojo final.
void main(){vec4 c=texture(texture0,fragTexCoord)*fragColor;float v=1.0-smoothstep(.38,.78,length(fragTexCoord-.5))*.18;float p=.012*(sin(time*2.0)+1.0);finalColor=vec4(c.rgb*v+vec3(p,p*.08,p*.1),c.a);}
