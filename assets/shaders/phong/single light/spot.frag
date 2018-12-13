#version 330 core

in Interpolators {
    vec3 world;
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

struct SpotLight {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 position;
    float attenuation_quadratic;
    float attenuation_linear;
    float attenuation_constant;
    vec3 direction;
    float inner_cone;
    float outer_cone;
};
uniform SpotLight light;

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
    vec3 l = light.position - fs_in.world;
    float d = length(l);
    l /= d;
    float angle = acos(dot(-l, light.direction));
    float attenuation = light.attenuation_constant +
                        light.attenuation_linear * d +
                        light.attenuation_quadratic * d * d;
    color = vec4(
        material.ambient*light.ambient + 
        (
            material.diffuse*light.diffuse*diffuse(n, l) + 
            material.specular*light.specular*specular(n, l, v, material.shininess)
        )/attenuation*smoothstep(light.outer_cone, light.inner_cone, angle),
        1.0f
    );
    //Notice that Attenuation and spot angle fading only affects diffuse and specular term
}