#ifndef SERVER_CREATOR_H
#define SERVER_CREATOR_H

/**
 * @file
 *
 * This file contains the server creator interface.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "database.h"
#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  /**
   * Class for creating instances of the server protocol.
   */
  class ServerCreator : public ProtocolCreator {

  public:

    /**
     * Construct a server with a given database.
     */
    ServerCreator(Database* const Database);

    /**
     * Creates instances of server protocols.
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

