#include "../../include/wrui.h"
#include "signal_wrappers.h"
#include <QApplication>
#include <QPushButton>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int connect(void* sender, const char* id, void* reciver, void* func) {
	GUObject* object = (GUObject*)sender;
	QObject* q_obj = (QObject*)object->p;

	QSlotWrapperNoArgs* wrap = new QSlotWrapperNoArgs(reciver, (SignalNoArgs)func);

	QObject::connect(q_obj, id, wrap, SLOT(method()));
	/*
		return 1;
	} else {
		printf("wrui: unable to create connection between (%p - %s) -> (%p -> %p)\n");
		return 0;
	}
	*/
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GUObjectFuncs s_objFuncs = {
	connect,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void widget_set_size(struct GUWidget* widget, int width, int height) {
	QObject* q_obj = (QObject*) widget->object->p;
	QWidget* q_widget = static_cast<QWidget*>(q_obj);
	q_widget->resize(width, height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GUWidgetFuncs s_widgetFuncs = {
	widget_set_size,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void object_setup(GUObject* object, void* data) {
	object->p = data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void widget_setup(GUWidget* widget, void* data) {
	widget->object = new GUObject;
	object_setup(widget->object, data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int app_run(void* app) {
	GUApplication* t = (GUApplication*)app;
	QApplication* qt_app = (QApplication*)t->p;
	return qt_app->exec();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GUApplicationFuncs s_appFuncs = {
	app_run,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GUApplication* application_create() {
	int argc = 0;
	GUApplication* app = new GUApplication;
	app->p = (void*) new QApplication(argc, 0);

	return app;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GUPushButton* push_button_create(const char* label, GUWidget* parent) {
    (void)parent;
	QPushButton* qt_button = new QPushButton(label, 0);
	qt_button->show();

	// TODO: Smarter allocator than just using new all the time

	GUPushButton* button = new GUPushButton;

	button->base = new GUWidget;

	widget_setup(button->base, (void*) static_cast<QObject*>(qt_button));

	return button;
}

extern GUDockWidgetFuncs g_dockWidgetFuncs;
extern GUMainWindowFuncs g_mainWindowFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Wrui s_wrui = {
	WRUI_VERSION(0, 0, 1),

	// user facing

	application_create,
	0,
	push_button_create,

	// funcs

	&s_objFuncs,
	&s_widgetFuncs,
	&g_mainWindowFuncs,
	0,
	&s_appFuncs
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" Wrui* wrui_get() {
	return &s_wrui;
}
