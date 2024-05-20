#include "ChatRoom.h"
#include "ChatSession.h"

void ChatRoom::join(std::shared_ptr<ChatSession> session) 
{
    sessions_.insert(session);
    for (auto msg: recent_msgs_)
        session->deliver(msg);
}

void ChatRoom::leave(std::shared_ptr<ChatSession> session)
{
    sessions_.erase(session);
}

void ChatRoom::deliver(const std::string& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    for (auto& session: sessions_)
        session->deliver(msg);
}