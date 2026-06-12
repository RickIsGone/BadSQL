#include <cstdlib>
#include <csignal>
#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include "logger.hpp"
#include "queryEngine.hpp"

/** @file
 * @brief Main entry point and TCP server initialization
 * Sets up an async TCP server listening on port 5432,
 * accepts client connections, and handles SQL queries.
 */

using boost::asio::ip::tcp;

/** @def PORT
 * @brief TCP port number where the BadSQL server listens
 */
constexpr unsigned short PORT = 5432;

/** @brief Main function - starts the BadSQL server
 *
 * Initializes the Boost ASIO IO context, starts an async TCP acceptor,
 * handles SIGINT/SIGTERM signals for graceful shutdown,
 * and spawns threads to handle incoming connections.
 *
 * @return EXIT_SUCCESS on successful shutdown, EXIT_FAILURE on error
 */
int main() {
   BadSQL::Logger::setLogLevel(BadSQL::Logger::LogLevel::Debug);

   try {
      boost::asio::io_context io;
      tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), PORT));
      acceptor.set_option(boost::asio::socket_base::reuse_address(true));
      BadSQL::Logger::info("Server listening on port {}...", PORT);

      boost::asio::signal_set signals(io, SIGINT, SIGTERM);
      signals.async_wait([&](const boost::system::error_code&, int) {
         BadSQL::Logger::info("Shutdown signal received, stopping server...");
         io.stop();
      });

      std::vector<std::thread> threadsHandler;

      std::function<void()> accept;
      accept = [&]() {
         auto socket = std::make_shared<tcp::socket>(io);
         acceptor.async_accept(*socket, [&, socket](const boost::system::error_code& ec) {
            if (!ec) {
               threadsHandler.emplace_back([sock = std::move(*socket)]() mutable {
                  try {
                     BadSQL::handleQuery(std::move(sock));
                  } catch (const std::exception& e) {
                     BadSQL::Logger::error("Connection handler exception: {}", e.what());
                  }
               });
            } else if (ec != boost::asio::error::operation_aborted)
               BadSQL::Logger::error("Async accept error: {}", ec.message());

            if (!io.stopped())
               accept();
         });
      };

      accept();
      io.run();

      BadSQL::Logger::info("Waiting for active connections to close...");
      for (auto& t : threadsHandler) {
         if (t.joinable())
            t.join();
      }

      BadSQL::Logger::info("Server stopped.");

   } catch (const std::exception& e) {
      BadSQL::Logger::error("Failed to start server: {}", e.what());
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}