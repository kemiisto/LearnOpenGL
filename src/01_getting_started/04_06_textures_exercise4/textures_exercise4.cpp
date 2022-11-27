#include "main.h"
#include "resource.h"
#include <tinygl/tinygl.h>
#include <fmt/format.h>
#include <iostream>

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void init() override;
    void processInput() override;
    void draw() override;
private:
    tinygl::ShaderProgram program;
    tinygl::Buffer vbo{tinygl::Buffer::Type::VertexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::Buffer ibo{tinygl::Buffer::Type::IndexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::VertexArrayObject vao;
    tinygl::Texture texture0{
        tinygl::Texture::Target::Target2D,
        resourcePath("textures/container.jpg"),
        GL_RGB, GL_RGB, true, 0
    };
    tinygl::Texture texture1{
        tinygl::Texture::Target::Target2D,
        resourcePath("textures/awesomeface.png"),
        GL_RGBA, GL_RGBA, true, 1
    };
    GLfloat weight = 0.0f;
};

void Window::init()
{
    program.addShaderFromSourceFile(tinygl::Shader::Type::Vertex, "textures_exercise4.vert");
    program.addShaderFromSourceFile(tinygl::Shader::Type::Fragment, "textures_exercise4.frag");
    program.link();

    program.use();
    program.setUniformValue("texture0", 0);
    program.setUniformValue("texture1", 1);

    const GLfloat vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    vbo.bind();
    vbo.create(sizeof(vertices), vertices);
    vbo.unbind();

    const GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    ibo.bind();
    ibo.create(sizeof(indices), indices);
    ibo.unbind();

    vao.bind();

    vbo.bind();
    vao.setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    vao.enableAttributeArray(0);
    vao.setAttributeArray(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    vao.enableAttributeArray(1);
    vao.setAttributeArray(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));
    vao.enableAttributeArray(2);
    vbo.unbind();

    ibo.bind();
    vao.unbind();
}

void Window::processInput()
{
    if (getKey(tinygl::keyboard::Key::Escape) == tinygl::keyboard::KeyState::Press) {
        setShouldClose(true);
    }
    if (getKey(tinygl::keyboard::Key::Up) == tinygl::keyboard::KeyState::Press) {
        weight += 0.0001f;
        if (weight > 1.0f) {
            weight = 1.0f;
        }
    }
    if (getKey(tinygl::keyboard::Key::Down) == tinygl::keyboard::KeyState::Press) {
        weight -= 0.0001f;
        if (weight < 0.0f) {
            weight = 0.0f;
        }
    }
}

void Window::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture0.bind();
    texture1.bind();

    program.use();
    program.setUniformValue("weight", weight);
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

MAIN
