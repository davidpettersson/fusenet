/**
 * @file memory-server.cc
 *
 * This file contains the implementation of the memory server class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <string>

#include "memory-server.h"

#define PREFIX "[MemoryServer] [" << transport->getName() << "] "

namespace fusenet {

  MemoryServer::MemoryServer(Transport* transport) : ServerProtocol(transport) {
    newsgroupCounter = 0;
  }

  void MemoryServer::onListNewsgroups(void) {
    std::cout << PREFIX << "Replying to list newsgroups" << std::endl;

    // FIXME: Replace with calls to database interface
    replyListNewsgroups(newsgroupList);
  }

  void MemoryServer::onCreateNewsgroup(std::string& newsgroupName) {
    Newsgroup_t newsgroup;

    std::cout << PREFIX << "Creating newsgroup '" << newsgroupName << "'" << std::endl;
    
    // FIXME: Replace with calls to database interface
    newsgroup.id = newsgroupCounter++;
    newsgroup.name = newsgroupName;
    newsgroupList.push_back(newsgroup);

    std::cout << PREFIX << "Replying to create newsgroup" << std::endl;
    replyCreateNewsgroup(STATUS_SUCCESS);
  }

  void MemoryServer::onDeleteNewsgroup(int newsgroupIdentifier) {
    std::cout << PREFIX << "Replying to delete newsgroup" << std::endl;

    // FIXME: Broken!
    replyDeleteNewsgroup(STATUS_SUCCESS);
  }

  void MemoryServer::onListArticles(int newsgroupIdentifier) {
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

  void MemoryServer::onCreateArticle(int newsgroupIdentifier,
				       Article_t& article) {
    sendCommand(ANS_CREATE_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }
  
  void MemoryServer::onDeleteArticle(int newsgroupIdentifier,
				       int articlIdentifier) {
    sendCommand(ANS_DELETE_ART);
    sendCommand(ANS_ACK);
    sendCommand(ANS_END);
  }

  void MemoryServer::onGetArticle(int newsgroupIdentifier,
				    int articleIdentifier) {
    sendCommand(ANS_GET_ART);
    sendCommand(ANS_NAK);
    sendCommand(ERR_NG_DOES_NOT_EXIST);
    sendCommand(ANS_END);
  }

  void MemoryServer::onConnectionMade(void) {
    std::cout << PREFIX << "Connection established" << std::endl;
  }

  void MemoryServer::onConnectionLost(void) {
    std::cout << PREFIX << "Connection lost" << std::endl;
  }

}

