/**
 * @file network-reactor.cc
 *
 * This file contains the implementation of the network reactor class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

#include "network-reactor.h"

#define PREFIX "[NetworkReactor] "
#define BACKLOG 8

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

namespace fusenet {

  NetworkReactor::NetworkReactor(void) {
    // Do nothing
  }

  void NetworkReactor::handleIncomingData(SocketTransport* transport) {
    Protocol* protocol = table[transport];
    protocol->onDataReceived(static_cast<uint8_t>(transport->receive()));

    if (transport->isClosed()) {
      handleLostConnection(transport);
    }
  }

  void NetworkReactor::handleLostConnection(SocketTransport* transport) {
    Protocol* protocol = table[transport];
    protocol->onConnectionLost();
    table.erase(transport);
    delete protocol;
    delete transport;

    std::cout << PREFIX "Lost transport for descriptor " << transport->getDescriptor() << std::endl;
  }

  int NetworkReactor::handleNewConnection(int acceptSocket) {
    SocketTransport* transport;
    Protocol* protocol;
    int socket;

    socket = accept(acceptSocket, 0, 0);

    if (socket == -1) {
      std::cerr << PREFIX "Unable to accept new connection, aborting" << std::endl;
      return -1;
    }

    transport = new SocketTransport(socket);

    if (transport == NULL) {
      std::cerr << PREFIX "Unable to create transport, aborting" << std::endl;
      return -1;
    }

    protocol = protocolCreator->create(transport);

    if (protocol == NULL) {
      std::cerr << PREFIX "Unable to create protocol, aborting" << std::endl;
      delete transport;
      return -1;
    }

    table[transport] = protocol;
    protocol->onConnectionMade();

    std::cout << PREFIX "Connection made for descriptor " << socket << std::endl;
    return socket;
  }

  int NetworkReactor::createAcceptSocket(int portNumber) {
    int yes = 1;
    int descriptor;
    int status;
    struct ::sockaddr_in remote;

    descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if (descriptor == -1) {
      return -1;
    }

    status = setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (status == -1) {
      return -1;
    }

    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = INADDR_ANY;
    remote.sin_port = htons(portNumber);
    
    status = bind(descriptor, reinterpret_cast<struct sockaddr*>(&remote), sizeof(remote));

    if (status == -1) {
      return -1;
    }
    
    status = listen(descriptor, BACKLOG);

    if (status == -1) {
      return -1;
    }

    return descriptor;
  }

  void NetworkReactor::serve(int portNumber, 
			     const ProtocolCreator* protocolCreator) {
    bool done = false;
    int acceptSocket;
    std::map<SocketTransport*, Protocol*>::iterator i;
    int status;
    int largestSocket = -1;

    this->protocolCreator = protocolCreator;
    largestSocket = acceptSocket = createAcceptSocket(portNumber);
    
    if (acceptSocket == -1) {
      std::cerr << PREFIX "Unable to create socket, aborting" << std::endl;
      return;
    }

    while (!done) {
      fd_set read_set;
      fd_set except_set;
      
      // Clear sets
      FD_ZERO(&read_set);

      // Add accept socket
      FD_SET(acceptSocket, &read_set);

      for (i = table.begin(); i != table.end(); i++) {
	SocketTransport* t = (*i).first;
	FD_SET(t->getDescriptor(), &read_set);
      }
      
      // Block for activity
      status = select(largestSocket + 1, &read_set, NULL, NULL, NULL);
      assert(status != -1);
      
      // Check for activity on existing connections
      for (i = table.begin(); i != table.end(); i++) {
	SocketTransport* t = (*i).first;

	if (FD_ISSET(t->getDescriptor(), &read_set)) {
	  handleIncomingData(t);
	}
      }
      
      // Check for new connections
      if (FD_ISSET(acceptSocket, &read_set)) {
	int newSocket = handleNewConnection(acceptSocket);
	largestSocket = MAX(largestSocket, newSocket);
      }
    }
  }

  void NetworkReactor::initiate(const char* const hostName, int portNumber,
				const ProtocolCreator* protocolCreator) {
    struct sockaddr_in remote;
    struct hostent* host;
    int descriptor;
    int status;

    descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor == -1) {
      std::cerr << PREFIX "Unable to create socket, aborting" << std::endl;
      return;
    }

    remote.sin_family = AF_INET;
    host = gethostbyname(hostName);

    if (host == NULL) {
      std::cerr << PREFIX "Unable to resolve " << hostName << std::endl;
      return;
    }

    memcpy(reinterpret_cast<char*>(&(remote.sin_addr)),
	   reinterpret_cast<char*>(host->h_addr),
	   sizeof(&(remote.sin_addr)));
    remote.sin_port = htons(portNumber);

    status = connect(descriptor, reinterpret_cast<struct sockaddr*>(&remote), sizeof(remote));

    if (status == -1) {
      std::cerr << PREFIX "Unable to establish connection to " << hostName << std::endl;
      return;
    }

    SocketTransport transport(descriptor);
    Protocol* protocol = protocolCreator->create(&transport);

    if (protocol == NULL) {
      std::cerr << PREFIX "Unable to create protocol, aborting" << std::endl;
    } else {
      protocol->onConnectionMade();

      while (true) {
	protocol->onDataReceived(transport.receive());
      }

      protocol->onConnectionLost();
      delete protocol;
    }
  }

  void NetworkReactor::stopServing(void) {
    std::map<SocketTransport*, Protocol*>::iterator i;
    
    for (i = table.begin(); i != table.end(); i++) {
      SocketTransport* t = (*i).first;
      Protocol* p = (*i).second;

      table.erase(t);
      p->onConnectionLost();
      delete p;
      delete t;
    }
  }

  NetworkReactor::~NetworkReactor(void) {
    // Do nothing
  }
}

