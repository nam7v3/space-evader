#pragma once
#include <list>

#include "Entity.h"
using namespace std;

class Physic {
        public:
                void update();

                void push(Entity &e);
        private:
                list<Entity> entities;
};
