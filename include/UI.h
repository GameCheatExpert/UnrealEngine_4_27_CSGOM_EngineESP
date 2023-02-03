#include <data.h>
#include <android_native_app_glue.h>

using namespace Data;

namespace UI{

    extern FVector2D MousePos;
    extern bool MouseDown;
    extern float ScaleX, ScaleY;

    void SetupUI(AHUD* _hud, float ScaleX, float ScaleY);

	bool Window(char* name, FVector2D* pos, FVector2D size, bool& isOpen, float& TempValue);
	void Text(char* text);
	bool ButtonTab(char* name, FVector2D size, bool active);
	bool Button(char* name, FVector2D size);
	bool Checkbox(char* name, bool* value);
	void SliderInt(char* name, int* value, int min, int max);
	void SliderFloat(char* name, float* value, float min, float max, char* format);

	void SameLine();
	void PushNextElementY(float y, bool from_last_element);
	void NextColumn(float x);
	void ClearFirstPos();
	
	void Draw_Cursor(bool Toggle);

	void onEvent(AInputEvent* input_event);
	
}