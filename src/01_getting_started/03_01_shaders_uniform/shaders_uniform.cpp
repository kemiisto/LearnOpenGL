#include "main.h"
#include <tinygl/tinygl.h>
#include <iostream>
#include <cmath>

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
    tinygl::VertexArrayObject vao;
};

void Window::init()
{
    program.addShaderFromSourceFile(tinygl::Shader::Type::Vertex, "shaders_uniform.vert");
    program.addShaderFromSourceFile(tinygl::Shader::Type::Fragment, "shaders_uniform.frag");
    program.link();

    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };
    vbo.bind();
    vbo.create(sizeof(vertices), vertices);
    vbo.unbind();

    vao.bind();
    vbo.bind();
    vao.setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    vao.enableAttributeArray(0);
    vbo.unbind();
    vao.unbind();
}

void Window::processInput()
{
    if (getKey(tinygl::keyboard::Key::Escape) == tinygl::keyboard::KeyState::Press) {
        setShouldClose(true);
    }
}

void Window::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.use();

    auto time = tinygl::getTime();
    auto green = std::sin(time) / 2.0f + 0.5f;
    auto colorLocation = program.uniformLocation("color");
    program.setUniformValue(colorLocation, 0.0f, green, 0.0f, 1.0f);

    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

MAIN
