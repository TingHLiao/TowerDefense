#ifndef MISSILEBULLET_HPP
#define MISSILEBULLET_HPP
#include "Bullet.hpp"
class MissileBullet : public Bullet {
public:
    explicit MissileBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif 
