#include <allegro5/allegro_primitives.h>
#include "GameEngine.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"

PlayScene* Turret::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Turret::Turret(std::string imgTurret, float x, float y, float radius, int price, float coolDown) :
	Sprite(imgTurret, x, y), price(price), coolDown(coolDown) /*imgBase(imgBase, x, y)*/ {
	CollisionRadius = radius;
}
void Turret::Update(float deltaTime) {
	Sprite::Update(deltaTime);
	PlayScene* scene = getPlayScene();
	if (!Enabled)
		return;
	
    reload -= deltaTime;
    if (reload <= 0) {
        // shoot.
        reload = coolDown;
        CreateBullet();
    }
    
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (!enemy->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, 10, enemy->Position, enemy->CollisionRadius)) {
            enemy->Hit(100);
            getPlayScene()->mapState[Position.y/150][Position.x/150] = PlayScene::TileType(0);
            getPlayScene()->UIGroup->RemoveObject(objectIterator);
            return;
        }
    }
}
void Turret::Draw() const {
	if (Preview) {
		//al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgba(0, 255, 0, 50));
	}
	Sprite::Draw();
	if (PlayScene::DebugMode) {
		// Draw target radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(0, 0, 255), 2);
	}
}
int Turret::GetPrice() const {
	return price;
}
