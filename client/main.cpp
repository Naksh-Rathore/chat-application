#include <iostream>
#include <string>
#include <thread>

#include <asio.hpp>

#include "mrp/include/mrp.hpp"

int main()
{
    try
    {
        asio::io_context io;

        asio::ip::tcp::socket socket(io);

        socket.connect({
            asio::ip::make_address("127.0.0.1"),
            1234
        });

        std::thread receiver([&]
        {
            std::string buffer;

            try
            {
                while (true)
                {
                    asio::read_until(
                        socket,
                        asio::dynamic_buffer(buffer),
                        '\n');

                    std::size_t bytes = buffer.find('\n') + 1;

                    std::string line = buffer.substr(0, bytes);
                    buffer.erase(0, bytes);

                    MRPHeader msg(line);

                    std::cout
                        << "[" << msg.utcDate << "] "
                        << msg.clientName << ": "
                        << msg.messageData
                        << '\n';
                }
            }
            catch (const std::exception&)
            {
                std::cout << "Disconnected from server.\n";
            }
        });

        std::string username;

        std::cout << "Username: ";
        std::getline(std::cin, username);

        std::string message;

        while (std::getline(std::cin, message))
        {
            MRPHeader packet(
                username,
                getCurrentUTCDate(),
                message
            );

            std::string data = packet.getFormattedHeader();

            asio::write(socket, asio::buffer(data));
        }

        receiver.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
