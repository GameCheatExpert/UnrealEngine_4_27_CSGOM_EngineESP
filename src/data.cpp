#include <data.h>
#include <UI.h>
#include <log.h>
#include <string>
#include <set>
namespace Data
{

    // symbols
    void* AHUD_ReceiveDrawHUD;
    void* AHUD_DrawText; // void* HUD, const FString & Text, FColor TextColor, float ScreenX, float ScreenY, void* Font, float Scale, bool bScalePosition
    void* UCanvas_K2_DrawLine;
    void* UUserWidget_AddToViewport;
    void* AHUD_GetTextSize;
    void* AGameMode_Say;
    std::unordered_map < int, BoneData > BoneCache;

    
    
    
    void* (*_Draw)(AHUD* hud, int Width, int Height);


    
    uintptr_t Module;
	float ScreenWidth;
	float ScreenHeight;
	_D3DMATRIX2 ViewWorld;
    uintptr_t Uworld = 0;
	uintptr_t GNames = 0;
	uintptr_t Matrix = 0;//ImGui::GetColorU32({ 1.f, 0.f, 0.f,1.f })
	uintptr_t Ulever = 0;
	uintptr_t Actor = 0;
	uintptr_t Count = 0;
	uintptr_t APawn = 0;
	uintptr_t RootComponent = 0;
	uintptr_t CameraManager = 0;
	uintptr_t PlayerController = 0;
	uintptr_t GEngine = 0;
	Vector3 SelfPos;
	CSGOCharacterBase_C* ACharacter;

    /**
        * 判断是否能读取
        * @param addr
        * @return
        */
    bool isCanRead(long addr) {
        static int pageSize = getpagesize();
        unsigned char vec = 0;
        unsigned long start = addr & (~(pageSize - 1));
    #if __aarch64__
        register int64_t x8 asm("x8") = __NR_mincore;
            register int64_t x0 asm("x0") = start;
            register int64_t x1 asm("x1") = pageSize;
            register int64_t x2 asm("x2") = (int64_t)&vec;
    
            asm volatile("svc 0"
            : "=r"(x0)
            : "r"(x8),"0"(x0),"r"(x1),"r"(x2)
            : "memory", "cc");
    
    #elif __arm__
    #endif
        return vec == 1;
    }
    
    std::unordered_map < int, std::string > ClassCache;
    
    std::string GetFromFName(const int index)
	{
		static char cBuf[256] = {};
		//memset(&cBuf, 0, 256);
		auto chunkOffset = index >> 16;
		auto nameOffset = index & 65535;
		auto namePoolChunk = Ram<uintptr_t>(GNames + 0x30 + 0x10 + (chunkOffset * 0x8));
		auto FNameEntry = namePoolChunk + (0x2 * nameOffset);
		
		auto FNameEntryHeader = Ram<unsigned short>(FNameEntry);
		auto StrPtr = FNameEntry + 0x2;
		auto nameLength = FNameEntryHeader >> 6;
		if (nameLength > 256 || nameLength <= 0) return "None";
		

		    
		memmove(&cBuf, (void*)StrPtr, nameLength);
		cBuf[nameLength] = 0;
		return cBuf;
	}

    
    bool WorldToScreen(const class Vector3& WorldLocation, Vector4* Rect)
	{
		Vector3 ViewW = Vector3(0.0f, 0.0f, 0.0f);
		float ViewY2 = 0.0f, ItemY = 0.0f;
		ViewW.z = ViewWorld._14 * WorldLocation.x + ViewWorld._24 * WorldLocation.y + ViewWorld._34 * WorldLocation.z + ViewWorld._44;
		if (ViewW.z <= 0.01f) { return false; }
		ViewW.z = 1 / ViewW.z;
		ViewW.x = ScreenWidth + (ViewWorld._11 * WorldLocation.x + ViewWorld._21 * WorldLocation.y + ViewWorld._31 * WorldLocation.z + ViewWorld._41) * ViewW.z * ScreenWidth;
		ViewW.y = ScreenHeight - (ViewWorld._12 * WorldLocation.x + ViewWorld._22 * WorldLocation.y + ViewWorld._32 * (WorldLocation.z - 45.0f) + ViewWorld._42) * ViewW.z * ScreenHeight;
		ViewY2 = ScreenHeight - (ViewWorld._12 * WorldLocation.x + ViewWorld._22 * WorldLocation.y + ViewWorld._32 * (WorldLocation.z + 45.0f) + ViewWorld._42) * ViewW.z * ScreenHeight;
		ItemY = ScreenHeight - (ViewWorld._12 * WorldLocation.x + ViewWorld._22 * WorldLocation.y + ViewWorld._32 * WorldLocation.z + ViewWorld._42) * ViewW.z * ScreenHeight;
		Rect->x = ViewW.x - (ViewY2 - ViewW.y) / 4;
		Rect->y = ViewW.y;
		Rect->w = (ViewY2 - ViewW.y) / 2;
		Rect->h = ViewY2 - ViewW.y;
		return true;
	}

