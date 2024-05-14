#include <array>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_context io_context;

        boost::asio::ip::tcp::socket socket(io_context);
        
        socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 13));

        while(true){
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break;  // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            
            std::cout.write(buf.data(), len);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}