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

  static MessageIdentifier_t TranslateError(Status_t identifier) {
    MessageIdentifier_t status = ANS_ACK;
    
    switch (identifier) {
    case STATUS_FAILURE_ALREADY_EXISTS:
      status = ERR_NG_ALREADY_EXISTS;
      break;
    case STATUS_FAILURE_N_DOES_NOT_EXIST:
      status = ERR_NG_DOES_NOT_EXIST;
      break;
    case STATUS_FAILURE_A_DOES_NOT_EXIST:
      status = ERR_ART_DOES_NOT_EXIST;
      break;
    case STATUS_FAILURE:
      status = ERR_NG_DOES_NOT_EXIST; // This is broken
      break;
    default:
      assert(0 == "This cannot happen");
      break;
    }
    
    return status;
  }

  void ServerProtocol::replyListNewsgroups(NewsgroupList_t& newsgroupList) {
    NewsgroupList_t::iterator i;

    sendCommand(ANS_LIST_NG);
    sendParameter(newsgroupList.size());

    for (i = newsgroupList.begin(); i != newsgroupList.end(); i++) {
      Newsgroup_t& newsgroup = *i;
      sendParameter(newsgroup.id);
      sendParameter(newsgroup.name);
    }

    sendCommand(ANS_END);
  }

  void ServerProtocol::replyCreateNewsgroup(Status_t status) {
    sendCommand(ANS_CREATE_NG);
    sendStatus(status);
    sendCommand(ANS_END);
  }

  void ServerProtocol::replyDeleteNewsgroup(Status_t status) {
    sendCommand(ANS_DELETE_NG);
    sendStatus(status);
    sendCommand(ANS_END);
  }

  void ServerProtocol::replyListArticles(Status_t status,
					 ArticleList_t& articleList) {
    ArticleList_t::iterator i;

    sendCommand(ANS_LIST_ART);
    sendStatus(status);

    if (IS_SUCCESS(status)) {
      sendParameter(articleList.size());
      for (i = articleList.begin(); i != articleList.end(); i++) {
	Article_t& article = *i;
	sendParameter(article.id);
	sendParameter(article.title);
      }
    }

    sendCommand(ANS_END);
  }

  void ServerProtocol::replyCreateArticle(Status_t status) {
    sendCommand(ANS_CREATE_ART);
    sendStatus(status);
    sendCommand(ANS_END);
  }

  void ServerProtocol::replyDeleteArticle(Status_t status) {
    sendCommand(ANS_CREATE_ART);
    sendStatus(status);
    sendCommand(ANS_END);
  }

  void ServerProtocol::replyGetArticle(Status_t status,
				       Article_t& article) {
    sendCommand(ANS_GET_ART);
    sendStatus(status);

    if (IS_SUCCESS(status)) {
      sendParameter(article.title);
      sendParameter(article.author);
      sendParameter(article.text);
    }

    sendCommand(ANS_END);
  }

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

  void ServerProtocol::sendStatus(Status_t status) {
    if (IS_SUCCESS(status)) {
      sendCommand(ANS_ACK);
    } else {
      sendCommand(ANS_NAK);
      sendCommand(TranslateError(status));
    }
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

