#include <cstdlib>
#include <csignal>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include "logger.h"
#include "queryEngine.h"
using boost::asio::ip::tcp;

constexpr unsigned short PORT = 5432;

int main() {
    BadSQL::Logger::setLogLevel(BadSQL::Logger::LogLevel::Info);

    try {
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), PORT));
        acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        BadSQL::Logger::info("Server listening on port {}...", PORT);
        std::thread ioThread([&](){ io.run(); });

        boost::asio::signal_set signals(io, SIGINT, SIGTERM);
        signals.async_wait([&](const boost::system::error_code&, int) {
            BadSQL::Logger::info("Shutdown signal received, stopping io_context...");
            io.stop();
        });

        while (!io.stopped()) {
            try {
                tcp::socket socket(io);
                acceptor.accept(socket);
                try {
                    auto ep = socket.remote_endpoint();
                    BadSQL::Logger::info("Client connected: {}:{}", ep.address().to_string(), ep.port());
                } catch (const std::exception& e) {
                    BadSQL::Logger::warn("Couldn't get remote endpoint: {}", e.what());
                }

                std::thread([sock = std::move(socket)]() mutable {
                    try {
                        BadSQL::handleQuery(std::move(sock));
                    } catch (const std::exception& e) {
                        BadSQL::Logger::error("Connection handler exception: {}", e.what());
                    }
                }).detach();
            } catch (const std::exception& e) {
                if (io.stopped())
                    break;
                BadSQL::Logger::error("Error accepting client connection: {}", e.what());
            }
        }
        if (ioThread.joinable()) 
            ioThread.join();
    } catch (const std::exception& e) {
        BadSQL::Logger::error("Failed to start server: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}