/**
 * @file main.cc
 *
 * This file contains the application entry point. Just having one
 * main.cc simplifies the build process, since we don't have to mess
 * about with several different make targets.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cstdio>

#include "client-protocol-creator.h"
#include "client-protocol.h"
#include "network-reactor.h"
#include "protocol.h"
#include "server-protocol-creator.h"
#include "server-protocol.h"
#include "transport.h"

#ifdef ENABLE_ECHO
#include "echo.h"
#endif

namespace fusenet {

  class TerminalTransport : public Transport {
  public:
    void send(uint8_t data) const {
      printf("%02x ", data);
    }
    ~TerminalTransport(void) {
      printf("\n");
    }
  };
}

static void serverBehaviour(void) {
  std::cout << "fusenet server started." << std::endl;
  fusenet::NetworkReactor networkReactor;

#ifdef ENABLE_ECHO
  fusenet::EchoServerCreator creator;
#else
  fusenet::ServerProtocolCreator creator;
#endif

  networkReactor.serve(4000, &creator);
}

static void clientBehaviour(void) {
  std::cout << "fusenet client started." << std::endl;
  fusenet::NetworkReactor networkReactor;

#ifdef ENABLE_ECHO
  fusenet::EchoClientCreator creator;
#else
  fusenet::ClientProtocolCreator creator;
#endif

  networkReactor.initiate("pi", 4000, &creator);
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

