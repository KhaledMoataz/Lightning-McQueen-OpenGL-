#ifndef DIRECTIONAL_LIGHT_SCENE_HPP
#define DIRECTIONAL_LIGHT_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>
#include <queue>

class DirectionalLightScene : public Scene {
private:
	glm::mat4 trans;
	Shader* shader; //,* texShader;
    Mesh* plane, *model, *car, *road, *coin;
    Camera* camera;
    FlyCameraController* controller;
	Texture2D *carTex, *roadTex, *coinTex;

    float lightYaw, lightPitch;

	glm::vec3 CarPosition;
	float CarRotation;
	int roadPos;
	std::queue<glm::vec3> coinPositions;
	float time = 0;
	int second = 0;
	float lastPosition = 0;
	bool colorTransformation = false;
	int startRoad = 0;

	GLuint mLoc, vLoc, pLoc, mitLoc, camPosLoc, texLoc, scLoc, transLoc;// mvpLoc,vpLoc
    struct {
        GLuint diffuse, specular, ambient, shininess;
    } materialVars;
    struct {
        GLuint diffuse, specular, ambient, direction;
    } lightVars;
public:
	DirectionalLightScene(Application* app) : Scene(app) { CarRotation = 0; roadPos = 0; }

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif