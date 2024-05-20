#ifndef CHAT_SESSION_H
#define CHAT_SESSION_H

#include <deque>
#include <memory>

#include <boost/asio.hpp>


using boost::asio::ip::tcp;

class ChatRoom;

class ChatSession : public std::enable_shared_from_this<ChatSession>
{
public:
    ChatSession(tcp::socket socket, ChatRoom& room)
        : socket_(std::move(socket)), room_(room) {}

    void start();
    void deliver(const std::string& msg);

private:
    tcp::socket socket_;
    ChatRoom& room_;
    std::array<char, 1024> read_buffer_;
    std::deque<std::string> write_msgs_;

    void doRead();
    void doWrite();
};

#endif