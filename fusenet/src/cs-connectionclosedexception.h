// ------------------------------------------------------------------
//
//                 Client/Server communication package
//
//                ConnectionClosedException header file
//
// Change log
//  020102  PH  Initial version
//  050113  PH  added deregisterConnection, new registration (vector),
//              added check for server shutdown, many more changes
//
// ------------------------------------------------------------------

#ifndef CONNECTION_CLOSED_EXCEPTION_H
#define CONNECTION_CLOSED_EXCEPTION_H

#include "cs-connection.h"

namespace client_server {
  class Connection;

  /* ConnectionClosedException is thrown when a connection is closed */
  class ConnectionClosedException {

  public:
    Connection *connection;
    ConnectionClosedException() { connection = 0; }
    ConnectionClosedException(Connection* c) { connection = c; }
  };
}

#endif
