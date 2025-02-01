#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 1000, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with the current color

        glBegin(GL_TRIANGLES);
            glColor3f(0.0f, 1.0f, 0.0f); // green color
            glVertex2f(-0.5f,-0.25f);
            glVertex2f(0.5f, -0.25f);
            glVertex2f(0.0f, 0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    /* Clean up and exit */
    glfwTerminate();
    return 0;
}
