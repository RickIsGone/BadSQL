#pragma once
#include <boost/asio/ip/tcp.hpp>
using boost::asio::ip::tcp;

/** @dir src
 * @brief @ref BadSQL namespace
 */
/** @file
 * @brief @ref handleQuery function declaration
 */
/** @namespace BadSQL
 * @brief The BadSQL components
 */
namespace BadSQL {
   /**
    * @brief the function that handles a query sent via tcp messages
    * @param socket the tcp socket to read the query from and write the result to
    */
   void handleQuery(tcp::socket socket);
} // namespace BadSQL
