#include <QDockWidget>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct GUDockWidget* create(struct GUWidget* parent) {
	QDockingWidget qdock = nullptr;

	if (parent) 
		qdock = QTDockWidget((QWidget*)parent->base->p);
	else
		qdock = QTDockWidget(nullptr);
		
	GUDockWidget* t = new GUDockWidget;
	t->priv = 0;

	widget_setup(t->base, (void*) qdock); 

	return t; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct GUDockWidget* create_title(const char* name, struct GUWidget* parent) {
	(void)name;
	printf("dock_widget: create_title not fully implemented yet\n");
	return create(parent);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool is_floating(GUDockWidget* w) {
	return ((QDockWidget*)w->base->p)->isFloating();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void set_floating(GUDockWidget* w, bool state) {
	((QDockWidget*)w->base->p)->setFloating(state);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void set_widget(GUDockWidget* dock, GUWidget* widget)
	dock->priv = (void*)widget;
	((QDockWidget*)w->base->p)->setWidget((QWidget*)widget->base->p);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct GUWidget* widget(GUDockWidget* w) {
	return (GUWidget*)w->priv;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GUDockWidgetFuncs s_dockWidgetFuncs = {
	create,
	create_title,
	is_floating,
	set_floating,
	set_widget,
	widget,
};
