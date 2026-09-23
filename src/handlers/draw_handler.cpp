#include "resources/drawable.hpp"
#include "handlers/resource_manager.hpp"

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include <glm/ext/vector_float3.hpp>
#include <vector>

// index -> shape
// 0 - Quad

#define VAO_COUNT 1

unsigned int VAOs[VAO_COUNT];

// Static methods
void Drawable::init_VAOs()
{
    std::vector<float> vertices[VAO_COUNT] = {
        {
            -0.5f,  0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f
        }
    };

    std::vector<unsigned int> indices[VAO_COUNT] = {
        {
            0, 1, 2,
            0, 2, 3
        }
    };

    // Generate all VAOs
    glGenVertexArrays(VAO_COUNT, VAOs);

    // Add render data to each VAO
    for (unsigned int i = 0; i < VAO_COUNT; ++i) {
        unsigned int buffers[2];
        glGenBuffers(2, buffers);

        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

        glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(float), vertices[i].data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices[i].size() * sizeof(unsigned int), indices[i].data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(2, buffers);
    }
}

void Drawable::delete_VAOs()
{
    glDeleteVertexArrays(VAO_COUNT, VAOs);
}

// Non-static methods
Drawable::Drawable(DrawableShape shape, float pos_x, float pos_y, float width, float height): m_shape(shape), m_model(1.0f)
{
    // Calculate m_model matrix from position and dimensions
    m_model = glm::translate(m_model, glm::vec3(pos_x, pos_y, 0.0f));
    m_model = glm::scale(m_model, glm::vec3(width, height, 1.0f));
}

void Drawable::draw() const
{
    static const unsigned int index_counts[VAO_COUNT] = {
        6
    };

    glBindVertexArray(VAOs[m_shape]);
    glDrawElements(GL_TRIANGLES, index_counts[m_shape], GL_UNSIGNED_INT, 0);
}
