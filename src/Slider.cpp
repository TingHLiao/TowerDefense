#include "Slider.hpp"
#include <algorithm>

Slider::Slider(float x, float y, float w, float h) :
	ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
	Bar("stage-select/bar.png", x, y, w, h),
	End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
	End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
	Position.x += w;
	Position.y += h / 2;
	Anchor = Engine::Point(0.5, 0.5);
}

void Slider::Draw() const {
    Bar.Draw();
    End1.Draw();
    End2.Draw();
    ImageButton::Draw();
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(float value) {
    if(OnValueChangedCallback){
        OnValueChangedCallback(value);
        if(value > Max) value = Max;
        else if(value < Min) value = Min;
        Position.x = Bar.Position.x + value * Bar.GetBitmapWidth();
    }
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if((button & 1) && mouseIn && Enabled )
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
	// TODO 4 (6/6): Clamp the coordinates and calculate the value. Call 'SetValue' when you're done.
    ImageButton::OnMouseMove(mx, my);
    if(Down)
        SetValue( (mx - Bar.Position.x)/Bar.GetBitmapWidth() );
}
