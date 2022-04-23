#pragma once

class Entity {
        public:
                Entity(float px, float py, float vx, float vy, int radius, float mass);
                Entity();

                const float get_px();
                const float get_py();
                const float get_vx();
                const float get_vy();

                void set_vx(float vx);
                void set_vy(float vy);
                void set_px(float px);
                void set_py(float py);
        private:
                float px, py;
                float vx, vy;
                float mass;
                int radius;
};
