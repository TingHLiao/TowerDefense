#include "PlayScene.hpp"
#include "whiteTurret.hpp"
#include "whiteBullet.hpp"
#include "AudioHelper.hpp"

const int whiteTurret::Price = 125;
whiteTurret::whiteTurret(float x, float y) :
Turret("play/125.png", x, y, 200, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void whiteTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    //Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new whiteBullet(Position, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
