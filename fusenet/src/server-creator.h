#ifndef SERVER_CREATOR_H
#define SERVER_CREATOR_H

/**
 * @file
 *
 * This file contains the server creator interface.
 */

#include "database.h"
#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Class for creating instances of the server.
   */
  class ServerCreator : public ProtocolCreator {

  public:

    /**
     * Construct a server with a given database.
     *
     * @param database the database to give the server instance.
     */
    ServerCreator(Database* const Database);

    /**
     * Creates instances of server protocols.
     *
     * @param transport the transport to give the protocol
     */
    Protocol* create(Transport* const transport) const;

  private:
    
    /**
     * Database instance to give all new protocol instances.
     */
    Database* database;
  };
}

#endif

