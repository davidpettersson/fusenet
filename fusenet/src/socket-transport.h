#ifndef SOCKET_TRANSPORT_H
#define SOCKET_TRANSPORT_H

/**
 * @file socket-transport.h
 *
 * This file contains the socket transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "cs-connection.h"
#include "transport.h"

namespace fusenet {

  /**
   * This class implements the socket transport.
   */
  class SocketTransport : public Transport {
  public:

    /**
     * Create a new socket transport.
     */
    SocketTransport(const client_server::Connection* connection);
    
    /**
     * Send data via socket. If something should go wrong, the
     * error is silently ignored.
     *
     * @param data the data to send
     *
     * @todo Should we not have any sort of error handling?
     */
    void send(uint8_t data) const;

    /**
     * Destroy a socket transport.
     */
    ~SocketTransport(void);

  private:

    /**
     * Internal connection.
     */
    const client_server::Connection* connection;
  };
}

#endif

