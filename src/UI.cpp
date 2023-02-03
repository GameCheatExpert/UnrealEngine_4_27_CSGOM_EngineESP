#include <UI.h>

typedef unsigned char BYTE;
typedef uint32_t UINT32;

namespace UI{
    AHUD* hud;
	UCanvas* canvas;
	float ScaleX, ScaleY;
	
	bool mouseDownAlready[256];

    FVector2D MousePos = {0,0};
	bool MouseDown = false;


	bool IsMouseClicked(int button, int element_id, bool repeat)
	{
		if (MouseDown)
		{
			if (!mouseDownAlready[element_id])
			{
				mouseDownAlready[element_id] = true;
				return true;
			}
			if (repeat)
				return true;
		}
		else
		{
			mouseDownAlready[element_id] = false;
		}
		return false;
	}
	
    void SetupUI(AHUD* _hud, float _ScaleX, float _ScaleY){
        hud = _hud;
        canvas = hud->Canvas;
        ScaleX = _ScaleX;
        ScaleY = _ScaleY;
    }
    
    namespace Colors
	{
		FLinearColor Text{ 1.0f, 1.0f, 1.0f, 1.0f };
		FLinearColor Text_Shadow{ 0.0f, 0.0f, 0.0f, 0.0f };
		FLinearColor Text_Outline{ 0.0f, 0.0f, 0.0f, 0.30f };

		FLinearColor Window_Background{ 0.009f, 0.009f, 0.009f, 1.0f };
		FLinearColor Window_Header{ 0.10f, 0.15f, 0.84f, 1.0f };

		FLinearColor Button_Idle{ 0.10f, 0.15f, 0.84f, 1.0f };
		FLinearColor Button_Hovered{ 0.15f, 0.20f, 0.89f, 1.0f };
		FLinearColor Button_Active{ 0.20f, 0.25f, 0.94f, 1.0f };

		FLinearColor Checkbox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		FLinearColor Checkbox_Hovered{ 0.22f, 0.30f, 0.72f, 1.0f };
		FLinearColor Checkbox_Enabled{ 0.20f, 0.25f, 0.94f, 1.0f };

		FLinearColor Combobox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		FLinearColor Combobox_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		FLinearColor Combobox_Elements{ 0.239f, 0.42f, 0.82f, 1.0f };

		FLinearColor Slider_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		FLinearColor Slider_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		FLinearColor Slider_Progress{ 0.22f, 0.30f, 0.72f, 1.0f };
		FLinearColor Slider_Button{ 0.10f, 0.15f, 0.84f, 1.0f };

		FLinearColor ColorPicker_Background{ 0.006f, 0.006f, 0.006f, 1.0f };
	}

	


	bool hover_element = false;
	FVector2D menu_pos = FVector2D{ 0, 0 };
	float offset_x = 0.0f;
	float offset_y = 0.0f;

	FVector2D first_element_pos = FVector2D{ 0, 0 };

	FVector2D last_element_pos = FVector2D{ 0, 0 };
	FVector2D last_element_size = FVector2D{ 0, 0 };

	int current_element = -1;
	FVector2D current_element_pos = FVector2D{ 0, 0 };
	FVector2D current_element_size = FVector2D{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;

	bool pushY = false;
	float pushYvalue = 0.0f;
	
	


	FVector2D CursorPos()
	{
	    return MousePos;
	}
	bool MouseInZone(FVector2D pos, FVector2D size)
	{
		FVector2D cursor_pos = CursorPos();
		if (cursor_pos.X > pos.X && cursor_pos.Y > pos.Y)
			if (cursor_pos.X < pos.X + size.X && cursor_pos.Y < pos.Y + size.Y)
				return true;

		return false;
	}

	void Draw_Cursor(bool toogle)
	{
		if (toogle)
		{
			FVector2D cursorPos = CursorPos();
			canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + 35, cursorPos.Y + 10 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });


			int x = 35;
			int y = 10;
			while (y != 30) //20 steps
			{
				x -= 1; if (x < 15) x = 15;
				y += 1; if (y > 30) y = 30;

				canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + x, cursorPos.Y + y }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
			}

