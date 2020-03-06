#include "PlayScene.hpp"
#include "MissileTurret.hpp"
#include "MissileBullet.hpp"
#include "AudioHelper.hpp"

const int MissileTurret::Price = 80;
MissileTurret::MissileTurret(float x, float y) :
// TODO 2 (2/8): You can imitate the 2 files: 'MachineGunTurret.hpp', 'MachineGunTurret.cpp' to create a new turret.
Turret("play/80.png", x, y, 200, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void MissileTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    //Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new MissileBullet(Position, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
