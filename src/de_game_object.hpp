#pragma once

#include "de_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace de {
    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
        // rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

    struct PointLightComponent {
        float lightIntensity = 1.0f;
    };

    class DeGameObject {
        public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, DeGameObject>;

        static DeGameObject createGameObject() {
            static id_t currentId = 0;
            return DeGameObject{currentId++};
        }

        static DeGameObject makePointLight(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        DeGameObject(const DeGameObject &) = delete;
        DeGameObject &operator=(const DeGameObject &) = delete;
        DeGameObject(DeGameObject &&) = default;
        DeGameObject &operator=(DeGameObject &&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        // Optional pointer components
        std::shared_ptr<DeModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;

        private:
        DeGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}