#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <array>
#include "queryEngine.h"
#include "lexer.h"
#include "logger.h"
using boost::asio::ip::tcp;

namespace BadSQL{
    void handleQuery(tcp::socket socket) {
        try {
            std::array<char, 8192> buffer{};
            size_t bytes = socket.read_some(boost::asio::buffer(buffer));
            std::string query(buffer.data(), bytes);
            std::string result = "OK\n";

            Lexer lexer{query};
            auto res = lexer.tokenize();
            if (!res)
                throw std::runtime_error{res.error()};

            auto tokens = std::move(res.value());

            boost::asio::write(socket, boost::asio::buffer(result));
        } catch (std::exception& e) {
            boost::asio::write(socket, boost::asio::buffer(std::string(e.what())));
        }
    }
}