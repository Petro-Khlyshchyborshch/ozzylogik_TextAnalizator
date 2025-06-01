#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include "Server.h"

int main() 
{
    try {
        asio::io_context ioc{ 1 };
        asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto)
            {
                ioc.stop();
                std::cout << "Stop Server" << std::endl;
            });

        tcp::acceptor acceptor(ioc, { asio::ip::make_address("0.0.0.0"), 8080 });

        std::cout << "Server listening on port 8080" << std::endl;

        do_accept(acceptor, ioc); 

        ioc.run(); 
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
