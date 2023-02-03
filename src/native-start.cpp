#include <hook.h>

__attribute__((constructor)) void _init() {
    
    LOGI("[Xiaohei] Inject Success");
    LOGI("[Xiaohei] Dobby : %s", DobbyBuildVersion());

    pthread_t t;
    pthread_create(&t, 0, main_thread, 0);
    
    LOGI("[Xiaohei] Init End");
    
}
