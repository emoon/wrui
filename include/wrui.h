#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define GU_EVENT_RELEASED "2released()"

typedef struct GUObjectFuncs {
	int (*connect)(struct GUWidget* widget, const char* id, void* reciver, void* func);
} GUObjectFuncs;

typedef struct GUWidgetFuncs {
	void (*set_size)(struct GUWidget* widget, int width, int height);
} GUWidgetFuncs;

typedef struct GUPushButtonFuncs {
	void (*set_default)(struct GUPushButton* button, int state);
} GUPushButtonFuncs;

typedef struct GUApplicationFuncs {
	int (*run)(void* p);
} GUApplicationFuncs;

typedef struct GUObject {
	void* p;
	GUObjectFuncs* funcs;
} GUObject;

typedef struct GUWidget {
	GUObject object;
	GUWidgetFuncs* funcs;
} GUWidget;

typedef struct GUWindow {
	GUWidget base;
} GUIWindow;

typedef struct GUPushButton {
	GUWidget base;
	GUPushButtonFuncs* funcs;
} GUPushButton;

typedef struct GUApplication {
	void* p; // private data
	GUApplicationFuncs* funcs;
} GUApplication;

typedef struct Wrui {
	GUApplication* (*application_create)();
	GUIWindow* (*window_create)();
	GUPushButton* (*push_button_create)(const char* label);
} Wrui;

// Windowing

#define gu_window_create(funcs) funcs->create_window()

// Generic window stuff

#define gu_set_size(widget, x, y) widget->base.funcs->set_size(widget->base, x, y)
#define gu_set_parent(widget, parent) widget->base->set_parent(widget->base, widget->base)
#define gu_push_button_create(funcs, label) funcs->push_button_create(label)

// Connection API

#define gu_connect(widget, id, data, func) widget->base->o->connect(widget->base->o, id, data, (void*)func)

// Application

#define gu_application_crate(api) api->application_create()
#define gu_application_run(app) app->run(app->p)

// Should be the only exported symbol

extern Wrui* wrui_get();

#ifdef __cplusplus
}
#endif

