
#include "../../include/wrui.h"

typedef struct GUMainWindowFuncs {
	void (*add_dock_widget)(GUMainWindow* win, GUDockingArea area, struct GUWidget* widget);
} GUMainWindowFuncs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void add_dock_widget(GUMainWindow* win, GUDockingArea area, struct GUDockWidget* w) {
	QMainWindow* main_win = (QMainWindow*)win->widget->p;
	QDockWidget* widget = (QWidget*)w->p;

	switch (area) {
		case GUDockingArea_Left : main_win->addDockWidget(Qt::LeftDockWidgetArea, widget); break;
		case GUDockingArea_Right  : main_win->addDockWidget(Qt::RightDockWidgetArea, widget); break;
		case GUDockingArea_Top : main_win->addDockWidget(Qt::TopDockWidgetArea, widget); break;
		case GUDockingArea_Bottom : main_win->addDockWidget(Qt::BottomDockWidgetArea, widget); break;
		case GUDockingArea_None : main_win->addDockWidget(Qt::NoDockWidgetArea, widget); break;
		default : {
			printf("add_dock_widget: Incorrect docking type %d\n", (int)area);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GUMainWindowFuncs g_mainWindowFuncs = {
	add_dock_widget,
};



