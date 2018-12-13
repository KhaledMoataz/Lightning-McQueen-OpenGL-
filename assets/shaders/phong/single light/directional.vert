#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

uniform mat4 M;
uniform mat4 M_it;

//uniform mat4 VP;
uniform mat4 vView;
uniform mat4 Proj;

uniform vec3 cam_pos;
out vec2 vUV;
out float visibility;				//visibility=exp(-distance*density)exp(gradient); 
const float denisty =0.07;
const float gradient=1;
out Interpolators {
    vec3 view;
    vec3 normal;
} vs_out;

void main()
{
	vUV = uv;
    vec4 world = M * vec4(position, 1.0f);
    vs_out.view = cam_pos - world.xyz;
    vs_out.normal = normalize(M_it * vec4(normal, 0.0f)).xyz;
	vec4 PositionRelativeToCam=vView*world;
	gl_Position = Proj * PositionRelativeToCam;
	float distance =length(PositionRelativeToCam.xyz);
	visibility=exp(-pow((distance*denisty),gradient));
	visibility=clamp(visibility,0.0,1.0);				//stay between 0,1;
    //gl_Position = VP * world;
}