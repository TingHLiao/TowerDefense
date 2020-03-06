#ifndef BLACKBULLET_HPP
#define BLACKBULLET_HPP
#include "Bullet.hpp"
class blackBullet : public Bullet {
public:
    explicit blackBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif
