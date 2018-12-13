#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

uniform mat4 M;

uniform mat4 vView;
uniform mat4 Proj;
out vec2 vUV;

out float visibility;				//visibility=exp(-distance*density)exp(gradient);
//uniform mat4 MVP; 
const float denisty =0.07;
const float gradient=1;
void main()
{
	vUV = uv;
	vec4 world = M * vec4(position, 1.0f);
	vec4 PositionRelativeToCam=vView*world;
	gl_Position = Proj * PositionRelativeToCam;
	float distance =length(PositionRelativeToCam.xyz);
	visibility=exp(-pow((distance*denisty),gradient));
	visibility=clamp(visibility,0.0,1.0);				//stay between 0,1;


    //gl_Position = MVP * vec4(position, 1.0f);
}


