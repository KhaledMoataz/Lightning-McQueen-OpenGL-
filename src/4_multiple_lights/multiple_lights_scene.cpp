#include <application.hpp>
#include "multiple_lights_scene.hpp"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
*
* This scene shows one option to support multiple lights in one scene which is to create one big shader
* that calculates all the lights in one draw call
*
*/

void MultipleLightsScene::Initialize() {
    shader = new Shader();
    shader->attach("assets/shaders/phong/multiple lights/lights.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/multiple lights/lights.frag", GL_FRAGMENT_SHADER);
    shader->link();

    directionalLightProps[0].enabled = true;
    directionalLightProps[0].diffuse = {0.45f, 0.4f, 0.3f};
    directionalLightProps[0].specular = {0.45f, 0.4f, 0.3f};
    directionalLightProps[0].ambient = {0.44f, 0.34f, 0.075f};
    directionalLightProps[0].direction = glm::normalize(glm::vec3(-1, -1, -1));

    pointLightProbs[0].enabled = true;
    pointLightProbs[0].diffuse = {1.0f, 0.3f, 0.3f};
    pointLightProbs[0].specular = {1.0f, 0.3f, 0.3f};
    pointLightProbs[0].ambient = {0, 0, 0};
    pointLightProbs[0].position = {2, 1, 2};
    pointLightProbs[0].attenuation_quadratic = 0.0f;
    pointLightProbs[0].attenuation_linear = 1.0f;
    pointLightProbs[0].attenuation_constant = 0.01f;
    
    pointLightProbs[1].enabled = true;
    pointLightProbs[1].diffuse = {0.3f, 1.0f, 0.3f};
    pointLightProbs[1].specular = {0.3f, 1.0f, 0.3f};
    pointLightProbs[1].ambient = {0, 0, 0};
    pointLightProbs[1].position = {38, 1, 2};
    pointLightProbs[1].attenuation_quadratic = 0.0f;
    pointLightProbs[1].attenuation_linear = 1.0f;
    pointLightProbs[1].attenuation_constant = 0.01f;

    pointLightProbs[2].enabled = true;
    pointLightProbs[2].diffuse = {0.3f, 0.3f, 1.0f};
    pointLightProbs[2].specular = {0.3f, 0.3f, 1.0f};
    pointLightProbs[2].ambient = {0, 0, 0};
    pointLightProbs[2].position = {38, 1, 38};
    pointLightProbs[2].attenuation_quadratic = 0.0f;
    pointLightProbs[2].attenuation_linear = 1.0f;
    pointLightProbs[2].attenuation_constant = 0.01f;

    pointLightProbs[3].enabled = true;
    pointLightProbs[3].diffuse = {0.6f, 0.6f, 0.3f};
    pointLightProbs[3].specular = {0.6f, 0.6f, 0.3f};
    pointLightProbs[3].ambient = {0, 0, 0};
    pointLightProbs[3].position = {2, 1, 38};
    pointLightProbs[3].attenuation_quadratic = 0.0f;
    pointLightProbs[3].attenuation_linear = 1.0f;
    pointLightProbs[3].attenuation_constant = 0.01f;

    spotLightProbs[0].enabled = true;
    spotLightProbs[0].diffuse = {1.0f, 0.3f, 0.3f};
    spotLightProbs[0].specular = {1.0f, 0.3f, 0.3f};
    spotLightProbs[0].ambient = {0, 0, 0};
    spotLightProbs[0].position = {6, 1, 6};
    spotLightProbs[0].attenuation_quadratic = 0.0f;
    spotLightProbs[0].attenuation_linear = 1.0f;
    spotLightProbs[0].attenuation_constant = 0.01f;
    spotLightProbs[0].direction = glm::normalize(glm::vec3(1,-1,1));
    spotLightProbs[0].inner_cone = glm::pi<float>()/4;
    spotLightProbs[0].outer_cone = glm::pi<float>()/3;
    
    spotLightProbs[1].enabled = true;
    spotLightProbs[1].diffuse = {0.3f, 1.0f, 0.3f};
    spotLightProbs[1].specular = {0.3f, 1.0f, 0.3f};
    spotLightProbs[1].ambient = {0, 0, 0};
    spotLightProbs[1].position = {34, 1, 6};
    spotLightProbs[1].attenuation_quadratic = 0.0f;
    spotLightProbs[1].attenuation_linear = 1.0f;
    spotLightProbs[1].attenuation_constant = 0.01f;
    spotLightProbs[1].direction = glm::normalize(glm::vec3(-1,-1,1));
    spotLightProbs[1].inner_cone = glm::pi<float>()/4;
    spotLightProbs[1].outer_cone = glm::pi<float>()/3;

    spotLightProbs[2].enabled = true;
    spotLightProbs[2].diffuse = {0.3f, 0.3f, 1.0f};
    spotLightProbs[2].specular = {0.3f, 0.3f, 1.0f};
    spotLightProbs[2].ambient = {0, 0, 0};
    spotLightProbs[2].position = {34, 1, 34};
    spotLightProbs[2].attenuation_quadratic = 0.0f;
    spotLightProbs[2].attenuation_linear = 1.0f;
    spotLightProbs[2].attenuation_constant = 0.01f;
    spotLightProbs[2].direction = glm::normalize(glm::vec3(-1,-1,-1));
    spotLightProbs[2].inner_cone = glm::pi<float>()/4;
    spotLightProbs[2].outer_cone = glm::pi<float>()/3;

    spotLightProbs[3].enabled = true;
    spotLightProbs[3].diffuse = {0.6f, 0.6f, 0.3f};
    spotLightProbs[3].specular = {0.6f, 0.6f, 0.3f};
    spotLightProbs[3].ambient = {0, 0, 0};
    spotLightProbs[3].position = {6, 1, 34};
    spotLightProbs[3].attenuation_quadratic = 0.0f;
    spotLightProbs[3].attenuation_linear = 1.0f;
    spotLightProbs[3].attenuation_constant = 0.01f;
    spotLightProbs[3].direction = glm::normalize(glm::vec3(1,-1,-1));
    spotLightProbs[3].inner_cone = glm::pi<float>()/4;
    spotLightProbs[3].outer_cone = glm::pi<float>()/3;
    
    plane = MeshUtils::Plane({0,0}, {10,10});
    model = MeshUtils::Sphere(48, 32);

    camera = new Camera();
    glm::ivec2 windowSize = getApplication()->getWindowSize();
    camera->setupPerspective(glm::pi<float>()/2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
    camera->setUp({0, 1, 0});

    controller = new FlyCameraController(this, camera);
    controller->setYaw(-glm::half_pi<float>());
    controller->setPitch(-glm::pi<float>()/3);
    controller->setPosition({20, 20, 40});

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.44f, 0.34f, 0.075f, 0.0f);
}

