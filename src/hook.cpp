#include <hook.h>
#include <data.h>
#include <UI.h>
#include <android_native_app_glue.h>

void android_main(struct android_app* state){
}

int32_t (*orig_onInputEvent)(struct android_app *app, AInputEvent *inputEvent);
int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent) {
    UI::onEvent(inputEvent);
    return orig_onInputEvent(app, inputEvent);
}

uintptr_t get_module(const char* module_name)
{
    
    
    FILE *fp;
    uintptr_t addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    snprintf( filename, sizeof(filename), "/proc/self/maps");
    fp = fopen( filename, "r" );

    if ( fp != NULL )
    {
        while ( fgets( line, sizeof(line), fp ) )
        {
            if ( strstr( line, module_name ) )
            {
                pch = strtok( line, "-" );
                addr = strtoul( pch, NULL, 16 );
                if ( addr == 0x8000 )
                    addr = 0;
                break;
            }
        }
        fclose( fp ) ;
    }
    return addr;
}

void* (*_test_hook)(void** arg);
void* test_hook(void** arg){
    LOGD("Called1");
}
void* (*_test_hook2)(void** arg);
void* test_hook2(void** arg){
    LOGD("Called2");
}
void* (*_test_hook3)(void** arg);
void* test_hook3(void** arg){
    LOGD("Called3");
}
void* (*_test_hook4)(void** arg);
void* test_hook4(void** arg){
    LOGD("Called4");
}

void* (*_magic_hook)(void* obj, FVector NewLocation, FRotator NewRotator, bool bSweep, FHitResult& SweepHitResult, bool bTeleport);
void* magic_hook(void* obj, FVector NewLocation, FRotator NewRotator, bool bSweep, FHitResult& SweepHitResult, bool bTeleport){
    LOGD("CalledMagic1");
    return _magic_hook(obj, NewLocation, NewRotator, bSweep, SweepHitResult, bTeleport);
}

void* (*_magic_hook2)(void* obj, FVector NewLocation, FRotator NewRotator, bool bSweep, FHitResult* SweepHitResult, int Mode);
void* magic_hook2(void* obj, FVector NewLocation, FRotator NewRotator, bool bSweep, FHitResult* SweepHitResult, int Mode){
    int Oid = Data::Ram<int>(Ram<uintptr_t>((uintptr_t) obj+0x20) + 0x18);
    NewLocation.Z += 50;
      
    LOGD("CalledMagic2 %s", Data::GetFromFName(Oid).c_str());
    
    return _magic_hook2(obj, NewLocation, NewRotator, bSweep, SweepHitResult, Mode);
}

void* (*_magic_hook3)(void* obj, FVector NewLocation, FQuat NewRotator, bool bSweep, FHitResult* SweepHitResult, int Mode);
void* magic_hook3(void* obj, FVector NewLocation, FQuat NewRotator, bool bSweep, FHitResult* SweepHitResult, int Mode){
    LOGD("CalledMagic3");
    return _magic_hook3(obj, NewLocation, NewRotator, bSweep, SweepHitResult, Mode);
}

void* (*_magic_hook4)(void* obj, FVector NewLocation, bool bSweep, FHitResult* SweepHitResult, int Mode);
void* magic_hook4(void* obj, FVector NewLocation, bool bSweep, FHitResult* SweepHitResult, int Mode){
    int Oid = Data::Ram<int>(Ram<uintptr_t>((uintptr_t) obj+0x20) + 0x18);
      
    LOGD("CalledMagic4 %s", Data::GetFromFName(Oid).c_str());
    
    return _magic_hook4(obj, {0,0,0}, bSweep, SweepHitResult, Mode);
}

void* (*_magic_hook5)(void* obj, FVector NewLocation, FRotator NewRotator);
void* magic_hook5(void* obj, FVector NewLocation, FRotator NewRotator){
    int Oid = Data::Ram<int>(Ram<uintptr_t>((uintptr_t) obj+0x20) + 0x18);
      
    LOGD("CalledMagic5 %s", Data::GetFromFName(Oid).c_str());
    
    return _magic_hook5(obj, NewLocation, NewRotator);
}

void* (*_magic_hook6)(void* obj);
void* magic_hook6(void* obj){
    //int Oid = Data::Ram<int>(Ram<uintptr_t>((uintptr_t) obj+0x20) + 0x18);
      
    LOGD("CalledMagic6");
    
    return _magic_hook6(obj);
}


std::unordered_map < void*, int > CallCache;


