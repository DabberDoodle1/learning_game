#include "drawable.hpp"

#include <glad/glad.h>
#include <vector>

unsigned int quad_VAO;

void Drawable::init_VAOs()
{
    unsigned int* VAOs[] = {
        &quad_VAO
    };

    std::vector<float> vertices[] = {
        {
            -1.0f,  1.0f,
             1.0f,  1.0f,
             1.0f, -1.0f,
            -1.0f, -1.0f
        }
    };

    std::vector<unsigned int> indices[] = {
        {
            0, 1, 2,
            0, 2, 3
        }
    };

    for (unsigned int i = 0; i < sizeof(VAOs) / sizeof(VAOs[0]); ++i) {
        unsigned int buffers[2];

        glGenVertexArrays(1, VAOs[i]);
        glGenBuffers(2, buffers);

        glBindVertexArray(*VAOs[i]);
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
    unsigned int* VAOs[] = {
        &quad_VAO
    };

    for (unsigned int* VAO : VAOs) {
        if (*VAO) {
            glDeleteVertexArrays(1, VAO);
        }
    }
}

void Drawable::draw(DrawableShapes shape) const
{
    unsigned int VAOs[] = {
        quad_VAO
    };

    unsigned int index_counts[] = {
        6
    };

    glBindVertexArray(VAOs[shape]);
    glDrawElements(GL_TRIANGLES, index_counts[shape], GL_UNSIGNED_INT, 0);
}
