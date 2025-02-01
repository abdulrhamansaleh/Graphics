#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

std::vector<float> dogCoords;
void drawDog() {
    glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < dogCoords.size(); i += 2) {
        glVertex2f(dogCoords[i], dogCoords[i + 1]);
    }
    glEnd();
}

int main() {
    // window
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise 1", NULL, NULL);
    glfwMakeContextCurrent(window);

    // read file and store coordinates/points
    std::ifstream file("dog.txt");
    float x, y;
    while (file >> x >> y) {
        dogCoords.push_back(x);
        dogCoords.push_back(y);
    }
    file.close();

    // Set up projection matrix once
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 60, 0, 60, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    float rotationAngle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        // white color
        glClearColor(1, 1, 1, 1); 
        glClear(GL_COLOR_BUFFER_BIT);

        static const float angles[8] = {0, 45, 90, 135, 180, 225, 270, 315};
        glColor3f(0, 0, 0);

        for (float angle : angles) {
            glPushMatrix();

            // get transformation
            float rad = angle * M_PI / 180.0f;
            float cx = 30.0f + 25.0f * cos(rad);
            float cy = 30.0f + 25.0f * sin(rad);

            // Apply transformations
            glTranslatef(cx, cy, 0);
            glRotatef(rotationAngle, 0, 0, 1);
            drawDog();

            glPopMatrix();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        rotationAngle += 1.0f;
    }

    glfwTerminate();
    return 0;
}
