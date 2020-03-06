#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

#include "Image.hpp"
#include "Label.hpp"
#include "ImageButton.hpp"
#include "PlayScene.hpp"
#include "LOG.hpp"
#include "Enemy.hpp"
#include "PlaneEnemy.hpp"
#include "SoldierEnemy.hpp"
#include "TankEnemy.hpp"
#include "MyEnemy.hpp"
#include "Boss.hpp"
#include "MachineGunTurret.hpp"
#include "LaserTurret.hpp"
#include "MissileTurret.hpp"
#include "NewTurret.hpp"
#include "whiteTurret.hpp"
#include "blackTurret.hpp"
#include "TurretButton.hpp"
#include "Plane.hpp"
#include "DirtyEffect.hpp"
#include "AudioHelper.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 10, PlayScene::MapHeight = 7;
const int PlayScene::BlockSize = 150;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
									ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
									ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
	// TODO 5 (1/2): There's a bug in this file, which crashes the game when you win. Try to find it.
	// TODO 5 (2/2): There's a cheat code in this file. Try to find it.
	mapState.clear();
	keyStrokes.clear();
	ticks1 = ticks2 = ticks3 = ticks4 = ticks5 = 0;
	lives = 10;
	money = 100;
	SpeedMult = 1;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	// Add groups from bottom to top.
    
	//AddNewObject(TileMapGroup = new Group());
    AddNewObject(new Engine::Image("play/bg.jpg", 0, 0, w, h));
    AddNewObject(new Engine::Image("play/coin.png", 35, 10));
    AddNewObject(new Engine::Image("play/lollipop.png", 65, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 221, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 377, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 533, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 689, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 845, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 1001, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 1157, 925));
    AddNewObject(new Engine::Image("play/lollipop.png", 1313, 925));
    
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target1.png", 0, 0);
	imgTarget->Visible = false;
	UIGroup->AddNewObject(imgTarget);
	AudioHelper::PlayBGM("play.ogg");
}

