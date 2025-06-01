#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include "TextAnalizator.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

std::string result_to_json(const AnalysisResult& res);

void do_session(tcp::socket socket);

void do_accept(tcp::acceptor& acceptor, asio::io_context& ioc);

#endif // SERVER_H
