/**
 * @file server.cc
 *
 * This file contains the implementation of the server class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <string>

#include "server.h"

#define PREFIX "[Server] [" << transport->getName() << "] "

namespace fusenet {

  Server::Server(Transport* transport, Database* database) : ServerProtocol(transport) {
    this->database = database;
  }

  void Server::onListNewsgroups(void) {
    NewsgroupList_t newsgroupList;
    
    std::cout << PREFIX << "Getting list of newsgroups" << std::endl;
    database->getNewsgroupList(newsgroupList);

    std::cout << PREFIX << "Replying to list newsgroups" << std::endl;
    replyListNewsgroups(newsgroupList);
  }

  void Server::onCreateNewsgroup(std::string& newsgroupName) {
    std::cout << PREFIX << "Replying to create newsgroup '" << newsgroupName << "'" << std::endl;
    replyCreateNewsgroup(database->createNewsgroup(newsgroupName));
  }

  void Server::onDeleteNewsgroup(int newsgroupIdentifier) {
    std::cout << PREFIX << "Replying to delete newsgroup " << newsgroupIdentifier << std::endl;
    replyDeleteNewsgroup(database->deleteNewsgroup(newsgroupIdentifier));
  }

  void Server::onListArticles(int newsgroupIdentifier) {
    sendCommand(ANS_LIST_ART);

    if (newsgroupIdentifier == 1) {
      sendCommand(ANS_ACK);
      sendParameter(2);
      sendParameter(1);
      sendParameter("haXX0r");
      sendParameter(2);
      sendParameter("eller ngt");
    } else {
      sendCommand(ANS_NAK);
      sendCommand(ERR_NG_DOES_NOT_EXIST);
    }

    sendCommand(ANS_END);
  }

  void Server::onCreateArticle(int newsgroupIdentifier,
				       Article_t& article) {
    sendCommand(ANS_CREATE_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }
  
  void Server::onDeleteArticle(int newsgroupIdentifier,
				       int articlIdentifier) {
    sendCommand(ANS_DELETE_ART);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void Server::onGetArticle(int newsgroupIdentifier,
				    int articleIdentifier) {
    sendCommand(ANS_GET_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }

  void Server::onConnectionMade(void) {
    std::cout << PREFIX << "Connection established" << std::endl;
  }

  void Server::onConnectionLost(void) {
    std::cout << PREFIX << "Connection lost" << std::endl;
  }

}