void PlayScene::Update(float deltaTime) {
	// Fake speed up.
	deltaTime *= SpeedMult;
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	if (deltaTime < 3) {
		IScene::Update(deltaTime);
	} else if (deltaTime < 6) {
		IScene::Update(deltaTime / 2);
		IScene::Update(deltaTime / 2);
	} else if (deltaTime < 9) {
		IScene::Update(deltaTime / 3);
		IScene::Update(deltaTime / 3);
		IScene::Update(deltaTime / 3);
	} else {
		IScene::Update(deltaTime / 4);
		IScene::Update(deltaTime / 4);
		IScene::Update(deltaTime / 4);
		IScene::Update(deltaTime / 4);
	}
	if (preview) {
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	}
	// Check if we should create new enemy.
	ticks1 += deltaTime;
    ticks2 += deltaTime;
    ticks3 += deltaTime;
    ticks4 += deltaTime;
    ticks5 += deltaTime;
	if (enemyWaveData1.empty() && enemyWaveData2.empty() && enemyWaveData3.empty() && enemyWaveData4.empty() && enemyWaveData5.empty()) {
		if (EnemyGroup->GetObjects().empty()) {
			Engine::GameEngine::GetInstance().ChangeScene("win");
		}
		return;
	}
    bool b1 = false, b2 = false, b3 = false, b4 = false, b5 = false;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    std::pair<int, float> current1, current2, current3, current4, current5;
    if(!enemyWaveData1.empty()){
        current1 = enemyWaveData1.front();
        if (ticks1 >= current1.second)
            b1 = true;
    }
    if(!enemyWaveData2.empty()){
        current2 = enemyWaveData2.front();
        if (ticks2 >= current2.second)
            b2 = true;
    }
    if(!enemyWaveData3.empty()){
        current3 = enemyWaveData3.front();
        if (ticks3 >= current3.second)
            b3 = true;
    }
    if(!enemyWaveData4.empty()){
        current4 = enemyWaveData4.front();
        if (ticks4 >= current4.second)
            b4 = true;
    }
    if(!enemyWaveData5.empty()){
        current5 = enemyWaveData5.front();
        if (ticks5 >= current5.second)
            b5 = true;
    }
    if(b1){
        ticks1 -= current1.second;
        enemyWaveData1.pop_front(); }
    if(b2){
        ticks2 -= current2.second;
        enemyWaveData2.pop_front(); }
    if(b3){
        ticks3 -= current3.second;
        enemyWaveData3.pop_front(); }
    if(b4){
        ticks4 -= current4.second;
        enemyWaveData4.pop_front(); }
    if(b5){
        ticks5 -= current5.second;
        enemyWaveData5.pop_front(); }
    
	Enemy *enemy1, *enemy2, *enemy3, *enemy4, *enemy5;
    if(b1){
        switch (current1.first) {
        case 0:
            EnemyGroup->AddNewObject(enemy1 = new SoldierEnemy(w+BlockSize, 1 * BlockSize + BlockSize / 2));
            break;
        case 1:
            EnemyGroup->AddNewObject(enemy1 = new PlaneEnemy(w+BlockSize, 1 * BlockSize + BlockSize / 2));
            break;
        case 2:
            EnemyGroup->AddNewObject(enemy1 = new TankEnemy(w+BlockSize, 1 * BlockSize + BlockSize / 2));
            break;
        // TODO 2 (7/8): You need to modify 'resources/enemy1.txt', or 'resources/enemy2.txt' to spawn the 4th enemy.
        //         The format is "[EnemyId] [TimeDelay] [Repeat]".
        // TODO 2 (8/8): Enable the creation of the 4th enemy.
        case 3:
            EnemyGroup->AddNewObject(enemy1 = new MyEnemy(w+BlockSize, 1 * BlockSize + BlockSize / 2));
            break;
        case 4:
            EnemyGroup->AddNewObject(enemy1 = new Boss(w+BlockSize, 1 * BlockSize + BlockSize / 2));
            break;
        default:
            return;
        }
        enemy1->UpdatePath(mapDistance);
        // Compensate the time lost.
        enemy1->Update(ticks1);
    }
    if(b2){
        switch (current2.first) {
            case 0:
                EnemyGroup->AddNewObject(enemy2 = new SoldierEnemy(w+BlockSize, 2 * BlockSize + BlockSize / 2));
                break;
            case 1:
                EnemyGroup->AddNewObject(enemy2 = new PlaneEnemy(w+BlockSize, 2 * BlockSize + BlockSize / 2));
                break;
            case 2:
                EnemyGroup->AddNewObject(enemy2 = new TankEnemy(w+BlockSize, 2 * BlockSize + BlockSize / 2));
                break;
            case 3:
                EnemyGroup->AddNewObject(enemy2 = new MyEnemy(w+BlockSize, 2 * BlockSize + BlockSize / 2));
                break;
            case 4:
                EnemyGroup->AddNewObject(enemy2 = new Boss(w+BlockSize, 2 * BlockSize + BlockSize / 2));
                break;
            default:
                return;
        }
        enemy2->UpdatePath(mapDistance);
        enemy2->Update(ticks2);
    }
    if(b3){
        switch (current3.first) {
            case 0:
                EnemyGroup->AddNewObject(enemy3 = new SoldierEnemy(w+BlockSize, 3 * BlockSize + BlockSize / 2));
                break;
            case 1:
                EnemyGroup->AddNewObject(enemy3 = new PlaneEnemy(w+BlockSize, 3 * BlockSize + BlockSize / 2));
                break;
            case 2:
                EnemyGroup->AddNewObject(enemy3 = new TankEnemy(w+BlockSize, 3 * BlockSize + BlockSize / 2));
                break;
            case 3:
                EnemyGroup->AddNewObject(enemy3 = new MyEnemy(w+BlockSize, 3 * BlockSize + BlockSize / 2));
                break;
            case 4:
                EnemyGroup->AddNewObject(enemy3 = new Boss(w+BlockSize, 3 * BlockSize + BlockSize / 2));
                break;
            default:
                return;
        }
        enemy3->UpdatePath(mapDistance);
        enemy3->Update(ticks3);
    }
    if(b4){
        switch (current4.first) {
            case 0:
                EnemyGroup->AddNewObject(enemy4 = new SoldierEnemy(w+BlockSize, 4 * BlockSize + BlockSize / 2));
                break;
            case 1:
                EnemyGroup->AddNewObject(enemy4 = new PlaneEnemy(w+BlockSize, 4 * BlockSize + BlockSize / 2));
                break;
            case 2:
                EnemyGroup->AddNewObject(enemy4 = new TankEnemy(w+BlockSize, 4 * BlockSize + BlockSize / 2));
                break;
            case 3:
                EnemyGroup->AddNewObject(enemy4 = new MyEnemy(w+BlockSize, 4 * BlockSize + BlockSize / 2));
                break;
            case 4:
                EnemyGroup->AddNewObject(enemy4 = new Boss(w+BlockSize, 4 * BlockSize + BlockSize / 2));
                break;
            default:
                return;
        }
        enemy4->UpdatePath(mapDistance);
        enemy4->Update(ticks4);
    }
    if(b5){
        switch (current5.first) {
            case 0:
                EnemyGroup->AddNewObject(enemy5 = new SoldierEnemy(w+BlockSize, 5 * BlockSize + BlockSize / 2));
                break;
            case 1:
                EnemyGroup->AddNewObject(enemy5 = new PlaneEnemy(w+BlockSize, 5 * BlockSize + BlockSize / 2));
                break;
            case 2:
                EnemyGroup->AddNewObject(enemy5 = new TankEnemy(w+BlockSize, 5 * BlockSize + BlockSize / 2));
                break;
            case 3:
                EnemyGroup->AddNewObject(enemy5 = new MyEnemy(w+BlockSize, 5 * BlockSize + BlockSize / 2));
                break;
            case 4:
                EnemyGroup->AddNewObject(enemy5 = new Boss(w+BlockSize, 5 * BlockSize + BlockSize / 2));
                break;
            default:
                return;
        }
        enemy5->UpdatePath(mapDistance);
        enemy5->Update(ticks5);
    }
}
void PlayScene::Draw() const {
	IScene::Draw();
    Group::Draw();
    
    if(lives > 0){
        al_draw_filled_rectangle(975, 70, 975 + lives*40, 90, al_map_rgb(205, 55, 0));
        al_draw_rectangle(975, 70, 1375, 90, al_map_rgb(0, 0, 0), 2);
    }
    
	if (DebugMode) {
		// Draw reverse BFS distance on all reachable blocks.
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
					label.Anchor = Engine::Point(0.5, 0.5);
					label.Draw();
				}
			}
		}
	}
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible)
		return;
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
		if (mapState[y][x] != TILE_OCCUPIED) {
			if (!preview)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			EarnMoney(-preview->GetPrice());
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			TowerGroup->AddNewObject(preview);
			// Remove Preview.
			preview = nullptr;

			mapState[y][x] = TILE_OCCUPIED;
			OnMouseMove(mx, my);
		}
	}
}
void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
		DebugMode = !DebugMode;
	}
	else {
		keyStrokes.push_back(keyCode);
		if (keyStrokes.size() > code.size())
			keyStrokes.pop_front();
		if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
			auto it = keyStrokes.begin();
			for (int c : code) {
				if (*it != c && (c == ALLEGRO_KEYMOD_SHIFT && *it != ALLEGRO_KEY_LSHIFT && *it != ALLEGRO_KEY_RSHIFT))
					return;
				++it;
			}
			EffectGroup->AddNewObject(new Plane());
			money += 100;
		}
	}
	if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for MachineGunTurret.
		UIBtnClicked(0);
	}
	else if (keyCode == ALLEGRO_KEY_W) {
		// Hotkey for LaserTurret.
		UIBtnClicked(1);
	}
	else if (keyCode == ALLEGRO_KEY_E) {
		// Hotkey for MissileTurret.
		UIBtnClicked(2);
	}
	// TODO 2 (5/8): Make the R key to create the 4th turret.
    else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for NewTurret.
        UIBtnClicked(3);
    }
    else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for whiteTurret.
        UIBtnClicked(4);
    }
    else if (keyCode == ALLEGRO_KEY_Y) {
        // Hotkey for blackTurret.
        UIBtnClicked(5);
    }
	else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
}
void PlayScene::Hit() {
	lives--;
	//UILives->Text = std::string("Life ") + std::to_string(lives);
	if (lives <= 0) {
		Engine::GameEngine::GetInstance().ChangeScene("lose");
	}
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	UIMoney->Text = std::string("M. ") + std::to_string(this->money);
}
void PlayScene::ReadMap() {
	std::string filename = std::string("resources/map") + std::to_string(MapId) + ".txt";
	// Read map file.
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
		case '0': mapData.push_back(false); break;
		case '1': mapData.push_back(true); break;
		case '\n':
		case '\r':
			if (static_cast<int>(mapData.size()) / MapWidth != 0)
				throw std::ios_base::failure("Map data is corrupted.");
			break;
		default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();

	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
		}
	}
}

