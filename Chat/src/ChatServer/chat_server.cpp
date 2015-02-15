//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ChatServer/chat_server.h>

using boost::asio::ip::tcp;

void chat_room::join(chat_participant_ptr participant)
{
    participants_.insert(participant);
    std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
        boost::bind(&chat_participant::deliver, participant, _1));
}

void chat_room::leave(chat_participant_ptr participant)
{
    participants_.erase(participant);
}

void chat_room::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
        recent_msgs_.pop_front();

    std::for_each(participants_.begin(), participants_.end(),
        boost::bind(&chat_participant::deliver, _1, boost::ref(msg)));
}
//----------------------------------------------------------------------

chat_session::chat_session(boost::asio::io_service& io_service, chat_room& room)
    : socket_(io_service),
    room_(room)
{
}

void chat_session::start()
{
    room_.join(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        boost::bind(
        &chat_session::handle_read_header, shared_from_this(),
        boost::asio::placeholders::error));
}

void chat_session::deliver(const chat_message& msg)
{
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
            write_msgs_.front().length()),
            boost::bind(&chat_session::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
}

void chat_session::handle_read_header(const boost::system::error_code& error)
{
    if (!error && read_msg_.decode_header())
    {
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            boost::bind(&chat_session::handle_read_body, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
        room_.leave(shared_from_this());
    }
}

void chat_session::handle_read_body(const boost::system::error_code& error)
{
    if (!error)
    {
        room_.deliver(read_msg_);
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.data(), chat_message::header_length),
            boost::bind(&chat_session::handle_read_header, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
        room_.leave(shared_from_this());
    }
}

void chat_session::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
        write_msgs_.pop_front();
        if (!write_msgs_.empty())
        {
            boost::asio::async_write(socket_,
                boost::asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
                boost::bind(&chat_session::handle_write, shared_from_this(),
                boost::asio::placeholders::error));
        }
    }
    else
    {
        room_.leave(shared_from_this());
    }
}


chat_server::chat_server(boost::asio::io_service& io_service,
    const tcp::endpoint& endpoint)
    : io_service_(io_service),
    acceptor_(io_service, endpoint)
{
    start_accept();
}

void chat_server::start_accept()
{
    chat_session_ptr new_session(new chat_session(io_service_, room_));
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&chat_server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
}

void chat_server::handle_accept(chat_session_ptr session,
    const boost::system::error_code& error)
{
    if (!error)
    {
        session->start();
    }

    start_accept();
}