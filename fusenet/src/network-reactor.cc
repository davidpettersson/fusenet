/**
 * @file network-reactor.cc
 *
 * This file contains the implementation of the network reactor class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cassert>
#include <sstream>
#include <csignal>

#include "network-reactor.h"

#define BACKLOG 8
#define PREFIX "[NetworkReactor] "

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

namespace fusenet {

  NetworkReactor::NetworkReactor(void) {
    // Disable broken pipe signal
    signal(SIGPIPE, SIG_IGN);
  }

  void NetworkReactor::handleIncomingData(SocketTransport* transport) {
    Protocol* protocol = table[transport];
    uint8_t data = static_cast<uint8_t>(transport->receive());

    if (!transport->isClosed()) {
      std::cout << TRANSPORT_PREFIX(transport) << "Receiving data" << std::endl;
      protocol->onDataReceived(data);
    }

    if (transport->isClosed()) {
      handleLostConnection(transport);
    }
  }

  void NetworkReactor::handleLostConnection(SocketTransport* transport) {
    Protocol* protocol = table[transport];
    protocol->onConnectionLost();
    table.erase(transport);

    std::cout << TRANSPORT_PREFIX(transport) << "Lost connection" << std::endl;

    delete protocol;
    delete transport;
  }

  int NetworkReactor::handleNewConnection(int acceptDescriptor) {
    std::string transportName;
    std::ostringstream port;
    struct sockaddr_in remote;
    SocketTransport* transport;
    Protocol* protocol;
    int descriptor;
    socklen_t remoteLength = sizeof(remote);

    descriptor = accept(acceptDescriptor, reinterpret_cast<struct sockaddr*>(&remote), &remoteLength);

    if (descriptor == -1) {
      std::cerr << PREFIX "Unable to accept new connection, aborting" << std::endl;
      return -1;
    }

    port << remote.sin_port;
    transportName = inet_ntoa(remote.sin_addr);
    transportName += ":";
    transportName += port.str();

    transport = new SocketTransport(descriptor, transportName);

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

    std::cout << TRANSPORT_PREFIX(transport) << "Connection established" << std::endl;
    return descriptor;
  }

  int NetworkReactor::createAcceptSocket(int portNumber) {
    int yes = 1;
    int descriptor;
    int status;
    struct sockaddr_in remote;

    descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if (descriptor == -1) {
      return -1;
    }

    status = setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (status == -1) {
      return -1;
    }

    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = htonl(INADDR_ANY);
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
    int acceptDescriptor;
    std::map<SocketTransport*, Protocol*>::iterator i;
    int status;
    int largestSocket = -1;

    this->protocolCreator = protocolCreator;
    largestSocket = acceptDescriptor = createAcceptSocket(portNumber);
    
    if (acceptDescriptor == -1) {
      std::cerr << PREFIX "Unable to create socket, aborting" << std::endl;
      return;
    }

    while (!done) {
      fd_set read_set;
      
      // Clear sets
      FD_ZERO(&read_set);

      // Add accept socket
      FD_SET(acceptDescriptor, &read_set);

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
      if (FD_ISSET(acceptDescriptor, &read_set)) {
	int newSocket = handleNewConnection(acceptDescriptor);
	largestSocket = MAX(largestSocket, newSocket);
      }
    }

    // We won't reach this statement... :-/
    stopServing();
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

      while (!transport.isClosed()) {
	protocol->onDataReceived(transport.receive());
      }

      std::cout << TRANSPORT_PREFIX(&transport) << "Lost connection, exiting" << std::endl;
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

