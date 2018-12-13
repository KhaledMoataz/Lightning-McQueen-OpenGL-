#version 330 core

in Interpolators {
    vec3 view;
    vec3 normal;
} fs_in;

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};
uniform Material material;

struct DirectionalLight {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 direction;
};
uniform DirectionalLight light;

out vec4 color;

float diffuse(vec3 n, vec3 l){
    //Diffuse (Lambert) term computation: reflected light = cosine the light incidence angle on the surface
    //max(0, ..) is used since light shouldn't be negative
    return max(0, dot(n,l));
}

float specular(vec3 n, vec3 l, vec3 v, float shininess){
    //Phong Specular term computation
    return pow(max(0, dot(v,reflect(-l, n))), shininess);
}

void main()
{
    vec3 n = normalize(fs_in.normal);
    vec3 v = normalize(fs_in.view);
    color = vec4(
        material.ambient*light.ambient + 
        material.diffuse*light.diffuse*diffuse(n, -light.direction) + 
        material.specular*light.specular*specular(n, -light.direction, v, material.shininess),
        1.0f
    );
}