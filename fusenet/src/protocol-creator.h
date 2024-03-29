#ifndef PROTOCOL_CREATOR_H
#define PROTOCOL_CREATOR_H

/**
 * @file
 *
 * This file contains the protocol creator interface.
 */

#include <iostream>

#include "fusenet-types.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Protocol creator interface. This interface is used to avoid
   * having the network reactor know anything about how to create
   * protocols other than that they require a transport.
   */
  class ProtocolCreator {

  public:

    /**
     * Creates instances of protocols.
     *
     * @param transport the transport to give the protocol
     */
    virtual Protocol* create(Transport* const transport) const = 0;
  };
}

#endif

