#include "PlayScene.hpp"
#include "MachineGunTurret.hpp"
#include "FireBullet.hpp"
#include "AudioHelper.hpp"

const int MachineGunTurret::Price = 30;
MachineGunTurret::MachineGunTurret(float x, float y) :
	// TODO 2 (2/8): You can imitate the 2 files: 'MachineGunTurret.hpp', 'MachineGunTurret.cpp' to create a new turret.
	Turret("play/30.png", x, y, 200, Price, 1) {
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}
void MachineGunTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	//Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new FireBullet(Position, diff, rotation, this));
	AudioHelper::PlayAudio("gun.wav");
}