void PlayScene::ReadEnemyWave() {
	std::string filename = std::string("resources/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type1, wait1, repeat1;
    float type2, wait2, repeat2;
    float type3, wait3, repeat3;
    float type4, wait4, repeat4;
    float type5, wait5, repeat5;
	enemyWaveData1.clear();
    enemyWaveData2.clear();
    enemyWaveData3.clear();
    enemyWaveData4.clear();
    enemyWaveData5.clear();
	std::ifstream fin(filename);
	while (fin >> type1 && fin >> wait1 && fin >> repeat1 && fin >> type2 && fin >> wait2 && fin >> repeat2 && fin >> type3 && fin >> wait3 && fin >> repeat3 && fin >> type4 && fin >> wait4 && fin >> repeat4 && fin >> type5 && fin >> wait5 && fin >> repeat5  ) {
		for (int i = 0; i < repeat1; i++)
			enemyWaveData1.emplace_back(type1, wait1);
        for (int i = 0; i < repeat2; i++)
            enemyWaveData2.emplace_back(type2, wait2);
        for (int i = 0; i < repeat3; i++)
            enemyWaveData3.emplace_back(type3, wait3);
        for (int i = 0; i < repeat4; i++)
            enemyWaveData4.emplace_back(type4, wait4);
        for (int i = 0; i < repeat5; i++)
            enemyWaveData5.emplace_back(type5, wait5);
	}
	fin.close();
}

void PlayScene::ConstructUI() {
	// Text
	if(MapId == 1)
        UIGroup->AddNewObject(new Engine::Label(std::string("Easy"), "pirulen.ttf", 35, 900, 10));
    else if (MapId == 2)
        UIGroup->AddNewObject(new Engine::Label(std::string("Hard"), "pirulen.ttf", 35, 900, 10));
    
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("M. ") + std::to_string(money), "pirulen.ttf", 20, 43, 105));
	UIGroup->AddNewObject(UILives = new Engine::Label(std::string("HP "), /*+ std::to_string(lives),*/ "pirulen.ttf", 30, 900, 61));
    
    TurretButton* btn;
	// Button 1
	btn = new TurretButton("play/card30 2.png", "play/card30 1.png",
		Engine::Sprite("play/30card.png", 190, 10 - 3, 0, 0, 0, 0)
		, 170, 10, MachineGunTurret::Price);
	// Reference: Class Member Function Pointer and std::bind.
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
	UIGroup->AddNewControlObject(btn);
    
	// Button 2
	btn = new TurretButton("play/card50 2.png", "play/card50 1.png",
		Engine::Sprite("play/50card.png", 310, 10 - 3, 0, 0, 0, 0)
		, 290, 10, LaserTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
	UIGroup->AddNewControlObject(btn);
    
	// Button 3
	btn = new TurretButton("play/card80 2.png", "play/card80 1.png",
		Engine::Sprite("play/80card.png", 435, 10 - 3, 0, 0, 0, 0)
		, 410, 10, MissileTurret::Price);
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
	UIGroup->AddNewControlObject(btn);
	
    // Button 4
    btn = new TurretButton("play/card100 2.png", "play/card100 1.png",
        Engine::Sprite("play/100card.png", 542, 10 - 3, 0, 0, 0, 0)
        , 530, 10, NewTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
    UIGroup->AddNewControlObject(btn);
    
    // Button 5
    btn = new TurretButton("play/card125 2.png", "play/card125 1.png",
        Engine::Sprite("play/125card.png", 675, 10 - 3, 0, 0, 0, 0)
        , 650, 10, whiteTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
    UIGroup->AddNewControlObject(btn);
    
    // Button 6
    btn = new TurretButton("play/card200 2.png", "play/card200 1.png",
        Engine::Sprite("play/200card.png", 795, 10 - 3, 0, 0, 0, 0)
        , 770, 10, blackTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 5));
    UIGroup->AddNewControlObject(btn);
}

void PlayScene::UIBtnClicked(int id) {
	if (preview)
		UIGroup->RemoveObject(preview->GetObjectIterator());
	if (id == 0 && money >= MachineGunTurret::Price)
		preview = new MachineGunTurret(0, 0);
	else if (id == 1 && money >= LaserTurret::Price)
		preview = new LaserTurret(0, 0);
	else if (id == 2 && money >= MissileTurret::Price)
		preview = new MissileTurret(0, 0);
	// TODO 2 (4/8): On callback, create the 4th tower.
    else if (id == 3 && money >= NewTurret::Price)
        preview = new NewTurret(0,0);
    else if (id == 4 && money >= whiteTurret::Price)
        preview = new whiteTurret(0,0);
    else if (id == 5 && money >= blackTurret::Price)
        preview = new blackTurret(0,0);
    
	if (!preview)
		return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	UIGroup->AddNewObject(preview);
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0 || pnt.x >= MapWidth || pnt.y < 0 || pnt.y >= MapHeight)
			continue;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));

	if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
		return map;
	
    for(int i=0; i<MapWidth; i++){
        for(int j=0; j<MapHeight; j++){
            map[j][i] = i;
        }
    }
	return map;
}
