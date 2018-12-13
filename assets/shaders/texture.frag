#version 330 core

out vec4 color;


in vec2 vUV;
in float visibility;
uniform vec3 skyColour;
uniform sampler2D tex;
void main()
{
    //function texture take a "sampler" and "uv" then return a vec4 containing the sampled color
	color = texture(tex, vUV);
	color=mix(vec4(skyColour,1.0),color,visibility);

}