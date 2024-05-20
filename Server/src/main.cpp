#include <memory>
#include <iostream>

#include <boost/asio.hpp>

#include "ChatRoom.h"
#include "ChatSession.h"


using boost::asio::ip::tcp;


class ChatServer
{
public:
    ChatServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        , room_() 
    {
        doAccept();
    }

private:
    tcp::acceptor acceptor_;
    ChatRoom room_;

    void doAccept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec) {
                    std::make_shared<ChatSession>(std::move(socket), room_)->start();
                }
                doAccept();
            });
    }
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        ChatServer server(io_context, 1234);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
