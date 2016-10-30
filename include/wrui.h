#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct GUWidget;
struct GUPushButton;

#define GU_EVENT_RELEASED "2released()"

typedef struct GUObjectFuncs {
	int (*connect)(void* sender, const char* id, void* reciver, void* func);
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
} GUObject;

typedef struct GUWidget {
	GUObject* object;
} GUWidget;

typedef struct GUWindow {
	GUWidget* base;
} GUIWindow;

typedef struct GUPushButton {
	GUWidget* base;
} GUPushButton;

typedef struct GUApplication {
	void* p; // private data
} GUApplication;

typedef struct Wrui {
	uint64_t api_version;

	GUApplication* (*application_create)();
	GUIWindow* (*window_create)();
	GUPushButton* (*push_button_create)(const char* label);

	GUObjectFuncs* object_funcs;
	GUWidgetFuncs* widget_funcs;
	GUPushButtonFuncs* push_button_funcs;
	GUApplicationFuncs* application_funcs;

} Wrui;

#define WRUI_VERSION(major, minor, sub) ((((uint64_t)major) << 32) | (minor << 16) | (sub))

// Windowing

#define gu_window_create(funcs) wrui_get()->create_window()

// Generic window stuff

// #define gu_ctx_set_size(widget, x, y) wrui_get()->widget_funcs->set_size(widget->base, x, y)

#define gu_set_size(widget, x, y) wrui_get()->widget_funcs->set_size(widget->base, x, y)
#define gu_set_parent(widget, parent) widget->base->set_parent(widget->base, widget->base)
#define gu_push_button_create(label) wrui_get()->push_button_create(label)

// Connection API

#define gu_connect(widget, id, data, func) wrui_get()->object_funcs->connect(widget->base->object, id, data, (void*)func)

// Application

#define gu_application_crate() wrui_get()->application_create()
#define gu_application_run(app) wrui_get()->application_funcs->run(app)

// Should be the only exported symbol

extern Wrui* wrui_get();

#ifdef __cplusplus
}
#endif