			canvas->K2_DrawLine(FVector2D{ cursorPos.X, cursorPos.Y }, FVector2D{ cursorPos.X + 15, cursorPos.Y + 30 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
			canvas->K2_DrawLine(FVector2D{ cursorPos.X + 35, cursorPos.Y + 10 }, FVector2D{ cursorPos.X + 15, cursorPos.Y + 30 }, 1, FLinearColor{ 0.30f, 0.30f, 0.80f, 1.0f });
		}
	}

	void SameLine()
	{
		sameLine = true;
	}
	void PushNextElementY(float y, bool from_last_element = true)
	{
		pushY = true;
		if (from_last_element)
			pushYvalue = last_element_pos.Y + last_element_size.Y + y;
		else
			pushYvalue = y;
	}
	void NextColumn(float x)
	{
		offset_x = x;
		PushNextElementY(first_element_pos.Y, false);
	}
	void ClearFirstPos()
	{
		first_element_pos = FVector2D{ 0, 0 };
	}

	void TextLeft(char* name, FVector2D pos, FLinearColor color)
	{
		//int length = strlen(name) + 1;
		//canvas->K2_DrawText(Functions::NamesFont, FString{ s2wc(name), length, length }, pos, FVector2D{ 0.97f, 0.97f }, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, false, true, true, Colors::Text_Outline);
	    auto StrBuf = FString(name);
	    float Width, Height;
	    hud->GetTextSize(StrBuf, Width, Height, 1.0f);
	    hud->DrawText(FString(name),pos.X, pos.Y-Height/2, color);
	}
	void TextCenter(char* name, FVector2D pos, FLinearColor color)
	{
		//int length = strlen(name) + 1;
		//canvas->K2_DrawText(Functions::NamesFont, FString{ s2wc(name), length, length }, pos, FVector2D{ 0.97f, 0.97f }, color, false, Colors::Text_Shadow, FVector2D{ pos.X + 1, pos.Y + 1 }, true, true, true, Colors::Text_Outline);
	    auto StrBuf = FString(name);
	    float Width, Height;
	    hud->GetTextSize(StrBuf, Width, Height, 1.0f);
	    //LOGD("%f %f", Width, Height);
	    hud->DrawText(StrBuf,pos.X-Width/2, pos.Y-Height/2, color);
	}

	void GetColor(FLinearColor* color, float* r, float* g, float* b, float* a)
	{
		*r = color->R;
		*g = color->G;
		*b = color->B;
		*a = color->A;
	}
	UINT32 GetColorUINT(int r, int g, int b, int a)
	{
		UINT32 result = (BYTE(a) << 24) + (BYTE(r) << 16) + (BYTE(g) << 8) + BYTE(b);
		return result;
	}

