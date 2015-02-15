/**
 *	服务器入口
 *  @author by zhaoshuailong
 *  @date 2015-01-14
 */

#include <ChatServer/chat_message.hpp>
#include <ChatServer/chat_server.h>

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        chat_server_list servers;
        for (int i = 1; i < argc; ++i)
        {
            using namespace std; // For atoi.
            tcp::endpoint endpoint(tcp::v4(), atoi(argv[i]));
            chat_server_ptr server(new chat_server(io_service, endpoint));
            servers.push_back(server);
        }

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}