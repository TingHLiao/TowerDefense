#include "PlayScene.hpp"
#include "blackTurret.hpp"
#include "blackBullet.hpp"
#include "AudioHelper.hpp"

const int blackTurret::Price = 200;
blackTurret::blackTurret(float x, float y) :
Turret("play/200.png", x, y, 200, Price, 0.4) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void blackTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    //Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new blackBullet(Position, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
