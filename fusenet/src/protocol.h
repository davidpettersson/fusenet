#ifndef PROTOCOL_H
#define PROTOCOL_H

/**
 * @file protocol.h
 *
 * This file contains the base protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <iostream>
#include <stdint.h>

#include "transport.h"

namespace fusenet {

  /**
   * Base class for all protocols. Protocols are defined seperately
   * for both client and server. To create a new protocol, create a
   * new class that inherits from this one, and override the data
   * receiving method, and add data sending methods.
   */
  class Protocol {

  public:

    /**
     * Creates a protocol instance and gives it a transport to use for
     * future communication.
     *
     * @param transport the transport
     */
    Protocol(const Transport* transport);

    /**
     * Called on data receival.
     *
     * @param data the data received
     * @param size the size of the data
     */
    virtual void onDataReceived(uint8_t* data, size_t size) = 0;

    /**
     * Called on lost connection.
     */
    virtual void onConnectionLost(void) = 0;

    /**
     * Destroys an instance.
     *
     * @todo Link errors occur if the destructor has no body. Is that
     *       correct? Why can't I write virtual ~Foo(void) = 0;?
     */
    virtual ~Protocol(void) { };

  protected:

    /**
     * The transport. Stored by the constructed.
     */
    const Transport* transport;
  };

}

#endif

