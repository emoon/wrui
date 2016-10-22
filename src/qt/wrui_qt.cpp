#include "../../include/wrui.h"
#include "signal_wrappers.h"
#include <QApplication>
#include <QPushButton>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int app_run(void* app) {
	GUApplication* t = (GUApplication*)app;
	QApplication* qt_app = (QApplication*)t->p;
	return qt_app->exec();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GUApplication* application_create() {
	int argc = 0;
	GUApplication* app = new GUApplication;
	app->p = (void*) new QApplication(argc, 0);
	app->run = app_run;
	return app;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GUPushButton* push_button_create(const char* label) {
	QPushButton* qt_button = new QPushButton(label, 0);

	// TODO: Smarter allocator than just using new all the time

	GUPushButton* button = new GUPushButton; 
	
	qt_button->show();

	button->base = new GUWidget;
	button->base->o = new GUObject;

	button->base->o->p = (void*) qt_button; 

	return button;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Wrui s_wrui = {
	application_create,
	0,
	push_button_create,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" Wrui* wrui_get() {
	return &s_wrui;
}
