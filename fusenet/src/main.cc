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

#include "client-creator.h"
#include "client.h"
#include "filesystem-database.h"
#include "memory-database.h"
#include "network-reactor.h"
#include "protocol.h"
#include "server-creator.h"
#include "server.h"
#include "transport.h"

static void serverBehaviour(int port, bool useMemoryBackend) {
  std::cout << "Fusenet server started" << std::endl;
  fusenet::NetworkReactor networkReactor;

  if (useMemoryBackend) {
    std::cout << "Memory backend selected" << std::endl;
    fusenet::MemoryDatabase database;
    fusenet::ServerCreator creator(&database);
    networkReactor.serve(port, &creator);
  } else {
    std::cout << "File system backend selected" << std::endl;
    fusenet::FilesystemDatabase database;
    fusenet::ServerCreator creator(&database);
    networkReactor.serve(port, &creator);
  }
}

static void clientBehaviour(const char* const host, int port) {
  std::cout << "Fusenet client started" << std::endl;
  fusenet::NetworkReactor networkReactor;
  fusenet::ClientCreator creator;
  networkReactor.initiate(host, port, &creator);
}

static void printUsage(void) {
  std::cerr << "usage: fusenet [ --client HOST PORT | --server PORT ( mem | fs ) ]" << std::endl;
}

int main(int argc, char* argv[]) {

  /*
   * This argument handling is ugly, fix it sometime. :-)
   */

  if (argc == 4 && strcmp(argv[1], "--client") == 0) {
    clientBehaviour(argv[2], atoi(argv[3]));
  } else if (argc == 4 && strcmp(argv[1], "--server") == 0) {
    serverBehaviour(atoi(argv[2]), strcmp(argv[3], "mem") == 0);
  } else {
    printUsage();
  }

  return 0;
}

