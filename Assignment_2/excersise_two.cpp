#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <ctime>

// random floats in [-1, 1]
static inline double frand() {
    double x = ((double)rand()) / (double)RAND_MAX;
    if (rand() % 2) {
        x *= -1.0;
    }
    return x;
}

std::vector<float> points;

int main(int argc, char** argv) {
    // Parse arguments
    int N = atoi(argv[1]);
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "Exercise 2", NULL, NULL);
    glfwMakeContextCurrent(window);

    float xMin = -1.f, yMin = -1.f;
    float xMax = 1.f, yMax = 1.f;

    int cornerIndex = rand() % 4;
    float px = static_cast<float>(frand());
    float py = static_cast<float>(frand());

    for (long long i = 0; i < N; ++i) {
        float cornerX, cornerY;
        switch (cornerIndex) {
            case 0: cornerX = xMin; cornerY = yMin; break; // Bottom-left
            case 1: cornerX = xMin; cornerY = yMax; break; // Top-left
            case 2: cornerX = xMax; cornerY = yMax; break; // Top-right
            case 3: cornerX = xMax; cornerY = yMin; break; // Bottom-right
        }

        // avoid diagonal
        int avoid = (cornerIndex + 2) % 4;
        int nextCorner;
        do {
            nextCorner = rand() % 4;
        } while (nextCorner == avoid);

        cornerIndex = nextCorner;

        px = 0.5f * (px + cornerX);
        py = 0.5f * (py + cornerY);

        points.push_back(px);
        points.push_back(py);
    }

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.1, 1.1, -1.1, 1.1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0, 0, 0);
    glPointSize(2.0f);
    
    glBegin(GL_POINTS);
    for (size_t i = 0; i < points.size(); i += 2) {
        glVertex2f(points[i], points[i + 1]);
    }
    glEnd();

    glfwSwapBuffers(window);

    // Keep window open
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
