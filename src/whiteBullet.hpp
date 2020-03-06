#ifndef WHITEBULLET_HPP
#define WHITEBULLET_HPP
#include "Bullet.hpp"
class whiteBullet : public Bullet {
public:
    explicit whiteBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif 
