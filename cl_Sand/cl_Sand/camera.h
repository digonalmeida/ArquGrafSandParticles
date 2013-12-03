/*
    CLASS CAMERA
    controls viewport, and screen rotation;
*/

#ifndef CAMERA_H
#define CAMERA_H
#include <GL\glut.h>

class Camera
{
public:
    Camera();
    ~Camera();
    float focus_point[3];
    float distance;
    float rotation[3];
    void update(); // call view,viewport and rotation update. [glLoadIdentity not Called inside the Camera Update!]

    float vx,vy,vw,vh;// viewport position and size;
    void viewport(float x, float y, float w, float h); // set Camera's viewport attributes
    void viewport_update(); //glViewport
    void view_update(); //gluLookAt
    void rotation_update(); // glRotate
};

#endif // CAMERA_H
