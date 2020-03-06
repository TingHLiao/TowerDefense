#include "WinScene.hpp"
#include "Label.hpp"
#include "AudioHelper.hpp"
#include "PlayScene.hpp"
#include <iostream>

void WinScene::Initialize() {
	ticks = 0;
    w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
	AddNewObject(new Engine::Label("safe!!", "pirulen.ttf", 52, halfW, halfH - 300, 0, 0, 0, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("play again", "pirulen.ttf", 40, halfW, halfH * 3 / 2, 255, 255, 255, 255, 0.5, 0.5));
	AudioHelper::PlayAudio("win.wav");
    l = new Engine::Image("gif/gif-1.png", halfW - 400, halfH - 101);
    AddNewObject(l);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime*10;
    if(ticks>10){
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
        l = new Engine::Image("gif/gif-1.png", halfW - 400, halfH - 101);
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
        l = new Engine::Image("gif/gif-" + std::to_string(n+1) + ".png", halfW - 400, halfH - 101);
        AddNewObject(l);
    }
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
