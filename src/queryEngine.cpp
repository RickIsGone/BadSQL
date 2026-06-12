#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <array>
#include <chrono>
#include "queryEngine.hpp"
#include "lexer.hpp"
#include "logger.hpp"
#include "parser.hpp"
using boost::asio::ip::tcp;

namespace BadSQL {
   /** @brief Handle an incoming SQL query from a TCP socket
    *
    * Main query processing function:
    * 1. Reads query data from socket
    * 2. Lexes the query string into tokens
    * 3. Parses tokens into an AST
    * 4. Executes the query (sends OK response)
    * 5. Logs execution time and results
    *
    * Exceptions are caught and sent back to the client.
    * Connection is closed after processing.
    *
    * @param socket The TCP socket connected to the client
    * @throw std::exception if lexing or parsing fails (sent to client)
    */
   void handleQuery(tcp::socket socket) {
      auto time = std::chrono::high_resolution_clock::now();
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
         Logger::debug("Entering Lexing phase");
         auto resLexing = lexer.tokenize();
         if (!resLexing)
            throw std::runtime_error{resLexing.error()};

         auto tokens = std::move(resLexing.value());

         Parser parser{tokens};
         Logger::debug("Entering Parsing phase");
         auto resParsing = parser.parse();
         if (!resParsing)
            throw std::runtime_error{resParsing.error()};

         auto expressions = std::move(resParsing.value());

         boost::asio::write(socket, boost::asio::buffer(result));
         Logger::info("{} -- {}", clientInfo, result);

      } catch (std::exception& e) {
         Logger::error("{} -- {}", clientInfo, e.what());
         try {
            boost::asio::write(socket, boost::asio::buffer(std::string(e.what())));
         } catch (...) {}
      }
      auto now = std::chrono::high_resolution_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - time).count();
      Logger::info("{} -- Query executed in {:.1f}ms", clientInfo, elapsed / 1000.f);
      socket.close();
   }
} // namespace BadSQL