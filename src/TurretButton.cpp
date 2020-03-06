#include "TurretButton.hpp"
PlayScene* TurretButton::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
TurretButton::TurretButton(std::string img, std::string imgIn, Engine::Sprite Turret, float x, float y, int money) :
	ImageButton(img, imgIn, x, y), money(money), /*Base(Base),*/ Turret(Turret) {
}
void TurretButton::Update(float deltaTime) {
	ImageButton::Update(deltaTime);
	if (getPlayScene()->GetMoney() >= money) {
		Enabled = true;
		Turret.Tint = al_map_rgba(255, 255, 255, 255);
	} else {
		Enabled = false;
		Turret.Tint = al_map_rgba(0, 0, 0, 150);
	}
}
void TurretButton::Draw() const {
	ImageButton::Draw();
	Turret.Draw();
}
