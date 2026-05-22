#include <cstdlib>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include "logger.h"
#include "lexer.h"
#include "queryEngine.h"
using boost::asio::ip::tcp;

constexpr unsigned short PORT = 5432;

int main() {
    BadSQL::Logger::setLogLevel(BadSQL::Logger::LogLevel::Info);

    boost::asio::io_context ioContext;
    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), PORT));
    BadSQL::Logger::info("Server listening on port {}...", PORT);

    while (true) {
        tcp::socket socket(ioContext);
        acceptor.accept(socket);
        BadSQL::Logger::info("Client connected: {}:{}", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());

        std::thread(&BadSQL::handleQuery, std::move(socket)).detach();
    }

    return EXIT_SUCCESS;
}