	bool BoneToScreen(const class Vector3& WorldLocation, Vector2* Rect)
	{
		Vector3 ViewW = Vector3(0.0f, 0.0f, 0.0f);
		float ViewY2 = 0.0f, ItemY = 0.0f, Box = 0.0f;
		ViewW.z = ViewWorld._14 * WorldLocation.x + ViewWorld._24 * WorldLocation.y + ViewWorld._34 * WorldLocation.z + ViewWorld._44;
		if (ViewW.z <= 0.01f) { return false; }
		ViewW.z = 1 / ViewW.z;
		ViewW.x = ScreenWidth + (ViewWorld._11 * WorldLocation.x + ViewWorld._21 * WorldLocation.y + ViewWorld._31 * WorldLocation.z + ViewWorld._41) * ViewW.z * ScreenWidth;
		ViewW.y = ScreenHeight - (ViewWorld._12 * WorldLocation.x + ViewWorld._22 * WorldLocation.y + ViewWorld._32 * WorldLocation.z + ViewWorld._42) * ViewW.z * ScreenHeight;
		Rect->x = ViewW.x;
		Rect->y = ViewW.y;
		return true;
	}
	
	bool BoneTo3D(float Facing, const class Vector2& Orig, const class Vector3& WorldLocation, Vector2* Rect)
	{
		Vector3 ViewW = Vector3(0.0f, 0.0f, 0.0f);
		float ViewY2 = 0.0f, ItemY = 0.0f, Box = 0.0f;
		ViewW.z = ViewWorld._14 * WorldLocation.x + ViewWorld._24 * WorldLocation.y + ViewWorld._34 * WorldLocation.z + ViewWorld._44;
		if (ViewW.z <= 0.01f) { return false; }
		ViewW.z = 1 / ViewW.z;
		ViewW.x = ScreenWidth + (ViewWorld._11 * WorldLocation.x + ViewWorld._21 * WorldLocation.y + ViewWorld._31 * WorldLocation.z + ViewWorld._41) * ViewW.z * ScreenWidth;
		ViewW.y = ScreenHeight - (ViewWorld._12 * WorldLocation.x + ViewWorld._22 * WorldLocation.y + ViewWorld._32 * WorldLocation.z + ViewWorld._42) * ViewW.z * ScreenHeight;
		ViewW.x = ViewW.x + (Orig.x - ViewW.x) * cos(Facing * M_PI / 180) + (Orig.y - ViewW.y) * sin(Facing * M_PI / 180);
	    ViewW.y = ViewW.y - (Orig.x - ViewW.x) * sin(Facing * M_PI / 180) + (Orig.y - ViewW.y) * cos(Facing * M_PI / 180);

		Rect->x = ViewW.x;
		Rect->y = ViewW.y;
		
		return true;
	}