void* (*orig_ProcessEvent)(void* pObject, void* pFunc, void** pParam);
void* _ProcessEvent(void* pObject, void* pFunc, void** pParam){
    if(pObject && pFunc && pParam){
        CallCache[pFunc] ++;
        
        int Oid = Data::Ram<int>((uintptr_t) pFunc + 0x18);
        
        if(strstr(Data::GetFromFName(Oid).c_str(), "SetWorld")){
            LOGD("%s", Data::GetFromFName(Oid).c_str());
        }

    }
    
    

    return orig_ProcessEvent(pObject, pFunc, pParam);
}
void *main_thread(void *) {
    
    // 等待 UE4 加载
    
    Data::Module=get_module("libUE4.so");
    while (!Data::Module) {
        Data::Module = get_module("libUE4.so");
        sleep(1);
    }
    
    
    
    
    
    LOGD("[Xiaohei] Hook Start");

    // _ZN4AHUD14ReceiveDrawHUDEii
    
    Data::AHUD_ReceiveDrawHUD = DobbySymbolResolver("libUE4.so", "_ZN4AHUD14ReceiveDrawHUDEii");
    if(Data::AHUD_ReceiveDrawHUD)
        LOGD("[Xiaohei] AHUD::ReceiveDrawHUD : %p", Data::AHUD_ReceiveDrawHUD);
    else{
        LOGE("[Xiaohei] Find AHUD::ReceiveDrawHUD failed!");
        return 0;
    }
    
    // _ZN4AHUD8DrawTextERK7FString12FLinearColorffP5UFontfb
    Data::AHUD_DrawText = DobbySymbolResolver("libUE4.so", "_ZN4AHUD8DrawTextERK7FString12FLinearColorffP5UFontfb");
    if(Data::AHUD_DrawText)
        LOGD("[Xiaohei] AHUD::DrawText : %p", Data::AHUD_DrawText);
    else{
        LOGE("[Xiaohei] Find AHUD::DrawText failed!");
        return 0;
    }
    
    // _ZN7UCanvas11K2_DrawLineE9FVector2DS0_f12FLinearColor
    
    Data::UCanvas_K2_DrawLine = DobbySymbolResolver("libUE4.so", "_ZN7UCanvas11K2_DrawLineE9FVector2DS0_f12FLinearColor");
    if(Data::UCanvas_K2_DrawLine)
        LOGD("[Xiaohei] UCanvas::K2_DrawLine : %p", Data::UCanvas_K2_DrawLine);
    else{
        LOGE("[Xiaohei] Find UCanvas::K2_DrawLine failed!");
        return 0;
    }
    
    // _ZN11UUserWidget13AddToViewportEi
    
    Data::UUserWidget_AddToViewport = DobbySymbolResolver("libUE4.so", "_ZN11UUserWidget13AddToViewportEi");
    if(Data::UUserWidget_AddToViewport)
        LOGD("[Xiaohei] UUserWidget::AddToViewport : %p", Data::UUserWidget_AddToViewport);
    else{
        LOGE("[Xiaohei] Find UUserWidget::AddToViewport failed!");
        return 0;
    }
    
    // _ZNK4AHUD11GetTextSizeERK7FStringRfS3_P5UFontf
    
    Data::AHUD_GetTextSize = DobbySymbolResolver("libUE4.so", "_ZNK4AHUD11GetTextSizeERK7FStringRfS3_P5UFontf");
    if(Data::AHUD_GetTextSize)
        LOGD("[Xiaohei] AHUD::GetTextSize : %p", Data::AHUD_GetTextSize);
    else{
        LOGE("[Xiaohei] Find AHUD::GetTextSize failed!");
        return 0;
    }
    
    Data::AGameMode_Say = DobbySymbolResolver("libUE4.so", "_ZN9AGameMode3SayERK7FString");
    if(Data::AGameMode_Say)
        LOGD("[Xiaohei] AGameMode::Say : %p", Data::AGameMode_Say);
    else{
        LOGE("[Xiaohei] Find AGameMode::Say failed!");
        return 0;
    }
    
    
    
    auto g_App = *(android_app**) (Data::Module + 0xAE6DFD0);
    
    orig_onInputEvent = decltype(orig_onInputEvent)(g_App->onInputEvent);
    g_App->onInputEvent = onInputEvent;
    
    
    // hook Draw
    DobbyHook(Data::AHUD_ReceiveDrawHUD, (void*)Data::Draw, (void**)&Data::_Draw);
    //DobbyHook(Data::AGameMode_Say, (void*)Data::Say, (void**)&Data::_Say);
    // hook Get
    /*DobbyHook((void*)(Data::Module + 0x8c72b08), (void*)magic_hook, (void**)&_magic_hook);
    DobbyHook((void*)(Data::Module + 0x8c6b9dc), (void*)magic_hook2, (void**)&_magic_hook2);
    DobbyHook((void*)(Data::Module + 0x8c6d9cc), (void*)magic_hook3, (void**)&_magic_hook3);
    DobbyHook((void*)(Data::Module + 0x8c65c58), (void*)magic_hook4, (void**)&_magic_hook4);
    DobbyHook((void*)(Data::Module + 0x8c6dbe0), (void*)magic_hook5, (void**)&_magic_hook5);
    DobbyHook((void*)(Data::Module + 0x689a3d0), (void*)magic_hook6, (void**)&_magic_hook6);
    */
    // hook Location
    DobbyHook((void*)(Data::Module + 0x911b99c), (void*)Data::test_hook5, (void**)&Data::_test_hook5);
    // hook ProcessEvent
    //DobbyHook((void*)(Data::Module + 0x6DDCC1C), (void*)_ProcessEvent, (void**)&orig_ProcessEvent);
    
    return 0;
}





