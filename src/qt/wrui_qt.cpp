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

static int connect(void* sender, const char* id, void* reciver, void* func) {
	GUObject* object = (GUObject*)sender;
	QObject* q_obj = (QObject*)object->p;
	printf("reciver %p - func %p\n", reciver, func);
	QSlotWrapperNoArgs* wrap = new QSlotWrapperNoArgs(reciver, (SignalNoArgs)func);
	QObject::connect(q_obj, "2released()", wrap, SLOT(method()));
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GUPushButton* push_button_create(const char* label) {
	QPushButton* qt_button = new QPushButton(label, 0);

	// TODO: Smarter allocator than just using new all the time

	GUPushButton* button = new GUPushButton; 
	
	qt_button->show();

	button->base = new GUWidget;
	button->base->o = new GUObject;

	button->base->o->p = (void*) static_cast<QObject*>(qt_button); 

	printf("qobj %p\n", button->base->o->p);

	button->base->o->connect = connect;

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
