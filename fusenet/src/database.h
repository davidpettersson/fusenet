#ifndef DATABASE_H
#define DATABASE_H

/**
 * @file database.h
 *
 * This file contains the base database class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "fusenet-types.h"

namespace fusenet {

  /**
   * Base class for all databases. This base class is supposed to be
   * inherited from by all different kinds of databases, in this
   * project a memory based and a disk based version.
   */
  class Database {

  public:

    /**
     * Create instance.
     */
    Database(void);

    /**
     * Destroy instance.
     */
    virtual ~Database(void);
  };
}

#endif

