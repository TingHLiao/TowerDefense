#include "PlayScene.hpp"
#include "LaserBullet.hpp"
#include "LaserTurret.hpp"
#include "AudioHelper.hpp"

const int LaserTurret::Price = 50;
LaserTurret::LaserTurret(float x, float y) :
	Turret("play/50.png", x, y, 300, Price, 1) {
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}
void LaserTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	//Engine::Point normalized = diff.Normalize();
	//Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new LaserBullet(Position, diff, rotation, this));
    
	AudioHelper::PlayAudio("laser.wav");
}
