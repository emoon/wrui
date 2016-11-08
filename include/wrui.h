#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "dock_widget.h"

struct GUWidget;
struct GUPushButton;
struct GUMainWindow;
struct GUDockWidget;

#define GU_EVENT_RELEASED "2released()"

#define GU_INTERNAL_WIDGET_TYPE(type) \
typedef struct type { \
	GUWidget* base; \
} type

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum GUDockingArea {
    GUDockingArea_Left = 0x1,
    GUDockingArea_Right = 0x2,
    GUDockingArea_Top = 0x4,
    GUDockingArea_Bottom = 0x8,
    GUDockingArea_All = 0xf,
    GUDockingArea_None = 0,
} GUDockingArea;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUObjectFuncs {
	int (*connect)(void* sender, const char* id, void* reciver, void* func);
} GUObjectFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUWidgetFuncs {
	void (*set_size)(struct GUWidget* widget, int width, int height);
} GUWidgetFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUMainWindowFuncs {
	void (*add_dock_widget)(struct GUMainWindow* win, GUDockingArea area, struct GUDockWidget* widget);
} GUMainWindowFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUPushButtonFuncs {
	void (*set_default)(struct GUPushButton* button, int state);
} GUPushButtonFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUApplicationFuncs {
	int (*run)(void* p);
} GUApplicationFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUObject {
	void* p;
} GUObject;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUWidget {
	GUObject* object;
} GUWidget;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUDockWidget {
	GUWidget* base;
	void* priv;
} GUDockWidget;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GU_INTERNAL_WIDGET_TYPE(GUWindow);
GU_INTERNAL_WIDGET_TYPE(GUPushButton);
GU_INTERNAL_WIDGET_TYPE(GUMainWindow);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct GUApplication {
	void* p; // private data
} GUApplication;

typedef struct Wrui {
	uint64_t api_version;

	GUApplication* (*application_create)();
	GUWindow* (*window_create)(GUWidget* parent);
	GUPushButton* (*push_button_create)(const char* label, GUWidget* parent);

	GUObjectFuncs* object_funcs;
	GUWidgetFuncs* widget_funcs;
	GUMainWindowFuncs* main_window_funcs;
	GUPushButtonFuncs* push_button_funcs;
	GUApplicationFuncs* application_funcs;

} Wrui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WRUI_VERSION(major, minor, sub) ((((uint64_t)major) << 32) | (minor << 16) | (sub))

// Windowing

#define gu_ctx_window_create(ctx) ctx->create_window()
#define gu_window_create() gu_ctx_window_create(wrui_get())

// Generic window stuff

// #define gu_ctx_set_size(widget, x, y) wrui_get()->widget_funcs->set_size(widget->base, x, y)

#define gu_set_size(widget, x, y) wrui_get()->widget_funcs->set_size(widget->base, x, y)
#define gu_set_parent(widget, parent) widget->base->set_parent(widget->base, widget->base)
#define gu_push_button_create(label, parent) wrui_get()->push_button_create(label, parent)

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

