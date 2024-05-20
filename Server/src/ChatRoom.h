#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <deque>
#include <memory>
#include <set>

class ChatSession;

class ChatRoom 
{
public:
    void join(std::shared_ptr<ChatSession> session);
    void leave(std::shared_ptr<ChatSession> session);
    void deliver(const std::string& msg);

private:
    std::set<std::shared_ptr<ChatSession>> sessions_;
    std::deque<std::string> recent_msgs_;
    static const std::size_t max_recent_msgs = 100;
};

#endif
