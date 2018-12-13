#ifndef MULTIPLE_LIGHTS_SCENE_HPP
#define MULTIPLE_LIGHTS_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>


#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 4
#define NUM_SPOT_LIGHTS 4

class MultipleLightsScene : public Scene {
private:
    Shader* shader;
    Mesh* plane, *model;
    Camera* camera;
    FlyCameraController* controller;

    struct {
        bool enabled;
        glm::vec3 diffuse, specular, ambient, direction;
    } directionalLightProps[NUM_DIRECTIONAL_LIGHTS];

    struct {
        bool enabled;
        glm::vec3 diffuse, specular, ambient, position;
        float attenuation_quadratic, attenuation_linear, attenuation_constant;
    } pointLightProbs[NUM_POINT_LIGHTS];

    struct {
        bool enabled;
        glm::vec3 diffuse, specular, ambient, position, direction;
        float attenuation_quadratic, attenuation_linear, attenuation_constant, inner_cone, outer_cone;
    } spotLightProbs[NUM_SPOT_LIGHTS];

public:
    MultipleLightsScene(Application* app): Scene(app) {}

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif