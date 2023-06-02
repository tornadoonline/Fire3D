#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, char** argv)
{

    glfwInit();
    auto _window = glfwCreateWindow(800, 600, "Fire3D", nullptr, nullptr);

    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
    }
    
    std::cout << "Hello Fire3D.\n";
    return 0;
}