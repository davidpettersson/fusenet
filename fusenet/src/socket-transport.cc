/**
 * @file socket-transport.cc
 *
 * This file contains the implementation of the socket transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "socket-transport.h"

namespace fusenet {
  
  SocketTransport::SocketTransport(const client_server::Connection* connection) {
    this->connection = connection;
  }

  void SocketTransport::send(uint8_t data) const {
    if (connection->isConnected()) {
      try {
	connection->write(static_cast<unsigned char>(data));
      } catch (client_server::ConnectionClosedException e) {
	// Silently ignore as in comment
      }
    }
  }

  uint8_t SocketTransport::receive(void) const {
    if (connection->isConnected()) {
      try {
	return connection->read();
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


