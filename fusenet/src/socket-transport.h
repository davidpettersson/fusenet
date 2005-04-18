#ifndef SOCKET_TRANSPORT_H
#define SOCKET_TRANSPORT_H

/**
 * @file
 *
 * This file contains the socket transport interface.
 */

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
    SocketTransport(int descriptor);

    /**
     * Create a new named socket transport.
     */
    SocketTransport(int descriptor, std::string& name);
    
    /**
     * Send data via socket. If something should go wrong, the
     * error is silently ignored.
     *
     * @param data the data to send
     *
     * @todo Should we not have any sort of error handling?
     */
    void send(uint8_t data);

    /**
     * Receive data via socket. If something should go wrong, the eror
     * is silently ignored.
     *
     * @return the data received
     *
     * @todo Should we not have any sort of error handling?
     */
    uint8_t receive(void);

    /**
     * Get the socket descriptor.
     */

    int getDescriptor(void);

    /**
     * Close socket.
     */
    void close(void);

    /**
     * Is socket closed.
     */
    bool isClosed(void) const;

    /**
     * Destroy a socket transport.
     */
    virtual ~SocketTransport(void);

  private:

    /**
     * Internal connection.
     */
    int descriptor;
  };
}

#endif