void MultipleLightsScene::Update(double delta_time) {
    controller->update(delta_time);
    Keyboard* kb = getKeyboard();

    if(kb->justPressed(GLFW_KEY_0)) directionalLightProps[0].enabled = !directionalLightProps[0].enabled;
    if(kb->justPressed(GLFW_KEY_1)) pointLightProbs[0].enabled = !pointLightProbs[0].enabled;
    if(kb->justPressed(GLFW_KEY_2)) pointLightProbs[1].enabled = !pointLightProbs[1].enabled;
    if(kb->justPressed(GLFW_KEY_3)) pointLightProbs[2].enabled = !pointLightProbs[2].enabled;
    if(kb->justPressed(GLFW_KEY_4)) pointLightProbs[3].enabled = !pointLightProbs[3].enabled;
    if(kb->justPressed(GLFW_KEY_5)) spotLightProbs[0].enabled = !spotLightProbs[0].enabled;
    if(kb->justPressed(GLFW_KEY_6)) spotLightProbs[1].enabled = !spotLightProbs[1].enabled;
    if(kb->justPressed(GLFW_KEY_7)) spotLightProbs[2].enabled = !spotLightProbs[2].enabled;
    if(kb->justPressed(GLFW_KEY_8)) spotLightProbs[3].enabled = !spotLightProbs[3].enabled;
}

