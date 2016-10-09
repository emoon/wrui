# wrui

Wrui is a wrapper for UI libraries (like Qt, wxWidgets, dear imgui) with a common C API. Motivations for these are:

* Using C++ APIs from other languages (such as Rust, Haskell, D, etc) is really annoying because of no standard ABI. Using a C API this can bring populare C++ libraries to these lanugages.
* Some people still prefer to code in C because of it's simplicity.
* Wrui will only expose one single symbol which is a table of function pointers. There are several upsides to this: 
	* Shared libraries doesn't need to hard-link with a Wrui shared library. It only needs to be sent one single pointer.
	* Possible for a main application to update with bug fixes without affecting plugins.
	* More clear separation
