#include <application.hpp>
#include "spot_light_scene.hpp"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
*
* Simple scene with one spot light and multiple spheres with different specular properties
* Spot lights are useful for simulating torches, street light, etc.
*
*/

void SpotLightScene::Initialize() {
    shader = new Shader();
    shader->attach("assets/shaders/phong/single light/spot.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/single light/spot.frag", GL_FRAGMENT_SHADER);
    shader->link();

    mLoc = glGetUniformLocation(shader->getID(), "M");
    mitLoc = glGetUniformLocation(shader->getID(), "M_it");
    vpLoc = glGetUniformLocation(shader->getID(), "VP");
    camPosLoc = glGetUniformLocation(shader->getID(), "cam_pos");
    
    materialVars.diffuse = glGetUniformLocation(shader->getID(), "material.diffuse");
    materialVars.specular = glGetUniformLocation(shader->getID(), "material.specular");
    materialVars.ambient = glGetUniformLocation(shader->getID(), "material.ambient");
    materialVars.shininess = glGetUniformLocation(shader->getID(), "material.shininess");
    
    lightVars.diffuse = glGetUniformLocation(shader->getID(), "light.diffuse");
    lightVars.specular = glGetUniformLocation(shader->getID(), "light.specular");
    lightVars.ambient = glGetUniformLocation(shader->getID(), "light.ambient");
    lightVars.position = glGetUniformLocation(shader->getID(), "light.position");
    lightVars.attenuation_quadratic = glGetUniformLocation(shader->getID(), "light.attenuation_quadratic");
    lightVars.attenuation_linear = glGetUniformLocation(shader->getID(), "light.attenuation_linear");
    lightVars.attenuation_constant = glGetUniformLocation(shader->getID(), "light.attenuation_constant");
    lightVars.direction = glGetUniformLocation(shader->getID(), "light.direction");
    lightVars.inner_cone = glGetUniformLocation(shader->getID(), "light.inner_cone");
    lightVars.outer_cone = glGetUniformLocation(shader->getID(), "light.outer_cone");
    
    plane = MeshUtils::Plane({0,0}, {10,10});
    model = MeshUtils::Sphere(48, 32);

    camera = new Camera();
    glm::ivec2 windowSize = getApplication()->getWindowSize();
    camera->setupPerspective(glm::pi<float>()/2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
    camera->setUp({0, 1, 0});

    controller = new FlyCameraController(this, camera);
    controller->setYaw(-glm::half_pi<float>());
    controller->setPitch(-glm::quarter_pi<float>());
    controller->setPosition({20, 10, 50});

    lightPosition = {20, 1, 20};
    lightYaw = lightPitch = -glm::half_pi<float>();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.09f,0.07f,0.02f,0.0f);
}

void SpotLightScene::Update(double delta_time) {
    controller->update(delta_time);
    Keyboard* kb = getKeyboard();

    float speed = 5.0f;

    if(kb->isPressed(GLFW_KEY_I)) lightPosition.z += (float)delta_time * speed;
    if(kb->isPressed(GLFW_KEY_K)) lightPosition.z -= (float)delta_time * speed;
    if(kb->isPressed(GLFW_KEY_L)) lightPosition.x += (float)delta_time * speed;
    if(kb->isPressed(GLFW_KEY_J)) lightPosition.x -= (float)delta_time * speed;
    if(kb->isPressed(GLFW_KEY_U)) lightPosition.y += (float)delta_time * speed;
    if(kb->isPressed(GLFW_KEY_O)) lightPosition.y -= (float)delta_time * speed;

    float pitch_speed = 1.0f, yaw_speed = 1.0f;

    if(kb->isPressed(GLFW_KEY_T)) lightPitch += (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_G)) lightPitch -= (float)delta_time * pitch_speed;
    if(kb->isPressed(GLFW_KEY_H)) lightYaw += (float)delta_time * yaw_speed;
    if(kb->isPressed(GLFW_KEY_F)) lightYaw -= (float)delta_time * yaw_speed;
    
    if(lightPitch < -glm::half_pi<float>()) lightPitch = -glm::half_pi<float>();
    if(lightPitch > glm::half_pi<float>()) lightPitch = glm::half_pi<float>();
    lightYaw = glm::wrapAngle(lightYaw);
}

void SpotLightScene::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth

    glm::mat4 VP = camera->getVPMatrix();
    glm::vec3 cam_pos = camera->getPosition();
    glm::vec3 light_dir = glm::vec3(glm::cos(lightYaw), 0, glm::sin(lightYaw)) * glm::cos(lightPitch) + glm::vec3(0, glm::sin(lightPitch), 0);

    shader->use();
    glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(VP));
    glUniform3f(camPosLoc, cam_pos.x, cam_pos.y, cam_pos.z);
    glUniform3f(lightVars.diffuse, 0.9f, 0.8f, 0.6f);
    glUniform3f(lightVars.specular, 0.9f, 0.8f, 0.6f);
    glUniform3f(lightVars.ambient, 0.09f, 0.07f, 0.02f);
    glUniform3f(lightVars.position, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform1f(lightVars.attenuation_quadratic, 0.0f);
    glUniform1f(lightVars.attenuation_linear, 1);
    glUniform1f(lightVars.attenuation_constant, 0.1f);
    glUniform3f(lightVars.direction, light_dir.x, light_dir.y, light_dir.z);
    glUniform1f(lightVars.inner_cone, glm::pi<float>()/4);
    glUniform1f(lightVars.outer_cone, glm::pi<float>()/3);

    glm::mat4 ground_mat = glm::scale(glm::mat4(), glm::vec3(100, 1, 100));
    glm::mat4 ground_mat_it = glm::transpose(glm::inverse(ground_mat));
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(ground_mat));
    glUniformMatrix4fv(mitLoc, 1, GL_FALSE, glm::value_ptr(ground_mat_it));
    glUniform3f(materialVars.diffuse, 0.5f, 0.6f, 0.5f);
    glUniform3f(materialVars.specular, 0.2f, 0.2f, 0.2f);
    glUniform3f(materialVars.ambient, 0.5f, 0.6f, 0.5f);
    glUniform1f(materialVars.shininess, 50);
    plane->draw();

    glUniform3f(materialVars.diffuse, 0.7f, 0.2f, 0.1f);
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
    }

}

void SpotLightScene::Finalize() {
    delete controller;
    delete camera;
    delete model;
    delete plane;
    delete shader;
}