#include "../../include/wrui.h"
#include "signal_wrappers.h"
#include "widget_private.h"
#include <QApplication>
#include <QPushButton>

#if 0

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

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int app_run(WUApplication* app) {
	QApplication* qt_app = (QApplication*)app->handle;
	return qt_app->exec();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct WUApplicationFuncs s_appFuncs = {
	app_run,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUApplication* application_create() {
	int argc = 0;
	WUApplication* app = new WUApplication;
	app->handle = (WUInternalHandle*) new QApplication(argc, 0);

	return app;
}

#if 0

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

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Wrui s_wrui = {
    WRUI_VERSION,

	// user facing

	application_create,
	0,
	0,

	// funcs

	0,
	&s_appFuncs
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" Wrui* wrui_get() {
	return &s_wrui;
}
