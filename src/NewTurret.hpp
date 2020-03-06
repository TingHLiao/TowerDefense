#ifndef NEWTURRET_HPP
#define NEWTURRET_HPP
#include "PlayScene.hpp"
#include "Bullet.hpp"
#include "Turret.hpp"

class PlayScene;
class NewTurret: public Turret {
public:
    static const int Price;
    NewTurret(float x, float y);
    void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
