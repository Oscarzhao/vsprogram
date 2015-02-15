//
// chat_client.h
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(boost::asio::io_service& io_service,
      tcp::resolver::iterator endpoint_iterator);

  void write(const chat_message& msg);

  void close();

private:

  void handle_connect(const boost::system::error_code& error);

  void handle_read_header(const boost::system::error_code& error);

  void handle_read_body(const boost::system::error_code& error);

  void do_write(chat_message msg);

  void handle_write(const boost::system::error_code& error);

  void do_close();

private:
  boost::asio::io_service& io_service_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};
