#include "../include/wrui.h"
#include <stdio.h>

typedef struct MyApp {
	int dummy;
} MyApp;

void on_clicked(void* data);

void on_clicked(void* data) {
	MyApp* d = (MyApp*)data;
	printf("0x%p\n status: %d\n", data, d->dummy);
}

int main() {
	MyApp test = { 0 };

	test.dummy = 0x12;

	Wrui* ui = wrui_get();

	GUApplication* app = gu_application_crate(ui);

	GUPushButton* button = gu_push_button_crate(ui, "Foo");

	gu_set_size(button, 100, 100);

	gu_connect(button, GU_EVENT_RELEASED, &test, on_clicked);

	gu_application_run(app);

    return 0;
}
