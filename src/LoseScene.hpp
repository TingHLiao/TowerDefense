#ifndef LOSESCENE_HPP
#define LOSESCENE_HPP
#include "IScene.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"

class LoseScene final : public Engine::IScene {
    float ticks;
    int n,m=0;
public:
    explicit LoseScene() = default;
    void Initialize() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    IObject* l;
    bool c = true, b = false;
    int w, h, halfW, halfH ;
};


#endif // LOSESCENE_HPP
