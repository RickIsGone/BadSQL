#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <array>
#include "queryEngine.h"
#include "lexer.h"
#include "logger.h"
using boost::asio::ip::tcp;

namespace BadSQL{
    void handleQuery(tcp::socket socket) {
        std::string clientInfo = "<unknown>";
        try {
            try {
                auto ep = socket.remote_endpoint();
                clientInfo = ep.address().to_string() + ":" + std::to_string(ep.port());
            } catch (...) {}
            std::array<char, 8192> buffer{};
            size_t bytes = socket.read_some(boost::asio::buffer(buffer));
            std::string query(buffer.data(), bytes);
            std::string result = "OK";

            Lexer lexer{query};
            auto res = lexer.tokenize();
            if (!res)
                throw std::runtime_error{res.error()};

            auto tokens = std::move(res.value());

            boost::asio::write(socket, boost::asio::buffer(result));
            Logger::info("{} -- {}", clientInfo, result);

        } catch (std::exception& e) {
            Logger::error("{} -- {}", clientInfo, e.what());
            try {
                boost::asio::write(socket, boost::asio::buffer(std::string(e.what())));
            } catch (...) {}
        }
        socket.close();
    }
}