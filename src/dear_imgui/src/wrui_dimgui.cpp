// Implements the wrapping for imgui

#include "../../../include/wrui.h"
#include <vector>
// GLFW
#include <GLFW/glfw3.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include <imgui.h>
#include "imgui_impl_glfw.h"

struct WUWindow {
	GLFWwindow* glfw_window = nullptr;
	PaintEvent paint_event = nullptr;
	void* paint_user_data;
};

extern "C" WRUI_EXPORT Wrui* wrui_get();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Application {
	std::vector<WUWindow*> windows;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Not very nice to have this global as it isn't thread-safe

static Application* s_application = nullptr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void error_callback(int error, const char* description) {
    printf("Error %d: %s\n", error, description);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUApplication* application_create() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return 0;
    }

	Application* app = new Application;
	WUApplication* t = new WUApplication;

	s_application = app;

	t->handle = (WUInternalHandle)app;
	return t;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUFont* create_font() {
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_text(struct WUPainter* painter, WUPos pos, WUColor color, const char* text, int len, const WUFont* font) {
	(void)font;

	ImVec2 p = { pos.x, pos.y };
	ImVec4 col = { color.r, color.g, color.b, color.a };

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddText(p, ImGui::GetColorU32(col), text, text + len);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void draw_rect(struct WUPainter* painter, WURect rect, WUColor color) {
	ImVec2 a = { rect.x, rect.y };
	ImVec2 b = { rect.width, rect.height };
	ImVec4 col = { color.r, color.g, color.b, color.a };

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(a, b, ImGui::GetColorU32(col));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUPainter s_painter {
	create_font,
	draw_text,
	draw_rect,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUPainter* painter_get(void) {
	return &s_painter; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUWindow* window_create(WUInternalHandle parent) {
    GLFWwindow* glfw_window = glfwCreateWindow(1280, 720, "wrui window", NULL, NULL);
    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui binding
    ImGui_ImplGlfwGL2_Init(glfw_window, true);

    WUWindow* window = new WUWindow;
    window->glfw_window = glfw_window;

	s_application->windows.push_back(window);

	return window;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
static void window_set_paint_event(struct WUWindow* window, void* user_data, PaintEvent event) {
	window->paint_event = event;
	window->paint_user_data = user_data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUWindowFuncs window_funcs = {
	window_set_paint_event,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int app_run(WUApplication* app) {
	// Only supporting 1 window right now
	if (s_application->windows.size() != 1) {
		printf("Only 1 window is exactly supported for now\n");
		return 0;
	}

	WUWindow* win = s_application->windows[0];
    
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    
    while (!glfwWindowShouldClose(win->glfw_window))
    {
		glfwPollEvents();
		ImGui_ImplGlfwGL2_NewFrame();

		if (win->paint_event) {
			win->paint_event(win->paint_user_data); 
		}

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(win->glfw_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        glfwSwapBuffers(win->glfw_window);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static WUApplicationFuncs app_funcs = {
	app_run,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Wrui s_wrui = {
	WRUI_VERSION,

	application_create,
	window_create,
	painter_get,
	

	&window_funcs,
	&app_funcs,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" WRUI_EXPORT Wrui* wrui_get() {
	return &s_wrui;
}


