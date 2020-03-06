#include "StageSelectScene.hpp"
#include "PlayScene.hpp"
#include "AudioHelper.hpp"
#include "Slider.hpp"
#include "LOG.hpp"

void StageSelectScene::Initialize() {
    w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
    
    AddNewObject(new Engine::Label("Mode", "pirulen.ttf", 52, halfW, halfH - 300, 0, 0, 0, 255, 0.5, 0.5));
    
    Engine::ImageButton* btn;
    
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH - 120, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("easy", "pirulen.ttf", 48, halfW, halfH - 70, 255, 255, 255, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH  + 120, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("hard", "pirulen.ttf", 48, halfW, halfH + 170 , 255, 255, 255, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("stage-select/dirt_1.png", "stage-select/floor_1.png", halfW + 500, halfH * 3 / 2 + 140, 200, 75);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("setting", "pirulen.ttf", 30, halfW + 600, halfH * 3 / 2 + 180, 0, 0, 0, 255, 0.5, 0.5));
    
    AudioHelper::PlayBGM("select.ogg");
    l = new Engine::Image("gif/gif3-1.tiff", halfW + 460, halfH + 80);
    AddNewObject(l);
}
void StageSelectScene::Update(float deltaTime) {
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
        l = new Engine::Image("gif/gif3-1.tiff", halfW + 460, halfH + 80);
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
        l = new Engine::Image("gif/gif3-" + std::to_string(n+1) + ".tiff", halfW + 460, halfH + 80);
        AddNewObject(l);
    }
}
void StageSelectScene::PlayOnClick(int stage) {
    if(stage == 1 || stage == 2){
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
        scene->MapId = stage;
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else
        Engine::GameEngine::GetInstance().ChangeScene("setting");
}
void StageSelectScene::BGMSlideOnValueChanged(float value) {
    al_set_sample_instance_gain(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
