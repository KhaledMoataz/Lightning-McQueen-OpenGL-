#ifndef POINT_LIGHT_SCENE_HPP
#define POINT_LIGHT_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>

class PointLightScene : public Scene {
private:
    Shader* shader;
    Mesh* plane, *model;
    Camera* camera;
    FlyCameraController* controller;

    glm::vec3 lightPosition;

    GLuint mLoc, mitLoc, vpLoc, camPosLoc;
    struct {
        GLuint diffuse, specular, ambient, shininess;
    } materialVars;
    struct {
        GLuint diffuse, specular, ambient, position;
        GLuint attenuation_quadratic, attenuation_linear, attenuation_constant;
    } lightVars;
public:
    PointLightScene(Application* app): Scene(app) {}

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif