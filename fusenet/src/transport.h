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
   * the ability to receive and send data using it.
   */
  class Transport {

  public:

    /**
     * Send data.
     *
     * @param data the data to send
     */
    virtual void send(uint8_t data) const = 0;

    /**
     * Receive data.
     */
    virtual uint8_t receive(void) const = 0;

    /**
     * Destruct instance.
     *
     * @todo Why can't I just say that the destructor should be = 0?
     */
    virtual ~Transport(void) { };
  };
}

#endif

