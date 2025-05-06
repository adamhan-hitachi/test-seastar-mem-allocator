#include <seastar/core/app-template.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/thread.hh>
#include <iostream>
#include <string> 
#include <sstream>

// Export the symbol for shared library
extern "C" void start_seastar() __attribute__((visibility("default")));

void start_seastar() {
    seastar::app_template app;
    
    const char* argv[3] = {"test-seastar", "-c2", "--default-log-level=error"};

    try {
        app.run(3, (char**)argv, [] {
            return seastar::async([] {
                // Test 
                std::string test_string = "Hello, Seastar!";
                std::ostringstream oss(std::move(test_string));
                std::cout << test_string << std::endl;
            });
        });  
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