	Vector3 GetBoneFTransform(uintptr_t Mesh, const int Id)
	{
		uintptr_t BoneActor = Ram<uintptr_t>(Mesh + OFFSET_BonArray) + 0x30;
		FTransform lpFTransform = Ram<FTransform>(BoneActor + Id * 0x30);
		FTransform ComponentToWorld = Ram<FTransform>(Mesh + OFFSET_ComponentToWorld);
		D3DMATRIX2 Matrix = FTransform::MatrixMultiplication(lpFTransform.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
		return Vector3(Matrix._41, Matrix._42, Matrix._43);
	}

    bool LineOfSightTo(uintptr_t PlayerController, uintptr_t Other, FVector ViewPoint, bool bAlternateChecks) {
		auto function = reinterpret_cast<bool(*)(uintptr_t, uintptr_t, FVector, bool)>(Module + 0x8cc4998);
		return function(PlayerController, Other, ViewPoint, bAlternateChecks);
	}




    bool Get(){
        Uworld = Ram<uintptr_t>(Module + OFFSET_Uworld);
		GNames = Module + OFFSET_GNames;
		Ulever = Ram<uintptr_t>(Uworld + OFFSET_Ulever);
		Actor = Ram<uintptr_t>(Ulever + OFFSET_Actor);
		Count = Ram<int>(Ulever + OFFSET_Count);
		uintptr_t GameInstance = Ram<uintptr_t>(Uworld + OFFSET_GameInstance);
		uintptr_t ULocalPlayer = Ram<uintptr_t>(GameInstance + OFFSET_ULocalPlayer);
		ULocalPlayer = Ram<uintptr_t>(ULocalPlayer);
		PlayerController = Ram<uintptr_t>(ULocalPlayer + OFFSET_PlayerController);
		CameraManager = Ram<uintptr_t>(PlayerController + OFFSET_CameraManager);
		APawn = Ram<uintptr_t>(PlayerController + OFFSET_APawn);
		ACharacter = (CSGOCharacterBase_C*) (APawn);
		uintptr_t RootComponent = Ram<uintptr_t>(APawn + OFFSET_RootComponent);
        SelfPos = Ram<Vector3>(RootComponent + OFFSET_RelativeLocation);
		Matrix = Ram<uintptr_t>(Ram<uintptr_t>(Module + OFFSET_Matrix) + 0x20) + 0x280;
	    GEngine = Ram<uintptr_t>(Module + 0xB105478);
		return true;
    }
    
    bool isStartWith(string str, string check)
    {
    	return (str.rfind(check, 0) == 0);
    }
    
    bool isEqual(string s1, string s2)
    {
    	return (s1 == s2);
    }
    
    bool isContain(string str, string check)
    {
    	size_t found = str.find(check);
    	return (found != string::npos);
    }
    
    uintptr_t LockObj;
    
    bool DrawLine = true;
    bool DrawBox = true;
    bool DrawBone = true;
    float AimRange = 200.0f;
    bool AutoDown = false;
    bool AutoFix = false;
    bool AutoFuck = true;
    
    uintptr_t FuckObj = 0;
    Vector3 FuckPos;
    bool FuckOthers = false;
    bool FuckWeapon = false;
    
    
	void* Draw(AHUD* hud, int Width, int Height){
	    Get();
	    
		ViewWorld = Ram<_D3DMATRIX2>(Matrix);
	    
	    ScreenWidth = Width / 2;
	    ScreenHeight = Height / 2;
	    
	    if(!Actor || Count > 16384 || Count <= 0 || !hud)
	        return _Draw(hud, Width, Height);
	    
	    int WindowWidth = Ram<int>(Module + 0xAE5EDB0);
	    int WindowHeight = Ram<int>(Module + 0xAE5EDB4);
	    
	    UI::SetupUI(hud, (float)((ScreenWidth * 2) / WindowWidth) - 0.0475f, (float)((ScreenHeight * 2) / WindowHeight));
	    static FVector2D WindowPos = {200,200};
	    static bool IsOpen = true;
	    static float TempValue = 0;
	    
	    
	    if(UI::Window("小黑", &WindowPos, {500,500}, IsOpen, TempValue)){
    	    static int tab = 0;
    		if (UI::ButtonTab("绘制", FVector2D{ 110, 25 }, tab == 0)) tab = 0;
    		if (UI::ButtonTab("自瞄", FVector2D{ 110, 25 }, tab == 1)) tab = 1;
    		if (UI::ButtonTab("功能", FVector2D{ 110, 25 }, tab == 2)) tab = 2;
            UI::NextColumn(130.0f);
        	
        	if(tab == 0){
        		
        		
        
        		UI::Checkbox("绘制射线", &DrawLine);
        		UI::Checkbox("绘制方框", &DrawBox);
        		UI::Checkbox("绘制骨骼", &DrawBone);
        		
    		}else if(tab == 1){
    		    UI::SliderFloat("自瞄范围", &AimRange, 15.0f, 500.0f, "范围: %.1f");
    		    UI::Text("开启无后聚点后请勿开枪智能压枪和修正，否正命中率降低");
        		UI::Checkbox("智能压枪", &AutoDown);
        		UI::Checkbox("智能修正", &AutoFix);
        		UI::Checkbox("无后聚点", &AutoFuck);
    		}else if(tab == 2){
    		    static bool HighLight = false;
        		if(UI::Checkbox("全图高亮", &HighLight)){
        		    struct RGB{
        		        float R;
        		        float G;
        		        float B;
        		        RGB(float _R, float _G, float _B) : R(_R), G(_G), B(_B){}
        		    };
        		    if(HighLight){
        		        *(RGB*)(CameraManager + 0x260) = RGB(5.0f,5.0f,5.0f);
        		        
        		    }else{
        		        *(RGB*)(CameraManager + 0x260) = RGB(1.0f,1.0f,1.0f);
        		    }
        		}
        		
        		UI::Checkbox("Fuck Others", &FuckOthers);
        		UI::Checkbox("改枪械数据", &FuckWeapon);
        	
        		
        		if(APawn){
        		    static float GlobalSpeed;
        		    static float Gravity;
        		    if(GlobalSpeed != Ram<float>(APawn + 0x98)){
        		        GlobalSpeed = Ram<float>(APawn + 0x98);
        		    }
        		    
            		UI::SliderFloat("人物全局速度", &GlobalSpeed, 1.0f, 10.0f, "速度: %.1f");
                    if(GlobalSpeed != Ram<float>(APawn + 0x98)){
        		        Write<float>(APawn + 0x98, GlobalSpeed);
        		    }
        		    
            		char debugText[64];
            		sprintf(debugText, "APawn: %p\nPlayerController: %p", APawn, PlayerController);
            		//*(uintptr_t*) (Module + 0x8) = CameraManager;
            		UI::Text(debugText);
            		
            		
            		
            	}
        		
        		
    		}
    		
    		
    
    		
    		
    		
    		
		}
		
		if(FuckOthers){
		    if(FuckObj){
		            
                auto RootComp = Ram<uintptr_t>(APawn + OFFSET_RootComponent);
                if(RootComp)
                    *(Vector3*)(RootComp + OFFSET_RelativeLocation) = FuckPos;
            }
		}
		
		if(FuckWeapon && ACharacter){
		
		    auto weapon = ACharacter->GetCurrentWeapon();
		    if(weapon){
    		    weapon->BulletNum = 15;
		   } 
		}
    		
		

		// UI::Draw_Cursor(true);


	    auto Canvas = hud->Canvas;

	    uintptr_t TempObj = 0;
	    float TempDis = AimRange * 2;
	    
	    if(!ACharacter) return _Draw(hud, Width, Height);
	    if(ACharacter->IsFreeze){
	        ACharacter->IsFreeze = false;
	    }
	    bool IsCT = ACharacter->IsCT;
	    
	    float LHeal = 101.0f;
	    FuckObj = 0;
	    for(int i = 0; i < Count; i ++){
	        auto Obj = Ram<uintptr_t>(Actor + i * 0x8);
	        if(!Obj || APawn == Obj) continue;
	        
	        int ObjId = Ram<int>(Obj + 0x18);
            auto ClassName = GetFromFName(ObjId);

	        
            auto RootComp = Ram<uintptr_t>(Obj + OFFSET_RootComponent);
            if(!RootComp)
                continue;

            auto Pos = Ram<Vector3>(RootComp + OFFSET_RelativeLocation);
            
            
            if(strstr(ClassName.c_str(), "CSGOCharacterBase_C")){
                
                CSGOCharacterBase_C* Character = (CSGOCharacterBase_C*) (Obj);
                
                if(Character->IsDead)
                    continue; // 过滤死亡
                
                if(Character->IsCT == IsCT)
                    continue; // 过滤队友
                
                
                Vector2 Out;
                if(!BoneToScreen(Pos, &Out)){
                    continue;
                }
                
                Vector4 Out2;
                WorldToScreen(Pos, &Out2);
                
                auto Mesh = Ram<uintptr_t>(((uintptr_t)Obj)+ 0x280);
	            if(!Mesh) continue;
	            
                auto boneData = Character->GetBoneData();

                int bonelist[][2]
        		{
        
        			{boneData.Chest, boneData.Pelvis},//脖子 脊椎
        
        			{boneData.Chest, boneData.arm_upper_L},//脖子 左肩膀
        			{boneData.arm_upper_L, boneData.arm_lower_L},//左肩膀 左关节
        			{boneData.arm_lower_L, boneData.Hand_L},//左关节 左手
        	
        			{boneData.Chest, boneData.arm_upper_R},//脖子 左肩膀
        			{boneData.arm_upper_R, boneData.arm_lower_R},//左肩膀 左关节
        			{boneData.arm_lower_R, boneData.Hand_R},//左关节 左手
        			
        			{boneData.Pelvis, boneData.leg_upper_L},//脖子 左肩膀
        			{boneData.leg_upper_L, boneData.ankle_L},//左肩膀 左关节
        			
        			{boneData.Pelvis, boneData.leg_upper_R},//脖子 左肩膀
        			{boneData.leg_upper_R, boneData.ankle_R},//左肩膀 左关节
        			
        			
        			
        		};
        
        		Vector2 rect_0 = Vector2();
        		Vector2 rect_1 = Vector2();
        		
        		bool CurVisble = false;
        		Vector3 CurLocation;
                std::set<float> BoneX, BoneY, BoneZ;
        
        		for (int i = 0; i < 11; ++i)
        		{
        			auto Start = GetBoneFTransform(Mesh, bonelist[i][0]);
        			auto End = GetBoneFTransform(Mesh, bonelist[i][1]);
        			
        			BoneX.insert(Start.x);
        			BoneY.insert(Start.y);
        			BoneZ.insert(Start.z);
        			BoneX.insert(End.x);
        			BoneY.insert(End.y);
        			BoneZ.insert(End.z);
        			
        			if (BoneToScreen(Start, &rect_0)
        				&& BoneToScreen(End, &rect_1))
        			{
        			    if(LineOfSightTo(PlayerController, CameraManager, {Start.x,Start.y,Start.z}, false) &&
        			      LineOfSightTo(PlayerController, CameraManager, {End.x,End.y,End.z}, false)){
    					    if(DrawBone)
    					        Canvas->K2_DrawLine({rect_0.x, rect_0.y}, {rect_1.x, rect_1.y}, 1.0f, FLinearColor(1.0f,0,0,1.0f));
    					    if(!CurVisble){
    					        CurLocation = Start;
    					        CurVisble = true;
    					    }
    					}else{
    					    if(DrawBone)
    					        Canvas->K2_DrawLine({rect_0.x, rect_0.y}, {rect_1.x, rect_1.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
    					}
        			}
        		}
        		
        		// Draw 动态3D Box
        		
        		// 1.计算八个顶点
        		float MinX = *min_element(BoneX.begin(), BoneX.end()) - 5.0f;
        		float MinY = *min_element(BoneY.begin(), BoneY.end()) - 5.0f;
        		float MinZ = *min_element(BoneZ.begin(), BoneZ.end()) - 10.0f;

        		float MaxX = *max_element(BoneX.begin(), BoneX.end()) + 5.0f;
        		float MaxY = *max_element(BoneY.begin(), BoneY.end()) + 5.0f;
        		float MaxZ = *max_element(BoneZ.begin(), BoneZ.end()) + 15.0f;
        		auto TOP_A_POS = Vector3(MinX, MinY, MaxZ);
        		auto TOP_B_POS = Vector3(MaxX, MinY, MaxZ);
        		auto TOP_C_POS = Vector3(MinX, MaxY, MaxZ);
        		auto TOP_D_POS = Vector3(MaxX, MaxY, MaxZ);
	            
	            auto BOT_A_POS = Vector3(MinX, MinY, MinZ);
        		auto BOT_B_POS = Vector3(MaxX, MinY, MinZ);
        		auto BOT_C_POS = Vector3(MinX, MaxY, MinZ);
        		auto BOT_D_POS = Vector3(MaxX, MaxY, MinZ);
        		
        		
        		Vector2 TOP_A, TOP_B, TOP_C, TOP_D,
        		         BOT_A, BOT_B, BOT_C, BOT_D;
        		bool ShowAllPoint = true;
        		ShowAllPoint &= BoneToScreen(TOP_A_POS, &TOP_A);
        		ShowAllPoint &= BoneToScreen(TOP_B_POS, &TOP_B);
        		ShowAllPoint &= BoneToScreen(TOP_C_POS, &TOP_C);
        		ShowAllPoint &= BoneToScreen(TOP_D_POS, &TOP_D);
        		ShowAllPoint &= BoneToScreen(BOT_A_POS, &BOT_A);
        		ShowAllPoint &= BoneToScreen(BOT_B_POS, &BOT_B);
        		ShowAllPoint &= BoneToScreen(BOT_C_POS, &BOT_C);
        		ShowAllPoint &= BoneToScreen(BOT_D_POS, &BOT_D);
        
        		if(ShowAllPoint){
            		Canvas->K2_DrawLine({TOP_A.x, TOP_A.y}, {BOT_A.x, BOT_A.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_B.x, TOP_B.y}, {BOT_B.x, BOT_B.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_C.x, TOP_C.y}, {BOT_C.x, BOT_C.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_D.x, TOP_D.y}, {BOT_D.x, BOT_D.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
    
            		Canvas->K2_DrawLine({TOP_A.x, TOP_A.y}, {TOP_B.x, TOP_B.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_C.x, TOP_C.y}, {TOP_D.x, TOP_D.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_A.x, TOP_A.y}, {TOP_C.x, TOP_C.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({TOP_B.x, TOP_B.y}, {TOP_D.x, TOP_D.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
    
            		Canvas->K2_DrawLine({BOT_A.x, BOT_A.y}, {BOT_B.x, BOT_B.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({BOT_C.x, BOT_C.y}, {BOT_D.x, BOT_D.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({BOT_A.x, BOT_A.y}, {BOT_C.x, BOT_C.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            		Canvas->K2_DrawLine({BOT_B.x, BOT_B.y}, {BOT_D.x, BOT_D.y}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
            	}
        		
        		auto HeadLoc = GetBoneFTransform(Mesh, boneData.Head);
        		Vector2 Head;
        		BoneToScreen(HeadLoc, &Head);
        		
        		
        		float RealHeal = Character->CHeal / Character->CHealMax * 100.0f;
        		if(LHeal > RealHeal){
        		    LHeal = RealHeal;
        		    FuckObj = Obj;
        		    FuckPos = HeadLoc;
        		    FuckPos.z += 50;
        		}
                Canvas->K2_DrawLine({Out2.x+Out2.w/2-60.0f, Out2.y}, {Out2.x+Out2.w/2-60.0f+120*RealHeal/100.0f, Out2.y}, 5.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));
                
                char DisplayDis[64];
                
                sprintf(DisplayDis, "%.1fm", SelfPos.Distance(Pos, 100.0f));
                
	            hud->DrawText(FString(DisplayDis),Out2.x-Out2.w, Out2.y+6, FLinearColor(1.f,0,0,1.f));
	            //hud->DrawText(FString(Character->GetCharacterName()),Out2.x, Out2.y+6, FLinearColor(1.f,0,0,1.f));
	            //if(DrawBox)
	            //    Canvas->DrawRect(Out2.x, Out2.y, Out2.w, Out2.h, 1.0f, FLinearColor(1.0f,0.0f,0.0f,1.0f));
                if(DrawLine)
                    Canvas->K2_DrawLine({Head.x, Head.y}, {ScreenWidth, 100}, 1.0f, FLinearColor(1.0f,1.0f,1.0f,1.0f));

	            
	            if(CurVisble){
	                float XDis = abs(ScreenWidth - Head.x);
	                float YDis = abs(ScreenHeight - Head.y);
	                
	                if(XDis <= AimRange && YDis <= AimRange){
	                    float Dis = (XDis + YDis) / 2;
	                    if(Dis < TempDis){
	                        TempDis = Dis;
	                        TempObj = Obj;
	                    }
	                }
	                
	                if(LockObj == Obj){
                        Canvas->K2_DrawLine({Head.x, Head.y}, {ScreenWidth, ScreenHeight}, 2.0f, FLinearColor(1.0f,0,0,1.0f));
                        if(ACharacter->IsFiring && !ACharacter->IsReloading && !ACharacter->IsSwitching){
                            // Aimbot
                            auto LocalCameraLocation = *(FVector *) (CameraManager + 0x20e0 + 0x10);
                            auto Rot = getPointingAngle(LocalCameraLocation, {CurLocation.x, CurLocation.y, CurLocation.z});
                            if(AutoDown){
                                Rot.Pitch -= ACharacter->NextPitchOffset; // 压枪
                                Rot.Yaw  -= ACharacter->NextYawOffset;
                            }
                            if(AutoFix){
                                Rot.Pitch -= ACharacter->CrosshairMovePitch; // 修正弹道
                                Rot.Yaw  -= ACharacter->CrosshairMoveYaw;
                            }
                            if(AutoFuck){
                                ACharacter->RecoilCount = 0;
                            }
                            *(FRotator*) (PlayerController + 0x288) = Rot;
                          //  *(FVector*) (Ram<uintptr_t>(CameraManager + OFFSET_RootComponent) + OFFSET_RelativeLocation) = {CurLocation.x, CurLocation.y, CurLocation.z};
                            
                                                
                        }
	                }
	                
	            }
	
	            
	        }

            

	        
	    }
	    
	
	    
	    Canvas->DrawCircle(ScreenWidth,ScreenHeight, AimRange, 64, FLinearColor(1.0f,0.0f,0.0f,1.0f));
	    LockObj = TempObj;
	    return _Draw(hud, Width, Height);
	}
	
	void* (*_Say)(void* GameMode, FString Text);
	void* Say(void* GameMode, FString Text){
	    LOGD("On Say");
	}
	
	void (*_test_hook5)(void* obj, FVector& CamLoc, FRotator& CamRot);
    void test_hook5(void* obj, FVector& CamLoc, FRotator& CamRot){
        int Oid = Data::Ram<int>((uintptr_t) obj + 0x18);
        _test_hook5(obj, CamLoc, CamRot);
        
        if(FuckObj&&FuckOthers){
            
            
            CamLoc.X = FuckPos.x;
            CamLoc.Y = FuckPos.y;
            CamLoc.Z = FuckPos.z;

            
        }
        LOGD("%s %f %f %f %f %f %f", Data::GetFromFName(Oid).c_str(), CamLoc.X, CamLoc.Y, CamLoc.Z, CamRot.Pitch, CamRot.Yaw, CamRot.Roll);
    }
	
}

