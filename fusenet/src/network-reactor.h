#ifndef NETWORK_REACTOR_H
#define NETWORK_REACTOR_H

/**
 * @file network-reactor.h
 *
 * This file contains the network reactor interface. The network
 * reactor reacts to network events such as incoming connections,
 * received data and lost connections. It forwards these events to
 * protocol instances.
 *
 * This design is based upon a the Reactor pattern by
 * Schmidt. Although modified a bit, the idea is the same.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "cs-connection.h"
#include "cs-connectionclosedexception.h"
#include "cs-server.h"
#include "protocol-creator.h"


#include <iostream>
#include <map>

namespace fusenet {

  /**
   * Reacts to network events. This class reacts to different kinds of
   * network events, and creates new protocol instances when
   * necessary.
   *
   * This reactor is custom made for BSD sockets, but could be adapted
   * to any case where events are delivered synchronously and can be
   * demultiplexed with functionality similar to Unix' select() call.
   */
  class NetworkReactor {
  public:

    /**
     * Creates a new network reactor instance.
     */
    NetworkReactor(void);

    /**
     * Start servicing network events. This method returns when it is
     * time to shut down. This might be due to an error, or because of
     * user intervention. When a new connection is made, a protocol
     * instance is created.
     *
     * @param portNumber the port number to react on
     * @param protocolCreator the protocol creator
     */
    void serve(int portNumber, 
	       const ProtocolCreator* protocolCreator);
    
    /**
     * Initiates a connection. This method initiates a connection
     * toward a server and uses the given protocol instance for
     * communication.
     *
     * @param hostName the hostname to connect to
     * @param portNumber the port number on the host to connect to
     * @param protocolCreator the protocol creator
     */
    void initiate(const char* const hostName, 
		  int portNumber, 
		  const ProtocolCreator* protocolCreator);

    /**
     * Destroys a network reactor instance.
     */
    ~NetworkReactor(void);

  private:

    /**
     * Handle incoming data on a connection.
     */
    void handleIncomingData(client_server::Connection* connection);

    /**
     * Handle incoming data on a connection.
     */
    void handleLostConnection(client_server::Connection* connection);

    /**
     * Handle incoming data on a connection.
     */
    void handleNewConnection(void);

    /**
     * Stop serving.
     */
    void stopServing(void);
    
    /**
     * Internal server instance.
     */
    client_server::Server* server;

    /**
     * Internal protocol creator.
     */
    const ProtocolCreator* protocolCreator;

    /**
     * Internal connection -> (transport, protocol) mapping.
     */
    std::map<client_server::Connection*, std::pair<Transport*, Protocol*> > table;
  };
}

#endif

