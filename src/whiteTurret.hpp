#ifndef WHITETURRET_HPP
#define WHITETURRET_HPP
#include "PlayScene.hpp"
#include "Bullet.hpp"
#include "Turret.hpp"

class PlayScene;
class whiteTurret: public Turret {
public:
    static const int Price;
    whiteTurret(float x, float y);
    void CreateBullet() override;
};
#endif // WHITETURRET_HPP
