#include "Server.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <limits>
#include <cstdlib>
#include <boost/json.hpp>

std::string result_to_json(const AnalysisResult& res)
{
    boost::json::object obj;
    obj["total_words"] = res.total_words;
    obj["unique_words"] = res.unique_words;
    obj["longest_sequence_length"] = res.longest_sequence_length;
    obj["longest_sequence"] = res.longest_sequence;

    return boost::json::serialize(obj);
}

// /analyze
void do_session(tcp::socket socket)
{
    try 
    {
        beast::flat_buffer buffer;
        http::request_parser<http::string_body> parser;
        parser.body_limit(std::numeric_limits<std::uint64_t>::max());

        std::cout << "Waiting for request..." << std::endl;

        http::read(socket, buffer, parser);
        auto req = parser.get();

        std::cout << "Request received: " << req.target() << " with method: " << req.method_string() << std::endl;

        http::response<http::string_body> res;
        if (req.method() == http::verb::post && req.target() == "/analyze")
        {
            std::cout << "Processing analysis for request body..." << std::endl;
            AnalysisResult ar = analyze_text(req.body());
            std::string body = result_to_json(ar);
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json; charset=utf-8");
            res.body() = body;
            res.prepare_payload();

            std::cout << "Analysis completed. Sending response..." << std::endl;
        }
        else
        {
            std::cout << "Invalid request method or target" << std::endl;
            res.result(http::status::not_found);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Not found";
            res.prepare_payload();
        }

        std::cout << "Sending response..." << std::endl;
        http::write(socket, res);
    }
    catch (std::exception& e)
    {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}


void do_accept(tcp::acceptor& acceptor, asio::io_context& ioc)
{
    acceptor.async_accept([&acceptor, &ioc](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "Accepted new connection from: " << socket.remote_endpoint() << std::endl;
                std::thread([sock = std::move(socket)]() mutable
                    {
                        do_session(std::move(sock));
                    }).detach();
            }
            else
            {
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            }

            if (!ioc.stopped())
            {
                do_accept(acceptor, ioc); // Продовжуємо приймати нові з'єднання
            }
        });
}
