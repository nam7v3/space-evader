#include "Entity.h"

bool collide(Entity &a, Entity &b){
        if(!((a.px-b.px)*(a.px-b.px)+(a.py-b.py)*(a.py-b.py)<(a.radius+b.radius)*(a.radius+b.radius))){
                return false;
        }
        return true;
}