void MultipleLightsScene::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth

    
    shader->use();
    shader->set("VP", camera->getVPMatrix());
    shader->set("cam_pos", camera->getPosition());

    for(int i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++){
        std::string l_acc = "directional_lights["+std::to_string(i)+"].";
        auto light = directionalLightProps[i];
        shader->set(l_acc+"enabled", light.enabled);
        shader->set(l_acc+"diffuse", light.diffuse);
        shader->set(l_acc+"specular", light.specular);
        shader->set(l_acc+"ambient", light.ambient);
        shader->set(l_acc+"direction", light.direction);  
    }
    for(int i = 0; i < NUM_POINT_LIGHTS; i++){
        std::string l_acc = "point_lights["+std::to_string(i)+"].";
        auto light = pointLightProbs[i];
        shader->set(l_acc+"enabled", light.enabled);
        shader->set(l_acc+"diffuse", light.diffuse);
        shader->set(l_acc+"specular", light.specular);
        shader->set(l_acc+"ambient", light.ambient);
        shader->set(l_acc+"position", light.position);  
        shader->set(l_acc+"attenuation_quadratic", light.attenuation_quadratic);
        shader->set(l_acc+"attenuation_linear", light.attenuation_linear);
        shader->set(l_acc+"attenuation_constant", light.attenuation_constant);
    }
    for(int i = 0; i < NUM_SPOT_LIGHTS; i++){
        std::string l_acc = "spot_lights["+std::to_string(i)+"].";
        auto light = spotLightProbs[i];
        shader->set(l_acc+"enabled", light.enabled);
        shader->set(l_acc+"diffuse", light.diffuse);
        shader->set(l_acc+"specular", light.specular);
        shader->set(l_acc+"ambient", light.ambient);
        shader->set(l_acc+"position", light.position);  
        shader->set(l_acc+"attenuation_quadratic", light.attenuation_quadratic);
        shader->set(l_acc+"attenuation_linear", light.attenuation_linear);
        shader->set(l_acc+"attenuation_constant", light.attenuation_constant);
        shader->set(l_acc+"direction", light.direction);
        shader->set(l_acc+"inner_cone", light.inner_cone);
        shader->set(l_acc+"outer_cone", light.outer_cone);
    }

    glm::mat4 ground_mat = glm::scale(glm::mat4(), glm::vec3(100, 1, 100));
    glm::mat4 ground_mat_it = glm::transpose(glm::inverse(ground_mat));
    shader->set("M", ground_mat);
    shader->set("M_it", ground_mat_it);
    shader->set("material.diffuse", {0.5f, 0.6f, 0.5f});
    shader->set("material.specular", {0.2f, 0.2f, 0.2f});
    shader->set("material.ambient", {0.5f, 0.6f, 0.5f});
    shader->set("material.shininess", 50);
    plane->draw();

    shader->set("material.diffuse", {0.7f, 0.2f, 0.1f});
    shader->set("material.ambient", {0.7f, 0.2f, 0.1f});

    for(int i = 0; i <= 10; i++){
        for(int j = 0; j <= 10; j++){
            glm::mat4 model_mat = glm::translate(glm::mat4(), glm::vec3(i*4, 1, j*4));
            glm::mat4 model_mat_it = glm::transpose(glm::inverse(model_mat));
            shader->set("M", model_mat);
            shader->set("M_it", model_mat_it);
            float specular = j/10.0f;
            shader->set("material.specular", {specular, specular, specular});
            shader->set("material.shininess", i * 5.0f + 1.0f);
            model->draw();
        }
    }

}

void MultipleLightsScene::Finalize() {
    delete controller;
    delete camera;
    delete model;
    delete plane;
    delete shader;
}