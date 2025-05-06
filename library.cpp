#include <seastar/core/app-template.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/thread.hh>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

// Export the symbol for shared library
extern "C" void start_seastar() __attribute__((visibility("default")));

// Define the Payload struct
struct Payload {
    int a;

    // Serialization method
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & a;
    }
};

// Function to serialize Payload to a binary string
std::string serialize_payload(const Payload& payload) {
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << payload; // Serialize the payload
    return oss.str();
}

// Function to deserialize Payload from a binary string
Payload deserialize_payload(const std::string& data) {
    std::istringstream iss(data);
    boost::archive::binary_iarchive ia(iss);
    Payload payload;
    ia >> payload; // Deserialize the payload
    return payload;
}

void start_seastar() {
    seastar::app_template app;

    const char* argv[3] = {"test-seastar", "-c2", "--default-log-level=error"};

    try {
        app.run(3, (char**)argv, [] {
            return seastar::async([] {
                // Test: Serialize and Deserialize Payload
                Payload payload{42}; // Create a Payload object
                std::string serialized_data = serialize_payload(payload); // Serialize
                std::cout << "Serialized Payload: " << serialized_data << std::endl;

                Payload deserialized_payload = deserialize_payload(serialized_data); // Deserialize
                std::cout << "Deserialized Payload: " << deserialized_payload.a << std::endl;
            });
        });
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
