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

namespace fusenet {

  /**
   * Base class for all transport. A transport gives the owner of it
   * the ability to send data using it. Receiving is often quite
   * dependent on the transport, and is therefore something that only
   * sub classes can export interfaces for.
   */
  class Transport {

  public:

    /**
     * Send data.
     *
     * @param data the data to send
     * @param size the size of the data
     */
    virtual void send(uint8_t* data, size_t size) = 0;
  };
}

#endif

