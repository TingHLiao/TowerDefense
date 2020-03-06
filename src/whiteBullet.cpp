#include "whiteBullet.hpp"
#include "DirtyEffect.hpp"

whiteBullet::whiteBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent) :
Bullet("play/bullet5.png", 350, 9, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
    
}
void whiteBullet::OnExplode(Enemy* enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-3.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
