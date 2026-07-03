#pragma once

#include <memory>
#include <deque>
#include <vector>
#include <string>

#include <asio.hpp>

#include "mrp/include/mrp.hpp"
#include "database/database.hpp"

class Server;

class Session : public std::enable_shared_from_this<Session> {

public:

    Session(asio::io_context& ioContext);
    Session(asio::ip::tcp::socket socket);

    void startRead(Server& server);
    void sendNewMsg(const MRPHeader& newMessage);

private:

    void write();

    std::string readBuffer;
    asio::ip::tcp::socket socket;
    std::deque<std::string> writeQueue; // To prevent multiple async_write calls to be done concurrently
};

class Server {

public:

    Server(asio::io_context& ioContext, Database &database);

    void broadcastNewMsg(const MRPHeader& message);
    void acceptNewSession();

private:

    std::vector<std::shared_ptr<Session>> sessions;

    asio::io_context& ioContext;
    asio::ip::tcp::acceptor acceptor;

    Database& database;

};
