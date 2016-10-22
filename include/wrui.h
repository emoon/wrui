#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define GU_EVENT_CLICKED "clicked"

typedef struct GUObject {
	// private data
	void* p;
	int (*connect)(void* sender, const char* id, void* reciver, void* func);
	//int (*connect_event)(void* recvier, const char* id, void* reciver, void* func);
} GUObject;

typedef struct GUWidget {
	GUObject* o;
	void (*set_size)(struct GUWidget* widget, int width, int height);
	void (*set_parent)(struct GUWidget* widget, int width, int height);
} GUWidget;

typedef struct GUWindow {
	GUWidget* base;
} GUIWindow;

typedef struct GUPushButton {
	GUWidget* base;
	void (*set_default)(struct GUPushButton* button, int state);
} GUPushButton;

typedef struct GUApplication {
	// private data
	void* p;
	int (*run)(void* p);
} GUApplication;

typedef struct Wrui {
	GUApplication* (*application_create)();
	GUIWindow* (*window_create)();
	GUPushButton* (*push_button_create)(const char* label);
} Wrui;

// Windowing

#define gu_create_window(funcs) funs->create_window()

// Generic window stuff

#define gu_set_size(widget, x, y) widget->base->set_size(widget->base, x, y)
#define gu_set_parent(widget, parent) widget->base->set_parent(widget->base, widget->base)
#define gu_push_button_crate(funcs, label) funcs->push_button_create(label)

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

