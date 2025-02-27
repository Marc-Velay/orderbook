// Header file for input output functions
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;
namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
// using json = boost::json;

// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}

// Function to handle CORS preflight requests
http::response<http::string_body> handle_cors_options(http::request<http::string_body> const& req) {
    http::response<http::string_body> res{http::status::no_content, req.version()};
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type, x-csrftoken");
    res.keep_alive(req.keep_alive());
    return res;
}


// This function produces an HTTP response for the given request.
http::response<http::string_body> handle_request(http::request<http::string_body> const& req) {

    cout << req.method() << endl;
    cout << req.target() << endl;
    cout << req.body() << endl;

    // Returns a not found response
    auto const not_found =
        [&req](beast::string_view target)
        {
            http::response<http::string_body> res{http::status::not_found, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.set(http::field::access_control_allow_origin, "*"); // Add this line
            res.keep_alive(req.keep_alive());
            res.body() = "The resource '" + std::string(target) + "' was not found.";
            res.prepare_payload();
            return res;
        };

    // Returns a server error response
    auto const server_error =
        [&req](beast::string_view what)
        {
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.set(http::field::access_control_allow_origin, "*");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();
            return res;
        };

    // Handle CORS preflight request
    if (req.method() == http::verb::options) {
        return handle_cors_options(req);
    }

    // Respond to GET request with "Hello, World!"
    if (req.method() == http::verb::get && req.target() == "/api") {
        cout << "New GET request" << endl;
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/plain");
        res.set(http::field::access_control_allow_origin, "*");
        res.keep_alive(req.keep_alive());
        string text_body = "Welcome to the API! ";
        res.body() = text_body;
        res.prepare_payload();
        return res;
    }
    
    else if (req.method() == http::verb::get) {
        cout << "New GET request" << endl;
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "text/html");
        res.set(http::field::access_control_allow_origin, "*"); 
        res.keep_alive(req.keep_alive());
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        string time = std::ctime(&end_time);
        string text_body = "Hello, World! " + time;

        res.body() = text_body;
        res.prepare_payload();
        return res;
    }
    
    else if (req.method() == http::verb::post && req.target() == "/api/trade") {
        cout << "New GET request" << endl;
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        // res.set(http::field::content_type, "text/plain");
        res.set(http::field::content_type, "text/html");
        res.set(http::field::access_control_allow_origin, "*"); // Add this line
        res.keep_alive(req.keep_alive());
        auto req_body = boost::json::parse(req.body());
        cout << req_body << endl;

        res.body() = "text_body";
        res.prepare_payload();
        return res;
    }

    cout << "no solution found" << endl;

    // Default response for unsupported methods
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}

// This class handles an HTTP server connection.
class Session : public std::enable_shared_from_this<Session> {
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

    public:
        explicit Session(tcp::socket socket) : socket_(std::move(socket)) {}

        void run() {
            do_read();
        }

    private:
        void do_read() {
            auto self(shared_from_this());
            http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t) {
                if (!ec) {
                    do_write(handle_request(req_));
                }
            });
        }

        void do_write(http::response<http::string_body> res) {
            auto self(shared_from_this());
            auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
            http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t) {
                socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
        }
};

// This class accepts incoming connections and launches the sessions.
class Listener : public std::enable_shared_from_this<Listener> {
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

    public:
        Listener(net::io_context& ioc, tcp::endpoint endpoint)
            : ioc_(ioc), acceptor_(net::make_strand(ioc)) {
            beast::error_code ec;

            // Open the acceptor
            acceptor_.open(endpoint.protocol(), ec);
            if (ec) {
                std::cerr << "Open error: " << ec.message() << std::endl;
                return;
            }

            // Allow address reuse
            acceptor_.set_option(net::socket_base::reuse_address(true), ec);
            if (ec) {
                std::cerr << "Set option error: " << ec.message() << std::endl;
                return;
            }

            // Bind to the server address
            acceptor_.bind(endpoint, ec);
            if (ec) {
                std::cerr << "Bind error: " << ec.message() << std::endl;
                return;
            }

            // Start listening for connections
            acceptor_.listen(net::socket_base::max_listen_connections, ec);
            if (ec) {
                std::cerr << "Listen error: " << ec.message() << std::endl;
                return;
            }

            do_accept();
        }

    private:
        void do_accept() {
            acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->run();
                }
                do_accept();
            });
        }
};


// main() function: where the execution of
// C++ program begins
int main() {
  
    // This statement prints "Hello World"
    cout << "Hello World\n";

    try {
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 8081;

        net::io_context ioc{1};

        // std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();
        auto listener = std::make_shared<Listener>(ioc, tcp::endpoint{address, port});
        
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
