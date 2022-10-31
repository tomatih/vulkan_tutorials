#include "first_app.hpp"
#include <exception>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char const *argv[])
{
    // initialize the app
    lve::FirstApp app{};

    // try running it and chatch and print errors
    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}