#include "Entity.h"

Entity::Entity(float px, float py, float vx, float vy, int radius, float mass): px(py), py(py), vx(vx), vy(vy), radius(radius), mass(mass){};
Entity::Entity(): px(0), py(0), vx(0), vy(0), radius(0), mass(0){};



const float Entity::get_px(){
        return px;
}
const float Entity::get_py(){
        return py;
}
const float Entity::get_vx(){
        return vx;
}
const float Entity::get_vy(){
        return vy;
}

void Entity::set_vx(float new_vx){
        vx = new_vx;
}
void Entity::set_vy(float new_vy){
        vy = new_vy;
}
void Entity::set_px(float new_px){
        px = new_px;
}
void Entity::set_py(float new_py){
        py = new_py;
}
