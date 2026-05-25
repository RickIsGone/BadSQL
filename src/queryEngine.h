#pragma once
#include <boost/asio/ip/tcp.hpp>
using boost::asio::ip::tcp;

namespace BadSQL {
   void handleQuery(tcp::socket socket);
}
