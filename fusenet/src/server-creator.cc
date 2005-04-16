/**
 * @file
 *
 * This file contains the server creator implementation.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "server-creator.h"
#include "server.h"

namespace fusenet {
  
  ServerCreator::ServerCreator(Database* database) {
    this->database = database;
  }

  Protocol* ServerCreator::create(Transport* const transport) const {
    return new Server(transport, database);
  }

}

