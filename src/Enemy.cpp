#include <random>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "PlayScene.hpp"
#include "Label.hpp"
#include "Enemy.hpp"
#include "ExplosionEffect.hpp"
#include "DirtyEffect.hpp"
#include "MachineGunTurret.hpp"
#include "AudioHelper.hpp"

PlayScene* Enemy::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {
	getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
	for (int i = 0; i < 10; i++) {
		// Random add 10 dirty effects.
		getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
	}
}
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money, int way) :
	Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money), way(way) {
	CollisionRadius = radius;
}
void Enemy::Hit(float damage) {
	hp -= damage;
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedTurrets)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		getPlayScene()->EarnMoney(money);
		getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
	}
}
void Enemy::UpdatePath(const std::vector<std::vector<int>>& mapDistance) {
	int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize)-2);
	int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
   
	if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight) {
		x = 0;
		y = 0;
	}
	
	int num = mapDistance[y][x];
	path = std::vector<Engine::Point>(num + 1);
	
    while(num >= 0){
        //std::cout << num-1 << " " << y << "ooo\n";
        Engine::Point pos(num-1, y);
        path[num] = pos;
        num--;
    }
    
}
void Enemy::Update(float deltaTime) {
	// Pre-calculate the velocity.
	float remainSpeed = speed * deltaTime;
    //std::cout << "remainSpeed: " << remainSpeed << "\n";
	while (remainSpeed != 0) {
		if (path.empty()) {
			// Reach end point.
			getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
			getPlayScene()->Hit();
			return;
		}
		Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		Engine::Point normalized = vec.Normalize();
       
		if (remainSpeed - vec.Magnitude() > 0) {
			Position = target;
			path.pop_back();
			remainSpeed -= vec.Magnitude();
		}
		else {
			Velocity = normalized * remainSpeed / deltaTime;
            
			remainSpeed = 0;
		}
	}
	Rotation = atan2(Velocity.y, Velocity.x);
	Sprite::Update(deltaTime);
}

void Enemy::Draw() const {
	Sprite::Draw();
    
    if(bmp == Engine::Resources::GetInstance().GetBitmap("play/nerd5.png")&&hp>150){
        al_draw_filled_rectangle(Position.x-270, Position.y-380, Position.x-300 + hp/5, Position.y-360, al_map_rgb(205, 55, 0));
        al_draw_rectangle(Position.x-270, Position.y-380, Position.x-300 + 3000/5, Position.y-360, al_map_rgb(0, 0, 0), 2);
    }
	if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
	}
}
