#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

struct Point {
    float x, y;
};

struct Node {
    Point pos;
    bool hasHandle1 = false, hasHandle2 = false;
    Point handle1, handle2;
};

std::vector<Node> nodes;
bool draggingNode = false;
int draggedNodeIndex = -1;

void drawBezierCurve(Node& p0, Node& p1) {
    glColor3f(0, 0, 0); // Black lines
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1; t += 0.005) {
        float x = (1 - t) * (1 - t) * (1 - t) * p0.pos.x +
                  3 * (1 - t) * (1 - t) * t * p0.handle2.x +
                  3 * (1 - t) * t * t * p1.handle1.x +
                  t * t * t * p1.pos.x;
        float y = (1 - t) * (1 - t) * (1 - t) * p0.pos.y +
                  3 * (1 - t) * (1 - t) * t * p0.handle2.y +
                  3 * (1 - t) * t * t * p1.handle1.y +
                  t * t * t * p1.pos.y;
        glVertex2f(x, y);
    }
    glEnd();
}

void drawScene() {
    glClearColor(1, 1, 1, 1); // White background
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0);
    
    if (!nodes.empty()) {
        for (size_t i = 0; i < nodes.size() - 1; ++i)
            drawBezierCurve(nodes[i], nodes[i + 1]);
    }
    
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(0, 0, 1); // Blue nodes
    for (auto& node : nodes) {
        glVertex2f(node.pos.x, node.pos.y);
    }
    glEnd();
    
    glColor3f(0, 0, 0); // Black dotted lines for handles
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xAAAA);
    glBegin(GL_LINES);
    for (auto& node : nodes) {
        if (node.hasHandle1) {
            glVertex2f(node.pos.x, node.pos.y);
            glVertex2f(node.handle1.x, node.handle1.y);
        }
        if (node.hasHandle2) {
            glVertex2f(node.pos.x, node.pos.y);
            glVertex2f(node.handle2.x, node.handle2.y);
        }
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    
    glFlush();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    my = 600 - my; 
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (hypot(nodes[i].pos.x - mx, nodes[i].pos.y - my) < 10) {
                draggingNode = true;
                draggedNodeIndex = i;
                return;
            }
        }
        
        Node newNode;
        newNode.pos = { (float)mx, (float)my };
        if (!nodes.empty()) {
            newNode.hasHandle1 = true;
            newNode.handle1 = { (float)mx, (float)my + 50 };
        }
        nodes.push_back(newNode);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        draggingNode = false;
        draggedNodeIndex = -1;
    }
}

void cursorPosCallback(GLFWwindow* window, double mx, double my) {
    my = 600 - my;
    if (draggingNode && draggedNodeIndex >= 0 && draggedNodeIndex < (int)nodes.size()) {
        nodes[draggedNodeIndex].pos.x = (float)mx;
        nodes[draggedNodeIndex].pos.y = (float)my;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        nodes.clear();
        draggingNode = false;
        draggedNodeIndex = -1;
    }
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Bezier Spline Editor", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    while (!glfwWindowShouldClose(window)) {
        drawScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
