#include <iostream>
#include <dlfcn.h>

int main(int, char**) {
    std::cout << "Hello, from application!\n";

    // Path to the shared library
    const char* library_path = "./build/libmy-library.so";

    // Open the shared library
    void* handle = nullptr;
    try {
        handle = dlopen(library_path, RTLD_LAZY | RTLD_LOCAL | RTLD_DEEPBIND);

    } catch (const std::exception& e) {
        std::cerr << "Error loading library: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error loading library." << std::endl;
        return 1;
    }

    if (!handle) {
        std::cerr << "Failed to load library: " << dlerror() << std::endl;
        return 1;
    }

    // Clear any existing errors
    dlerror();

    // Load the start_seastar function
    using start_seastar_t = void (*)();
    start_seastar_t start_seastar = (start_seastar_t)dlsym(handle, "start_seastar");

    const char* error = dlerror();
    if (error) {
        std::cerr << "Failed to load symbol 'start_seastar': " << error << std::endl;
        dlclose(handle);
        return 1;
    }

    // Call the start_seastar function
    std::cout << "Starting Seastar...\n";
    start_seastar();

    // Close the shared library
    dlclose(handle);

    return 0;
}