	void Draw_Line(FVector2D from, FVector2D to, int thickness, FLinearColor color)
	{
		canvas->K2_DrawLine(FVector2D{ from.X, from.Y }, FVector2D{ to.X, to.Y }, thickness, color);
	}
	void drawFilledRect(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.0f, color);
	}
	void DrawFilledCircle(FVector2D pos, float r, FLinearColor color)
	{
		float smooth = 0.07f;

		double PI = 3.14159265359;
		int size = (int)(2.0f * PI / smooth) + 1;

		float angle = 0.0f;
		int i = 0;

		for (; angle < 2 * PI; angle += smooth, i++)
		{
			Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ pos.X + cosf(angle) * r, pos.Y + sinf(angle) * r }, 1.0f, color);
		}
	}
	void DrawCircle(FVector2D pos, int radius, int numSides, FLinearColor Color)
	{
		float PI = 3.1415927f;

		float Step = PI * 2.0 / numSides;
		int Count = 0;
		FVector2D V[128];
		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = radius * cos(a) + pos.X;
			float Y1 = radius * sin(a) + pos.Y;
			float X2 = radius * cos(a + Step) + pos.X;
			float Y2 = radius * sin(a + Step) + pos.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			//Draw_Line(FVector2D{ pos.X, pos.Y }, FVector2D{ X2, Y2 }, 1.0f, Color); // Points from Centre to ends of circle
			Draw_Line(FVector2D{ V[Count].X, V[Count].Y }, FVector2D{ X2, Y2 }, 1.0f, Color);// Circle Around
		}
	}

	FVector2D dragPos;
	FVector2D movePos;
	FVector2D origPos;
	bool Window(char* name, FVector2D* pos, FVector2D size, bool& isOpen, float& tempValue)
	{
		elements_count = 0;
		
	
			

		bool isHovered = MouseInZone(FVector2D{ pos->X, pos->Y }, isOpen ? size : FVector2D(size.X, 25.0f));

		//Drop last element
		if (current_element != -1 && !MouseDown)
		{
			current_element = -1;
		}

		//Drag
		if (hover_element && MouseDown)
		{

		}
		else if ((isHovered || dragPos.X != 0) && !hover_element)
		{
			if (IsMouseClicked(0, elements_count, true))
			{
				FVector2D cursorPos = CursorPos();

				cursorPos.X -= size.X;
				cursorPos.Y -= size.Y;

				if (dragPos.X == 0)
				{
					dragPos.X = (cursorPos.X - pos->X);
					dragPos.Y = (cursorPos.Y - pos->Y);
					origPos = {cursorPos.X - dragPos.X, cursorPos.Y - dragPos.Y};
					// 判断点击点是否在标题栏内
    		        bool isClickedTitle = MouseInZone(FVector2D{ pos->X, pos->Y }, {size.X, 25.0f});
    		        if(isClickedTitle){
    		            tempValue = 1.0f;
    		        }else{
    		            tempValue = 0.0f;
    		        }
				}
				
				
				pos->X = cursorPos.X - dragPos.X;
				pos->Y = cursorPos.Y - dragPos.Y;
				
			
				
			}
			else
			{
			

				dragPos = FVector2D{ 0, 0 };
				if(tempValue && abs(origPos.X - pos->X) <= 10 && abs(origPos.Y - pos->Y) <= 10){
				    tempValue = 0;
				    isOpen = !isOpen;
				}
			}
		}
		else
		{
			hover_element = false;
		}


		offset_x = 0.0f; offset_y = 0.0f;
		menu_pos = FVector2D{ pos->X, pos->Y };
		first_element_pos = FVector2D{ 0, 0 };
		current_element_pos = FVector2D{ 0, 0 };
		current_element_size = FVector2D{ 0, 0 };
	    
	    if (isOpen)
	        // Bg
	        drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, size.Y, Colors::Window_Background);

	    
	    //Header
		drawFilledRect(FVector2D{ pos->X, pos->Y }, size.X, 25.0f, Colors::Window_Header);

		offset_y += 25.0f;

		//Title
		FVector2D titlePos = FVector2D{ pos->X + size.X / 2, pos->Y + 25 / 2 };
		TextCenter(name, titlePos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
		
	
		//drawFilledRect(FVector2D{ pos->X, pos->Y }, 122, size.Y, FLinearColor{ 0.006f, 0.006f, 0.006f, 1.0f });//My tabs bg

	
        

		return isOpen;
	}
	
	void Text(char* text)
	{
		elements_count++;

		float size = 25;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}

		if (!sameLine)
			offset_y += size + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + 5.0f, pos.Y + size / 2 };
		//if (center)
			TextCenter(text, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
		//else
		//	TextLeft(text, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	bool ButtonTab(char* name, FVector2D size, bool active)
	{
		elements_count++;

		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);

		//Bg
		if (active)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Active);
		}
		else if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
		TextCenter(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
		
	

		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;

		if (isHovered && IsMouseClicked(0, elements_count, false))
			return true;

		return false;
	}
	bool Button(char* name, FVector2D size)
	{
		elements_count++;
		
		FVector2D padding = FVector2D{ 5, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, size);
		
		//Bg
		if (isHovered)
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Hovered);
			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y }, size.X, size.Y, Colors::Button_Idle);
		}
		
		if (!sameLine)
			offset_y += size.Y + padding.Y;
		
		//Text
		FVector2D textPos = FVector2D{ pos.X + size.X / 2, pos.Y + size.Y / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
			TextCenter(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
		
		
		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
		
		if (isHovered && IsMouseClicked(0, elements_count, false))
			return true;
		
		return false;
	}
	bool Checkbox(char* name, bool* value)
	{
		elements_count++;
		
		float size = 30;
		FVector2D padding = FVector2D{ 10, 10 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y }, FVector2D{ size, size });
		
		//Bg
		if (isHovered)
		{
			hover_element = true;
		}
		drawFilledRect(FVector2D{ pos.X, pos.Y }, size, size, Colors::Checkbox_Idle);
	
		if (!sameLine)
			offset_y += size + padding.Y;
	
		if (*value)
		{
			drawFilledRect(FVector2D{ pos.X + 3, pos.Y + 3 }, size - 6, size - 6, Colors::Checkbox_Enabled);
			//drawFilledRect(FVector2D{ pos.X + 9, pos.Y + 9 }, size - 18, size - 18, Colors::Checkbox_Hovered);
		}
		
		
		
		//Text
		FVector2D textPos = FVector2D{ pos.X + size + 5.0f, pos.Y + size / 2 };
		//if (!TextOverlapedFromActiveElement(textPos))
			TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
		
		
		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
		
		if (isHovered && IsMouseClicked(0, elements_count, false)){
			*value = !*value;
			return true;
	    }
	    return false;
	}
	void SliderInt(char* name, int* value, int min, int max)
	{
		elements_count++;

		FVector2D size = FVector2D{ 240, 50 };
		FVector2D slider_size = FVector2D{ 200, 10 };
		FVector2D padding = FVector2D{ 10, 15 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size);

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Bg
		if (isHovered || current_element == elements_count)
		{
			//Drag
			if (IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y + 5.0f }, 5.0f, 5.0f, Colors::Slider_Progress);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y + 5.0f }, 5.0f, 5.0f, Colors::Slider_Progress);
		}


		//Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		//drawFilledRect(FVector2D{ pos.X + oneP * (*value - min) - 10.0f, pos.Y + slider_size.Y - 5.0f + padding.Y }, 20.0f, 20.0f, Colors::Slider_Button);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 3.3f + padding.Y }, 10.0f, Colors::Slider_Button);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 3.3f + padding.Y }, 5.0f, Colors::Slider_Progress);

		char buffer[32];
		sprintf(buffer, "%i", *value);
		FVector2D valuePos = FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 25 + padding.Y };
		TextCenter(buffer, valuePos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });

		//Text
		FVector2D textPos = FVector2D{ pos.X + 5, pos.Y + 10 };
		TextLeft(name, textPos, FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	void SliderFloat(char* name, float* value, float min, float max, char* format = "%.0f")
	{
		elements_count++;

		FVector2D size = FVector2D{ 210, 40 };
		FVector2D slider_size = FVector2D{ 170, 7 };
		FVector2D adjust_zone = FVector2D{ 0, 20 };
		FVector2D padding = FVector2D{ 10, 15 };
		FVector2D pos = FVector2D{ menu_pos.X + padding.X + offset_x, menu_pos.Y + padding.Y + offset_y };
		if (sameLine)
		{
			pos.X = last_element_pos.X + last_element_size.X + padding.X;
			pos.Y = last_element_pos.Y;
		}
		if (pushY)
		{
			pos.Y = pushYvalue;
			pushY = false;
			pushYvalue = 0.0f;
			offset_y = pos.Y - menu_pos.Y;
		}
		bool isHovered = MouseInZone(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y - adjust_zone.Y }, FVector2D{ slider_size.X, slider_size.Y + adjust_zone.Y * 1.5f });

		if (!sameLine)
			offset_y += size.Y + padding.Y;

		//Bg
		if (isHovered || current_element == elements_count)
		{
			//Drag
			
			if (IsMouseClicked(0, elements_count, true))
			{
				current_element = elements_count;

				FVector2D cursorPos = CursorPos();
				*value = ((cursorPos.X - pos.X) * ((max - min) / slider_size.X)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Hovered);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Hovered);

			hover_element = true;
		}
		else
		{
			drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, slider_size.X, slider_size.Y, Colors::Slider_Idle);
			DrawFilledCircle(FVector2D{ pos.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Progress);
			DrawFilledCircle(FVector2D{ pos.X + slider_size.X, pos.Y + padding.Y + 9.3f }, 3.1f, Colors::Slider_Idle);
		}


		//Text
		FVector2D textPos = FVector2D{ pos.X, pos.Y + 5 };
		TextLeft(name, textPos, Colors::Text);

		//Value
		float oneP = slider_size.X / (max - min);
		drawFilledRect(FVector2D{ pos.X, pos.Y + slider_size.Y + padding.Y }, oneP * (*value - min), slider_size.Y, Colors::Slider_Progress);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 8.0f, Colors::Slider_Button);
		DrawFilledCircle(FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 2.66f + padding.Y }, 4.0f, Colors::Slider_Progress);

		char buffer[32];
		sprintf(buffer, format, *value);
		FVector2D valuePos = FVector2D{ pos.X + oneP * (*value - min), pos.Y + slider_size.Y + 20 + padding.Y };
		TextCenter(buffer, valuePos, Colors::Text);


		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.X == 0.0f)
			first_element_pos = pos;
	}
	
	


	void onEvent(AInputEvent* input_event){
	    auto event_type = AInputEvent_getType(input_event);
	    if(event_type != AINPUT_EVENT_TYPE_MOTION)
	        return; // 仅处理触摸事件
        int32_t event_action = AMotionEvent_getAction(input_event);
        int32_t event_pointer_index = (event_action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        if(event_pointer_index > 0)
            return; // 单指
        event_action &= AMOTION_EVENT_ACTION_MASK;
        switch (event_action) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_UP:
                if ((AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_FINGER) || (AMotionEvent_getToolType(input_event, event_pointer_index) == AMOTION_EVENT_TOOL_TYPE_UNKNOWN)) {
                    MouseDown = (event_action == AMOTION_EVENT_ACTION_DOWN);
                    MousePos = FVector2D(AMotionEvent_getRawX(input_event, event_pointer_index) * ScaleX, AMotionEvent_getRawY(input_event, event_pointer_index) * ScaleY);
                }
                break;
            case AMOTION_EVENT_ACTION_BUTTON_PRESS:
            case AMOTION_EVENT_ACTION_BUTTON_RELEASE: {
                int32_t button_state = AMotionEvent_getButtonState(input_event);
                MouseDown = ((button_state & AMOTION_EVENT_BUTTON_PRIMARY) != 0);

            }
                break;
            case AMOTION_EVENT_ACTION_HOVER_MOVE: // Hovering: Tool moves while NOT pressed (such as a physical mouse)
            case AMOTION_EVENT_ACTION_MOVE: {       // Touch pointer moves while DOWN
                MousePos = FVector2D(AMotionEvent_getRawX(input_event, event_pointer_index) * ScaleX, AMotionEvent_getRawY(input_event, event_pointer_index) * ScaleY);
                break;
            }
 
            default:
                break;
        }
	}






	
    
};