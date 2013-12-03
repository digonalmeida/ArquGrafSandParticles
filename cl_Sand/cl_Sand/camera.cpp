#include "camera.h"

Camera::Camera()
{
    for(int i=0; i<3; i++)
    {
        focus_point[i]=0;
        rotation[i]=0;
    }
    distance=100;
}
Camera::~Camera()
{
}
void Camera::viewport_update(){
    glViewport(vx,vy,vw,vh);
}
void Camera::viewport(float x, float y, float w, float h){
    vx=x;
    vy=y;
    vw=w;
    vh=w;
}
void Camera::update(){
    viewport_update();
    view_update();
    rotation_update();
}
void Camera::view_update()
{
    gluLookAt(0,0,-distance,focus_point[0],focus_point[1],focus_point[2],0,1,0);
    //gluLookAt(0,0,-1.5,0,0,0,0,1,0);
}
void Camera::rotation_update()
{
    glRotatef(rotation[0],1,0,0);
    glRotatef(rotation[1],0,1,0);
    glRotatef(rotation[2],0,0,1);
}
