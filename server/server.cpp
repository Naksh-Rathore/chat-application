#include "server/server.hpp"

#include <asio/ip/address_v4.hpp>
#include <deque>
#include <memory>
#include <string>

#include <asio.hpp>

#include "mrp/include/mrp.hpp"

Session::Session(asio::io_context& ioContext)
    : socket(ioContext)
    , writeQueue()
{}

Session::Session(asio::ip::tcp::socket socket)
    : socket(std::move(socket))
    , writeQueue()
{}

void Session::startRead(Server& server) {
    std::shared_ptr<Session> self = shared_from_this(); 

    asio::async_read_until(socket, asio::dynamic_buffer(readBuffer), '\n', 
        [self, &server](const asio::error_code& errorCode, std::size_t bytesTransferred) {
            if (errorCode)
                return;

                
            std::string line = self->readBuffer.substr(0, bytesTransferred);
            self->readBuffer.erase(0, bytesTransferred);

            server.broadcastNewMsg(MRPHeader(line));

            self->startRead(server);
        }
    );
}

void Session::sendNewMsg(const MRPHeader& message) {
    bool isWriting = !writeQueue.empty();

    writeQueue.push_back(message.getFormattedHeader());

    if (!isWriting)
        write();
}

void Session::write() {
    std::shared_ptr<Session> self = shared_from_this(); 

    asio::async_write(
            socket, asio::buffer(writeQueue.front()), 
            [self](const asio::error_code& errorCode, std::size_t /**/) {

                if (errorCode)
                    return;  

                self->writeQueue.pop_front();

                if (!self->writeQueue.empty())
                    self->write(); // Continue writing down the queue
            }
    );
}

Server::Server(asio::io_context& ioContext, Database& database)
    : sessions()
    , ioContext(ioContext)
    , acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 1234))
    , database(database)
{
    database.createMRPTable();
    acceptNewSession();
}

void Server::acceptNewSession() {
    acceptor.async_accept(
        [this](const asio::error_code& ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                std::shared_ptr<Session> session = std::make_shared<Session>(std::move(socket));
                session->startRead(*this);
                this->sessions.push_back(session);
            }

            this->acceptNewSession();
        }
    );
}

void Server::broadcastNewMsg(const MRPHeader& message) {
    for (const std::shared_ptr<Session>& session : sessions)
        session->sendNewMsg(message);

    database.createMRPHeader(message);
}
