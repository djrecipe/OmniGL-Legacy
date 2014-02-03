#version 330

in vec3 normal;
in vec3 position;
in vec4 color;
in vec2 texcoord;
in mat4 srt_combined;

out vec4 myvertex;
out vec3 mynormal;
out vec4 Color;
out vec2 Texcoord;
out mat4 modelview;
out float fogFactor;
out vec3 eyepos_in;

uniform vec3 eyepos;
uniform mat4 pv_combined;
uniform float fog_start;
uniform float fog_end;
void main()
{
    Color=color;
    Texcoord = vec2(texcoord.x,texcoord.y);
    gl_Position = pv_combined*srt_combined*vec4(position.x,position.y,position.z,1.0f);
    fogFactor = min(max(length(gl_Position.xyz)-fog_start,0.0)/fog_end,1.0);
    myvertex=vec4(position.x,position.y,position.z,1.0f);
    mynormal=normal;
    modelview=srt_combined;
    eyepos_in=eyepos;
}