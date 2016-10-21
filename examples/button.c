#include "../include/wrui.h"

int main() {
	Wrui* ui = wrui_get();

	GUApplication* app = gu_application_crate(ui);
	gu_application_run(app);

    return 0;
}
