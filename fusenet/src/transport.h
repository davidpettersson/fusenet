#ifndef TRANSPORT_H
#define TRANSPORT_H

/**
 * @file transport.h
 *
 * This file contains the base transport class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>
#include <stdint.h>
#include <string>

#define TRANSPORT_PREFIX(tp) PREFIX << "[" << (tp)->getName() << "] "

namespace fusenet {

  /**
   * Base class for all transport. A transport gives the owner of it
   * the ability to receive and send data using it.
   */
  class Transport {

  public:

    /**
     * Create transport.
     */
    Transport(void);

    /**
     * Create transport with name.
     */
    Transport(std::string& name);

    /**
     * Send data.
     *
     * @param data the data to send
     */
    virtual void send(uint8_t data) = 0;

    /**
     * Receive data.
     */
    virtual uint8_t receive(void) = 0;

    /**
     * Close transport.
     */
    virtual void close(void) = 0;

    /**
     * Is transport closed.
     */
    virtual bool isClosed(void) const = 0;

    /**
     * Transport name.
     */
    std::string& getName(void);

    /**
     * Destruct instance.
     *
     * @todo Why can't I just say that the destructor should be = 0?
     */
    virtual ~Transport(void);
    
  private:

    /**
     * Internal name.
     */
    std::string transportName;
  };
}

#endif

