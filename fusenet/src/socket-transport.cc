/**
 * @file socket-transport.cc
 *
 * This file contains the implementation of the socket transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "socket-transport.h"

#define PREFIX "[SocketTransport] "

namespace fusenet {
  
  SocketTransport::SocketTransport(const client_server::Connection* connection) {
    this->connection = connection;
  }

  void SocketTransport::send(uint8_t data) const {
    if (connection->isConnected()) {
      try {
#ifdef ENABLE_DEBUG
	std::cout << PREFIX "send(" << (int) data << ")" << std::endl;
#endif
	connection->write(static_cast<unsigned char>(data));
      } catch (client_server::ConnectionClosedException e) {
	// Silently ignore as in comment
      }
    }
  }

  uint8_t SocketTransport::receive(void) const {
    if (connection->isConnected()) {
      try {
	uint8_t data = connection->read();
#ifdef ENABLE_DEBUG
	std::cout << PREFIX "receive() = " << (int) data << std::endl;
#endif
	return data;
      } catch (client_server::ConnectionClosedException e) {
	// Silently ignore as in comment
      }
    }

    return 0;
  }

  SocketTransport::~SocketTransport(void) {
    // Do nothing
  }
}


