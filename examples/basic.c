///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/wrui.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
    
typedef Wrui* (*Get_wrui)(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	void* handle = dlopen("t2-output/macosx-clang-debug-default/libwrui_dimgui.dylib", RTLD_LAZY);
    
    if (!handle) {
        printf("Cannot open library: %s\n", dlerror());
        return 1;
    }

    Get_wrui get_wrui = (Get_wrui) dlsym(handle, "wrui_get");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        printf("Cannot load symbol: %s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    Wrui* wrui = get_wrui();

    WUApplication* app = wrui->application_create();
    /*struct WUWindow* window =*/ wrui->window_create(0);

	return wrui->application_funcs->run(app);
}
