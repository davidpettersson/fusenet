/**
 * @file
 *
 * This file contains the client creator implementation.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "client-creator.h"
#include "client.h"

namespace fusenet {

  Protocol* ClientCreator::create(Transport* const transport) const {
    return new Client(transport);
  }

}

