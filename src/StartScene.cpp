#include "StartScene.hpp"
#include "Label.hpp"
#include "AudioHelper.hpp"

void StartScene::Initialize() {
    
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("start/start_bg.jpg",0, 0, w, h));
    AddNewObject(new Engine::Label("Are you ready ? : )", "pirulen.ttf", 48, halfW, halfH + 150, 0, 0, 0, 255, 0.5, 0.5));
    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 250, halfH * 3 / 2 - 50, 500, 125);
    btn->SetOnClickCallback(std::bind(&StartScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Of course!", "pirulen.ttf", 52, halfW, halfH * 3 / 2 + 10, 255, 255, 255, 255, 0.5, 0.5));
    AudioHelper::PlayAudio("lose.wav");
}
void StartScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
