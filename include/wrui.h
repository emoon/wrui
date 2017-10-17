#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct WUWidget;
struct WUPushButton;
struct WUDockWidget;

typedef void* WUInternalHandle;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUWidgetFuncs {
	void (*set_size)(WUInternalHandle widget, int width, int height);
} WUWidgetFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUPushButtonFuncs {
	void (*set_default)(struct WUPushButton* button, int state);
} WUPushButtonFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUApplicationFuncs {
	int (*run)(WUInternalHandle p);
} WUApplicationFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUWidget {
	WUInternalHandle handle;
} WUWidget;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUApplication {
	WUInternalHandle handle;
} WUApplication;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUPos {
	float x, y;
} WUPos;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WURect {
	float x, y, width, height;
} WURect;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUColor {
	float r, g, b, a;
} WUColor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUFont {
	int (*set_size)(WUFont* font, int size);
	int (*set_font_from_memory)(WUFont* font, void data, int size);
	int (*set_font_from_filename)(WUFont* font, const char* filename, int len);
} WUFont;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct WUPainter {
	WUFont* (*create_font)();
	void (*draw_text)(WUPainter* painter, WUPos pos, const char* text, int len, const WUFont* font);
	void (*draw_rect)(WUPainter* painter, WURect rect, WUColor color);

} WUPainter;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Wrui {
	uint64_t api_version;

	WUApplication* (*application_create)();
	WUWindow* (*window_create)(WUInternalHandle parent);
	WUPushButton* (*push_button_create)(const char* label, WUInternalHandle parent);

	WUObjectFuncs* object_funcs;
	WUWidgetFuncs* widget_funcs;
	WUMainWindowFuncs* main_window_funcs;
	WUPushButtonFuncs* push_button_funcs;
	WUApplicationFuncs* application_funcs;

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

