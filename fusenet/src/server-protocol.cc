/**
 * @file server-protocol.cc
 *
 * This file contains the implementation of the server protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <cassert>
#include <string>

#include "server-protocol.h"

namespace fusenet {

  void ServerProtocol::handleListNewsgroups(void) {
    receiveCommand();
    onListNewsgroups();
  }

  void ServerProtocol::handleCreateNewsgroup(void) {
    std::string name;
    receiveParameter(name);
    receiveCommand();
    onCreateNewsgroup(name);
  }

  void ServerProtocol::handleDeleteNewsgroup(void) {
    int id;
    receiveParameter(&id);
    receiveCommand();
    onDeleteNewsgroup(id);
  }

  void ServerProtocol::handleListArticles(void) {
    int group;
    receiveParameter(&group);
    receiveCommand();
    onListArticles(group);
  }
    
  void ServerProtocol::handleCreateArticle(void) {
    int ngid;
    Article_t article;
    receiveParameter(&ngid);
    receiveParameter(article.title);
    receiveParameter(article.author);
    receiveParameter(article.text);
    receiveCommand();
    onCreateArticle(ngid, article);
  }

  void ServerProtocol::handleDeleteArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
    onDeleteArticle(gid, aid);
  }

  void ServerProtocol::handleGetArticle(void) {
    int gid, aid;
    receiveParameter(&gid);
    receiveParameter(&aid);
    receiveCommand();
    onGetArticle(gid, aid);
  }

  void ServerProtocol::onDataReceived(uint8_t data) {

    switch (data) {
    case COM_LIST_NG:
      handleListNewsgroups();
      break;
    case COM_CREATE_NG:
      handleCreateNewsgroup();
      break;
    case COM_DELETE_NG:
      handleDeleteNewsgroup();
      break;
    case COM_LIST_ART:
      handleListArticles();
      break;
    case COM_CREATE_ART:
      handleCreateArticle();
      break;
    case COM_DELETE_ART:
      handleDeleteArticle();
      break;
    case COM_GET_ART:
      handleGetArticle();
      break;
    default:
      std::cerr << "Error, unknown command byte: " 
		<< static_cast<int>(data) << std::endl;
      break;
    }
  }
}

