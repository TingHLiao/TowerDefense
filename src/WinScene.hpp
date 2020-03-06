#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "IScene.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include <iostream>

class WinScene final : public Engine::IScene {
private:
    float ticks;
    int n,m=0;
public:
    explicit WinScene() = default;
    void Initialize() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    IObject* l;
    bool c = true, b = false;
    int w, h, halfW, halfH ;
};

#endif // WINSCENE_HPP
