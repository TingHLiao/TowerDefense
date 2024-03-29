#ifndef SLIDER_HPP
#define SLIDER_HPP
#include "ImageButton.hpp"

class Slider : public Engine::ImageButton {  //繼承Engine::ImageButton ->可用裡面的protected 
private:
	Image Bar;
	Image End1;
	Image End2;
	std::function<void(float value)> OnValueChangedCallback;
	float value = 0;
public:
	const float Min = 0;
	const float Max = 1;
	bool Down = false;
	Slider(float x, float y, float w, float h);
	void Draw() const override;
	void SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback);
	void SetValue(float value);
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
};
#endif // SLIDER_HPP
