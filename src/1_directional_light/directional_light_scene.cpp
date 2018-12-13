#include <application.hpp>
#include "directional_light_scene.hpp"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
*
* Simple scene with one directional light and multiple spheres with different specular properties
* Directional lights are mainly used for simulating sun light since they are parallel and have no position 
*
*/

void DirectionalLightScene::Initialize() {

	texShader = new Shader();
	texShader->attach("assets/shaders/texture.vert", GL_VERTEX_SHADER);
	texShader->attach("assets/shaders/texture.frag", GL_FRAGMENT_SHADER);
	texShader->link();

    shader = new Shader();
    shader->attach("assets/shaders/phong/single light/directional.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/single light/directional.frag", GL_FRAGMENT_SHADER);
    shader->link();


    mLoc = glGetUniformLocation(shader->getID(), "M");
    mitLoc = glGetUniformLocation(shader->getID(), "M_it");
    vpLoc = glGetUniformLocation(shader->getID(), "VP");
    camPosLoc = glGetUniformLocation(shader->getID(), "cam_pos");

	texLoc = glGetUniformLocation(texShader->getID(), "tex");
	mvpLoc = glGetUniformLocation(texShader->getID(), "MVP");

    materialVars.diffuse = glGetUniformLocation(shader->getID(), "material.diffuse");
    materialVars.specular = glGetUniformLocation(shader->getID(), "material.specular");
    materialVars.ambient = glGetUniformLocation(shader->getID(), "material.ambient");
    materialVars.shininess = glGetUniformLocation(shader->getID(), "material.shininess");
    
    lightVars.diffuse = glGetUniformLocation(shader->getID(), "light.diffuse");
    lightVars.specular = glGetUniformLocation(shader->getID(), "light.specular");
    lightVars.ambient = glGetUniformLocation(shader->getID(), "light.ambient");
    lightVars.direction = glGetUniformLocation(shader->getID(), "light.direction");
    
    //plane = MeshUtils::Plane({0,0}, {10,10});
    //model = MeshUtils::Sphere(48, 32);

	car = MeshUtils::LoadObj("assets/models/Cars/lightning-mcqueen-cars-2.obj");
	carTex = TextureUtils::Load2DTextureFromFile("assets/textures/light1_l0.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	road = MeshUtils::LoadObj("assets/models/Road/10563_RoadSectionStraight_v1-L3.obj");
	roadTex = TextureUtils::Load2DTextureFromFile("assets/textures/RoadSectionStraight.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    camera = new Camera();
    glm::ivec2 windowSize = getApplication()->getWindowSize();
    camera->setupPerspective(glm::pi<float>()/2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
    camera->setUp({0, 1, 0});

    controller = new FlyCameraController(this, camera);
    controller->setYaw(glm::half_pi<float>());
    controller->setPitch(-glm::quarter_pi<float>());
    controller->setPosition({0, 5, -5});

    lightYaw = lightPitch = glm::quarter_pi<float>();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.88f,0.68f,0.15f,0.0f);
}

void DirectionalLightScene::Update(double delta_time) {
    controller->update(delta_time);
	controller->setPosition({ camera->getPosition().r, camera->getPosition().g, camera->getPosition().b + (float)delta_time * 5 });
	CarPosition.z += (float)delta_time * 5;
	Keyboard* kb = getKeyboard();
	if (kb->isPressed(GLFW_KEY_KP_6)) CarPosition.x -= (float)delta_time * 8;
	if (kb->isPressed(GLFW_KEY_KP_4)) CarPosition.x += (float)delta_time * 8;
    /*Keyboard* kb = getKeyboard();

    float pitch_speed = 1.0f, yaw_speed = 1.0f;

    if(kb->isPressed(GLFW_KEY_I)) lightPitch += (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_K)) lightPitch -= (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_L)) lightYaw += (float)delta_time * yaw_speed;
    if(kb->isPressed(GLFW_KEY_J)) lightYaw -= (float)delta_time * yaw_speed;
    
    if(lightPitch < -glm::half_pi<float>()) lightPitch = -glm::half_pi<float>();
    if(lightPitch > glm::half_pi<float>()) lightPitch = glm::half_pi<float>();
    lightYaw = glm::wrapAngle(lightYaw);

	if (kb->isPressed(GLFW_KEY_KP_8)) CarPosition.z += (float)delta_time * 5;
	if (kb->isPressed(GLFW_KEY_KP_5)) CarPosition.z -= (float)delta_time * 5;
	if (kb->isPressed(GLFW_KEY_KP_6)) CarPosition.x -= (float)delta_time * 5;
	if (kb->isPressed(GLFW_KEY_KP_4)) CarPosition.x += (float)delta_time * 5;

	if (kb->isPressed(GLFW_KEY_KP_9)) CarRotation -= (float)delta_time * 2;
	if (kb->isPressed(GLFW_KEY_KP_7)) CarRotation += (float)delta_time * 2;
	CarRotation = glm::wrapAngle(CarRotation);*/
}

void DirectionalLightScene::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth

    glm::mat4 VP = camera->getVPMatrix();
    glm::vec3 cam_pos = camera->getPosition();
    glm::vec3 light_dir = -(glm::vec3(glm::cos(lightYaw), 0, glm::sin(lightYaw)) * glm::cos(lightPitch) + glm::vec3(0, glm::sin(lightPitch), 0));

    shader->use();
    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(VP));
    glUniform3f(camPosLoc, cam_pos.x, cam_pos.y, cam_pos.z);
    glUniform3f(lightVars.diffuse, 0.9f, 0.8f, 0.6f);
    glUniform3f(lightVars.specular, 0.9f, 0.8f, 0.6f);
    glUniform3f(lightVars.ambient, 0.88f, 0.68f, 0.15f);
    glUniform3f(lightVars.direction, light_dir.x, light_dir.y, light_dir.z);

    /*glm::mat4 ground_mat = glm::scale(glm::mat4(), glm::vec3(100, 1, 100));
    glm::mat4 ground_mat_it = glm::transpose(glm::inverse(ground_mat));
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(ground_mat));
    glUniformMatrix4fv(mitLoc, 1, GL_FALSE, glm::value_ptr(ground_mat_it));
    glUniform3f(materialVars.diffuse, 0.5f, 0.6f, 0.5f);
    glUniform3f(materialVars.specular, 0.2f, 0.2f, 0.2f);
    glUniform3f(materialVars.ambient, 0.5f, 0.6f, 0.5f);
    glUniform1f(materialVars.shininess, 50);
    plane->draw();*/

    /*glUniform3f(materialVars.diffuse, 0.7f, 0.2f, 0.1f);
    glUniform3f(materialVars.ambient, 0.7f, 0.2f, 0.1f);

    for(int i = 0; i <= 10; i++){
        for(int j = 0; j <= 10; j++){
            glm::mat4 model_mat = glm::translate(glm::mat4(), glm::vec3(i*4, 1, j*4));
            glm::mat4 model_mat_it = glm::transpose(glm::inverse(model_mat));
            glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model_mat));
            glUniformMatrix4fv(mitLoc, 1, GL_FALSE, glm::value_ptr(model_mat_it));
            float specular = j/10.0f;
            glUniform3f(materialVars.specular, specular, specular, specular);
            glUniform1f(materialVars.shininess, i * 5.0f + 1.0f);
            model->draw();
        }
    }*/

	texShader->use();
	carTex->bind();
	glm::mat4 tree_mat = glm::translate(glm::mat4(), { CarPosition.x ,1 ,CarPosition.z })
						*glm::rotate(glm::mat4(), -CarRotation, glm::vec3(0, 1, 0))
						*glm::translate(glm::mat4(), { 0,-1,0 })*glm::scale(glm::mat4(), { 1, 1, 1 });
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(VP * tree_mat));
	car->draw();

	
	roadTex->bind();
	glm::mat4 temp = glm::rotate(glm::mat4(), (float)glm::radians(-90.0f), { 1,0,0 })*
		glm::rotate(glm::mat4(), (float)glm::radians(-90.0f), { 0,0,1 })*
		glm::scale(glm::mat4(), glm::vec3(0.05f, 0.05f, 0.05f));
	for (int i = 0; i <= 10; i++) {
		tree_mat = glm::translate(glm::mat4(), { 0, -0.4f, 15 * i }) * temp;
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(VP * tree_mat));
		road->draw();
	}

}

void DirectionalLightScene::Finalize() {
    delete controller;
    delete camera;
    delete model;
    delete plane;
    delete shader;
	delete texShader;
	delete road;
	delete roadTex;
	delete carTex;
}