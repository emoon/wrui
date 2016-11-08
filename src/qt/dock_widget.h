#pragma once

// http://doc.qt.io/qt-5/qdockwidget.html

struct GUWidget;
struct GUDockWidget;

typedef struct GUDockWidgetFuncs {
	void GUDockWidget* (*create)(struct GUWidget* parent);
	void (*create_title)(const char* name, struct GUWidget* parent);
	bool (*is_floating)(GUDockWidget* dock_widget);
	bool (*set_floating)(GUDockWidget* dock_widget, bool floating);
	void (*set_widget)(GUDockWidget* dock_widget, struct GUWidget* dock_widget);
	struct GUWidget* (*widget)(GUDockWidget* dock_widget);
}

