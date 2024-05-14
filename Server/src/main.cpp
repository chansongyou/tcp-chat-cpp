#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

std::string make_daytime_string()
{
    std::time_t now = time(0);
    return ctime(&now);
}

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13));

        while(true)
        {
            boost::asio::ip::tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
