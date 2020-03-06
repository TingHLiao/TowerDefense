#ifndef BLACKTURRET_HPP
#define BLACKTURRET_HPP
#include "PlayScene.hpp"
#include "Bullet.hpp"
#include "Turret.hpp"

class PlayScene;
class blackTurret: public Turret {
public:
    static const int Price;
    blackTurret(float x, float y);
    void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
