#include "LoseScene.hpp"
#include "Label.hpp"
#include "AudioHelper.hpp"
#include <iostream>

void LoseScene::Initialize() {
	// TODO 1 (2/2): You can imitate the 2 files: 'LoseScene.hpp', 'LoseScene.cpp' to implement your start scene.
	w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	halfW = w / 2;
	halfH = h / 2;
	AddNewObject(new Engine::Label("Oh sorry... :(", "pirulen.ttf", 52, halfW, halfH - 300, 0, 0, 0, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&LoseScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("play again", "pirulen.ttf", 40, halfW, halfH * 3 / 2, 255, 255, 255, 255, 0.5, 0.5));
	AudioHelper::PlayAudio("lose.wav");
    l = new Engine::Image("gif/gif2-1.png", halfW - 160, halfH - 15);
    AddNewObject(l);
}
void LoseScene::Update(float deltaTime) {
    std::cout << "update" << '\n';
    ticks += deltaTime*5;
    if(ticks>11){
        ticks = 0.157315;
        m = 0;
    }
    n = ticks;
    /*if (ticks > 4 && ticks < 100 &&
     dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
     ticks = 100;
     AudioHelper::PlayBGM("happy.ogg");
     }*/
    if(ticks>=0 && ticks<0.2){
        RemoveObject(l->GetObjectIterator());
        l = new Engine::Image("gif/gif2-1.png", halfW - 160, halfH - 15);
        AddNewObject(l);
    }
    else if(ticks > n && c){
        c = false;
        b = true;
        m = n+1;
    }
    else if(ticks > m && b){
        //Remove
        RemoveObject(l->GetObjectIterator());
        b = false;
        c = true;
        l = new Engine::Image("gif/gif2-" + std::to_string(n+1) + ".tiff", halfW - 160, halfH - 15);
        AddNewObject(l);
    }
}
void LoseScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
