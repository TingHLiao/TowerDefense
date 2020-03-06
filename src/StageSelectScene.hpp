#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP
#include "IScene.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"

class StageSelectScene final : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_INSTANCE* bgmInstance;
    float ticks;
    int n,m=0;
public:
	explicit StageSelectScene() = default;
	void Initialize() override;
	void PlayOnClick(int stage);
	void BGMSlideOnValueChanged(float value);
	void SFXSlideOnValueChanged(float value);
    void Update(float deltaTime) override;
    IObject* l;
    bool c = true, b = false;
    int w, h, halfW, halfH ;
};

#endif // STAGESELECTSCENE_HPP
