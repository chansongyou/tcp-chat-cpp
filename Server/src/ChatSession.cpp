#include "ChatSession.h"
#include "ChatRoom.h"


void ChatSession::start()
{
    room_.join(shared_from_this());
    doRead();
}

void ChatSession::deliver(const std::string& msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        doWrite();
    }
}

void ChatSession::doRead()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(read_buffer_),
        [this, self](boost::system::error_code ec, std::size_t length) 
        {
            if (!ec)
            {
                room_.deliver(std::string(read_buffer_.data(), length));
                doRead();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        }
    );
}

void ChatSession::doWrite()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front()),
        [this, self](boost::system::error_code ec, std::size_t /* length */)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    doWrite();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        }
    );
}