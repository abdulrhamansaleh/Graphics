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
bool draggingNode = false, draggingHandle = false;
int draggedNodeIndex = -1, draggedHandleIndex = -1;
bool draggingHandle1 = false, draggingHandle2 = false;

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
    glColor3f(1, 0, 0); // Red handles
    for (auto& node : nodes) {
        if (node.hasHandle1) {
            glVertex2f(node.handle1.x, node.handle1.y);
        }
        if (node.hasHandle2) {
            glVertex2f(node.handle2.x, node.handle2.y);
        }
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
    bool clickedOnExisting = false;
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    my = 600 - my; 
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        bool clickedOnExisting = false;
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (hypot(nodes[i].pos.x - mx, nodes[i].pos.y - my) < 10) {
                draggingNode = true;
                draggedNodeIndex = i;
                clickedOnExisting = true;
                return;
            }
            if (hypot(nodes[i].handle1.x - mx, nodes[i].handle1.y - my) < 10) {
                draggingHandle = true;
                draggingHandle1 = true;
                draggedNodeIndex = i;
                clickedOnExisting = true;
                return;
            }
            if (hypot(nodes[i].handle2.x - mx, nodes[i].handle2.y - my) < 10) {
                draggingHandle = true;
                draggingHandle2 = true;
                draggedNodeIndex = i;
                clickedOnExisting = true;
                return;
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !clickedOnExisting) {
        Node newNode;
        newNode.pos = { (float)mx, (float)my };
        newNode.hasHandle1 = true;
        newNode.handle1 = { (float)mx + 50, (float)my };
        newNode.hasHandle2 = true;
        newNode.handle2 = { (float)mx - 50, (float)my };
        nodes.push_back(newNode);
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        draggingNode = false;
        draggingHandle = false;
        draggingHandle1 = false;
        draggingHandle2 = false;
        draggedNodeIndex = -1;
    }
}

void cursorPosCallback(GLFWwindow* window, double mx, double my) {
    my = 600 - my;
    if (draggingNode && draggedNodeIndex >= 0) {
        Node& node = nodes[draggedNodeIndex];
        float dx = (float)mx - node.pos.x;
        float dy = (float)my - node.pos.y;
        
        // Update node position
        node.pos.x = (float)mx;
        node.pos.y = (float)my;
        
        // Move handles while maintaining their relative offsets
        if (node.hasHandle1) {
            node.handle1.x += dx;
            node.handle1.y += dy;
        }
        if (node.hasHandle2) {
            node.handle2.x += dx;
            node.handle2.y += dy;
        }
        nodes[draggedNodeIndex].pos.x = (float)mx;
        nodes[draggedNodeIndex].pos.y = (float)my;
    }
    if (draggingHandle && draggedNodeIndex >= 0) {
        Node& node = nodes[draggedNodeIndex];
        if (draggingHandle1) {
            float dx = (float)mx - node.pos.x;
            float dy = (float)my - node.pos.y;
            float dist = hypot(dx, dy);
            float angle = atan2(dy, dx);
            node.handle1.x = node.pos.x + dist * cos(angle);
            node.handle1.y = node.pos.y + dist * sin(angle);
            if (node.hasHandle2) { // Mirror handle for collinearity
                node.handle2.x = node.pos.x - dist * cos(angle);
                node.handle2.y = node.pos.y - dist * sin(angle);
            }
            nodes[draggedNodeIndex].handle1.x = (float)mx;
            nodes[draggedNodeIndex].handle1.y = (float)my;
        }
        if (draggingHandle2) {
            float dx = (float)mx - node.pos.x;
            float dy = (float)my - node.pos.y;
            float dist = hypot(dx, dy);
            float angle = atan2(dy, dx);
            node.handle2.x = node.pos.x + dist * cos(angle);
            node.handle2.y = node.pos.y + dist * sin(angle);
            if (node.hasHandle1) { // Mirror handle for collinearity
                node.handle1.x = node.pos.x - dist * cos(angle);
                node.handle1.y = node.pos.y - dist * sin(angle);
            }
            nodes[draggedNodeIndex].handle2.x = (float)mx;
            nodes[draggedNodeIndex].handle2.y = (float)my;
        }
    }
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Assignment 3", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    
    glfwMakeContextCurrent(window);
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    
    while (!glfwWindowShouldClose(window)) {
        drawScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
