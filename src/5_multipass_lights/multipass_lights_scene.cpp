#include <application.hpp>
#include "multipass_lights_scene.hpp"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
*
* This scene shows one option to support multiple lights in one scene which is to use multiple single light shaders
* and blend their results through multiple draw calls
*
*/

void MultiPassLightsScene::Initialize() {
    Shader* shader;
    shader = new Shader();
    shader->attach("assets/shaders/phong/single light/directional.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/single light/directional.frag", GL_FRAGMENT_SHADER);
    shader->link();
    shaders[static_cast<int>(LightType::DirectionalLight)] = shader;
    shader = new Shader();
    shader->attach("assets/shaders/phong/single light/point.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/single light/point.frag", GL_FRAGMENT_SHADER);
    shader->link();
    shaders[static_cast<int>(LightType::PointLight)] = shader;
    shader = new Shader();
    shader->attach("assets/shaders/phong/single light/spot.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/phong/single light/spot.frag", GL_FRAGMENT_SHADER);
    shader->link();
    shaders[static_cast<int>(LightType::SpotLight)] = shader;

    Light light;
    light.enabled = true;
    light.type = LightType::DirectionalLight;
    light.diffuse = {0.45f, 0.4f, 0.3f};
    light.specular = {0.45f, 0.4f, 0.3f};
    light.ambient = {0.44f, 0.34f, 0.075f};
    light.direction = glm::normalize(glm::vec3(-1, -1, -1));
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::PointLight;
    light.diffuse = {1.0f, 0.3f, 0.3f};
    light.specular = {1.0f, 0.3f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {2, 1, 2};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    lights.push_back(light);
    
    light.enabled = true;
    light.type = LightType::PointLight;
    light.diffuse = {0.3f, 1.0f, 0.3f};
    light.specular = {0.3f, 1.0f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {38, 1, 2};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::PointLight;
    light.diffuse = {0.3f, 0.3f, 1.0f};
    light.specular = {0.3f, 0.3f, 1.0f};
    light.ambient = {0, 0, 0};
    light.position = {38, 1, 38};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::PointLight;
    light.diffuse = {0.6f, 0.6f, 0.3f};
    light.specular = {0.6f, 0.6f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {2, 1, 38};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::SpotLight;
    light.diffuse = {1.0f, 0.3f, 0.3f};
    light.specular = {1.0f, 0.3f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {6, 1, 6};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    light.direction = glm::normalize(glm::vec3(1,-1,1));
    light.inner_cone = glm::pi<float>()/4;
    light.outer_cone = glm::pi<float>()/3;
    lights.push_back(light);
    
    light.enabled = true;
    light.type = LightType::SpotLight;
    light.diffuse = {0.3f, 1.0f, 0.3f};
    light.specular = {0.3f, 1.0f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {34, 1, 6};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    light.direction = glm::normalize(glm::vec3(-1,-1,1));
    light.inner_cone = glm::pi<float>()/4;
    light.outer_cone = glm::pi<float>()/3;
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::SpotLight;
    light.diffuse = {0.3f, 0.3f, 1.0f};
    light.specular = {0.3f, 0.3f, 1.0f};
    light.ambient = {0, 0, 0};
    light.position = {34, 1, 34};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    light.direction = glm::normalize(glm::vec3(-1,-1,-1));
    light.inner_cone = glm::pi<float>()/4;
    light.outer_cone = glm::pi<float>()/3;
    lights.push_back(light);

    light.enabled = true;
    light.type = LightType::SpotLight;
    light.diffuse = {0.6f, 0.6f, 0.3f};
    light.specular = {0.6f, 0.6f, 0.3f};
    light.ambient = {0, 0, 0};
    light.position = {6, 1, 34};
    light.attenuation_quadratic = 0.0f;
    light.attenuation_linear = 1.0f;
    light.attenuation_constant = 0.01f;
    light.direction = glm::normalize(glm::vec3(1,-1,-1));
    light.inner_cone = glm::pi<float>()/4;
    light.outer_cone = glm::pi<float>()/3;
    lights.push_back(light);
    
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
    //Since we will draw the scene multiple times,
    //the depth test must pass for pixels drawn at the same depth
    //or every pass after the first one will fail
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.44f, 0.34f, 0.075f, 0.0f);
}

void MultiPassLightsScene::Update(double delta_time) {
    controller->update(delta_time);
    Keyboard* kb = getKeyboard();

    for(size_t i = 0; i < lights.size(); i++){
        if(kb->justPressed(GLFW_KEY_0 + i)) lights[i].enabled = !lights[i].enabled;    
    }
}

void MultiPassLightsScene::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear colors and depth

    bool first_light = true;
    for(Light light: lights){
        if(!light.enabled) continue;
        if(first_light){
            //If it is the first light, we don't need blending
            glDisable(GL_BLEND);
        }

        Shader* shader = shaders[static_cast<int>(light.type)];
        shader->use();

        shader->set("VP", camera->getVPMatrix());
        shader->set("cam_pos", camera->getPosition());

        shader->set("light.diffuse", light.diffuse);
        shader->set("light.specular", light.specular);
        shader->set("light.ambient", light.ambient);
        if(light.type == LightType::DirectionalLight || light.type == LightType::SpotLight)
            shader->set("light.direction", light.direction);
        if(light.type == LightType::PointLight || light.type == LightType::SpotLight){
            shader->set("light.position", light.position);  
            shader->set("light.attenuation_quadratic", light.attenuation_quadratic);
            shader->set("light.attenuation_linear", light.attenuation_linear);
            shader->set("light.attenuation_constant", light.attenuation_constant);
            shader->set("light.direction", light.direction);
        }
        if(light.type == LightType::SpotLight){
            shader->set("light.inner_cone", light.inner_cone);
            shader->set("light.outer_cone", light.outer_cone);
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

        if(first_light){
            //For latter passes, we don't need to modify the depth buffer
            glDepthMask(GL_FALSE);
            //And we will use blending to add new lights to the screen
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            first_light = false;
        }
    }
    //Re-enable writing to depth buffer so that it can be cleared in the next frame
    glDepthMask(GL_TRUE);

}

void MultiPassLightsScene::Finalize() {
    delete controller;
    delete camera;
    delete model;
    delete plane;
    delete shaders[0];
    delete shaders[1];
    delete shaders[2];
}