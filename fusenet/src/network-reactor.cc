/**
 * @file network-reactor.cc
 *
 * This file contains the implementation of the network reactor class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "network-reactor.h"
#include "socket-transport.h"

#define PREFIX "[NetworkReactor] "

namespace fusenet {

  NetworkReactor::NetworkReactor(int portNumber,
				 const ProtocolCreator* protocolCreator) {
    this->protocolCreator = protocolCreator;
    this->server = new client_server::Server(portNumber);
  }

  void NetworkReactor::handleIncomingData(client_server::Connection* connection) {
    std::pair<Transport*, Protocol*> value = table[connection];
    value.second->onDataReceived(static_cast<uint8_t>(connection->read()));
  }

  void NetworkReactor::handleLostConnection(client_server::Connection* connection) {
    std::pair<Transport*, Protocol*> value = table[connection];
    value.second->onConnectionLost();
    delete value.second;
    delete value.first;
    delete connection;
  }

  void NetworkReactor::handleNewConnection(void) {
    client_server::Connection* connection;
    Transport* socketTransport;
    Protocol* protocol;

    connection = new client_server::Connection();

    if (connection == NULL) {
      std::cerr << PREFIX "Unable to create connection, aborting" << std::endl;
      return;
    }

    server->registerConnection(connection);

    socketTransport = new SocketTransport(connection);
    
    if (socketTransport == NULL) {
      std::cerr << PREFIX "Unable to create transport, aborting" << std::endl;
      delete connection;
      return;
    }

    protocol = protocolCreator->create(socketTransport);

    if (protocol == NULL) {
      std::cerr << PREFIX "Unable to create protocol, aborting" << std::endl;
      delete protocol;
      delete connection;
      return;
    }

    table[connection] = std::make_pair(socketTransport, protocol);
  }

  void NetworkReactor::serveForever(void) {
    client_server::Connection* connection;
    bool done = false;

    if (!server->isReady()) {
      std::cerr << PREFIX "Server not ready" << std::endl;
      return;
    } else {
      while (!done) {
	try {
	  connection = server->waitForActivity();
	  
	  if (connection != NULL) {
	    if (connection->isConnected()) {
	      std::cout << PREFIX "Handle incoming data" << std::endl;
	      handleIncomingData(connection);
	    } else {
	      std::cout << PREFIX "Handle lost connection" << std::endl;
	      handleLostConnection(connection);
	    }
	  } else {
	    std::cout << PREFIX "Handle new connection" << std::endl;
	    handleNewConnection();
	  }
	} catch (client_server::ConnectionClosedException e) {
	  std::cerr << PREFIX "Got ConnectionClosedException, aborting" << std::endl;
	  std::cerr << PREFIX "This is likely due to a bug in client_server::Server (see comment)" << std::endl;

	  /*
	   * The client_server::Server class only uses the read set
	   * for the select call. This is quite weird, and forces us
	   * to always catch an exception, which sort of defies the
	   * purpose of an exception. Should be fixed by correcting
	   * their code.
	   */

	  return;
	}
      }
    }
  }
 
  NetworkReactor::~NetworkReactor(void) {
    std::map<client_server::Connection*, std::pair<Transport*, Protocol*> >::iterator i;
    std::pair<Transport*, Protocol*> value;

    for (i = table.begin(); i != table.end(); i++) {
      value = (*i).second;
      value.second->onConnectionLost();
      delete value.second;
      delete value.first;
      delete (*i).first;
    }

    delete server;
  }
}

