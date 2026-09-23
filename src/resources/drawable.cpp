#include "resources/drawable.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

glm::mat4 Drawable::get_model() const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(w, h, 1.0f));

    return model;
}
