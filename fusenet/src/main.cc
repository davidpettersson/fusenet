/**
 * @file main.cc
 *
 * This file contains the application entry point. Just having one
 * main.cc simplifies the build process, since we don't have to mess
 * about with several different make targets.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include "network-reactor.h"
#include "protocol-creator.h"
#include "protocol.h"
#include "transport.h"

namespace fusenet {

  class EchoServer : public Protocol {
  public:
    EchoServer(const Transport* transport) : Protocol(transport) {
      std::cout << "[" << this << "] Created" << std::endl;
    }

    void onDataReceived(uint8_t data) {
      std::cout << "[" << this << "] Echoing back '" << data << "'" << std::endl;
      transport->send(data);
    }

    void onConnectionLost(void) {
      std::cout << "[" << this << "] Connection lost" << std::endl;
    }

    void onConnectionMade(void) {
      std::cout << "[" << this << "] Connection made" << std::endl;
    }

    ~EchoServer(void) {
      std::cout << "[" << this << "] Destroyed" << std::endl;
    }
  };

  class EchoServerCreator : public ProtocolCreator {
  public:
    Protocol* create(const Transport* transport) const {
      return new EchoServer(transport);
    }
  };

  class EchoClient : public Protocol {
  public:
    EchoClient(const Transport* transport) : Protocol(transport) {
      std::cout << "Created!" << std::endl;
    }

    void onDataReceived(uint8_t data) {
      std::cout << "Server said: " << data << std::endl;
      std::cout << "You say: ";
      std::cin >> data;
      transport->send(data);
    }

    void onConnectionLost(void) {
    }

    void onConnectionMade(void) {
      uint8_t data;
      std::cout << "You say: ";
      std::cin >> data;
      transport->send(data);
    }

    ~EchoClient(void) {
      std::cout << "Destroyed!" << std::endl;
    }
  };

  class EchoClientCreator : public ProtocolCreator {
  public:
    Protocol* create(const Transport* transport) const {
      return new EchoClient(transport);
    }
  };

  class TerminalTransport : public Transport {
  public:
    void send(uint8_t* data, size_t size) const {
      size_t i;
      for (i = 0; i < size; i++) {
	std::cout << data[i] << std::endl;
      }
    }
  };
}

static void serverBehaviour(void) {
  fusenet::EchoServerCreator creator;
  fusenet::NetworkReactor networkReactor;
  networkReactor.serve(4000, &creator);
}

static void clientBehaviour(void) {
  fusenet::EchoClientCreator creator;
  fusenet::NetworkReactor networkReactor;
  networkReactor.initiate("localhost", 4000, &creator);
}

static void printUsage(void) {
  std::cerr << "usage: fusenet [--client|--server]" << std::endl;
}

int main(int argc, char* argv[]) {

  /*
   * This argument handling is ugly, fix it sometime. :-)
   */

  if (argc != 2) {
    printUsage();
  } else if (strcmp(argv[1], "--client") == 0) {
    clientBehaviour();
  } else if (strcmp(argv[1], "--server") == 0) {
    serverBehaviour();
  } else {
    printUsage();
  }

  return 0;
}

