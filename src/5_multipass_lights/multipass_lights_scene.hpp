#ifndef MULTIPASS_LIGHTS_SCENE_HPP
#define MULTIPASS_LIGHTS_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>


class MultiPassLightsScene : public Scene {
private:
    Shader* shaders[3];
    Mesh* plane, *model;
    Camera* camera;
    FlyCameraController* controller;

    enum struct LightType : int {
        DirectionalLight = 0,
        PointLight = 1,
        SpotLight = 2
    };

    struct Light {
        bool enabled;
        LightType type;   
        glm::vec3 diffuse, specular, ambient, position, direction;
        float attenuation_quadratic, attenuation_linear, attenuation_constant, inner_cone, outer_cone;
    };

    std::vector<Light> lights;
public:
    MultiPassLightsScene(Application* app): Scene(app) {}

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif