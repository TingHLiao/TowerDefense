#include "SettingScene.hpp"
#include "Label.hpp"
#include "AudioHelper.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "PlayScene.hpp"
#include "Slider.hpp"
#include "LOG.hpp"

void SettingScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    
    AddNewObject(new Engine::Label("Setting", "pirulen.ttf", 52, halfW, halfH - 300, 0, 0, 0, 255, 0.5, 0.5));
    
    Engine::ImageButton* btn;
    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(halfW - 75, halfH - 100, 250, 8);
    sliderBGM->SetOnValueChangedCallback(std::bind(&SettingScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(new Engine::Label("BGM : ", "pirulen.ttf", 35, halfW - 60 - 95, halfH - 100, 0, 0, 0, 255, 0.5, 0.5));
    sliderSFX = new Slider(halfW - 75, halfH + 100, 250, 8);
    sliderSFX->SetOnValueChangedCallback(std::bind(&SettingScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(new Engine::Label("SFX : ", "pirulen.ttf", 35, halfW - 60 - 95, halfH + 100, 0, 0, 0, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("stage-select/dirt_1.png", "stage-select/floor_1.png", halfW + 500, halfH * 3 / 2 + 140, 200, 75);
    btn->SetOnClickCallback(std::bind(&SettingScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 30, halfW + 600, halfH * 3 / 2 + 180, 0, 0, 0, 255, 0.5, 0.5));
    
    bgmInstance = al_create_sample_instance(Engine::Resources::GetInstance().GetSample("select.ogg").get());
    al_set_sample_instance_playmode(bgmInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(bgmInstance, al_get_default_mixer());
    al_play_sample_instance(bgmInstance);
    
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void SettingScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void SettingScene::BGMSlideOnValueChanged(float value) {
    al_set_sample_instance_gain(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void SettingScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
