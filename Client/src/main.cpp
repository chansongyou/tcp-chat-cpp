#include <array>
#include <iostream>
#include <deque>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context,
           const std::string& host, const std::string& service)
    : io_context_(io_context)
    , socket_(io_context) 
    {
        connect(host, service);
    }

    void write(const std::string& message) 
    {
        boost::asio::post(io_context_,
            [this, message]() {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(message);
                if (!write_in_progress) {
                    doWrite();
                }
            });
    }

    void close() {
        boost::asio::post(io_context_, [this]() { socket_.close(); });
    }

private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    std::array<char, 1024> read_buffer_;
    std::deque<std::string> write_msgs_;

    void connect(const std::string& host, const std::string& service) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, service);
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint) {
                if (!ec) {
                    doRead();
                }
            });
    }

    void doRead() {
        socket_.async_read_some(boost::asio::buffer(read_buffer_),
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout.write(read_buffer_.data(), length);
                    std::cout << std::endl;
                    doRead();
                }
            });
    }

    void doWrite() {
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front()),
            [this](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        doWrite();
                    }
                }
            });
    }
};

int main(int argc, char *argv[])
{
    try {
        boost::asio::io_context io_context;

        std::string host = "127.0.0.1";
        std::string port = "1234";
        Client client(io_context, host, port);

        std::thread t([&io_context]() { io_context.run(); });

        std::string line;
        while (std::getline(std::cin, line)) {
            client.write(line);
        }

        client.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}