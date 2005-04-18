/**
 * @file
 *
 * This file contains the client protocol implementation.
 */

#include <cassert>

#include "client-protocol.h"

#define PRINT_ERR_NG  std::cout << "Newsgroup does not exist" << std::endl
#define PRINT_ERR_ART std::cout << "Article does not exist" << std::endl
#define PRINT_ERR_UNK std::cout << "Unknown error, error message not defined." << std::endl


namespace fusenet {
  
  static Status_t TranslateError(MessageIdentifier_t identifier) {
    Status_t status;
    
    switch (identifier) {
    case ERR_NG_ALREADY_EXISTS:
      status = STATUS_FAILURE_ALREADY_EXISTS;
      break;
    case ERR_NG_DOES_NOT_EXIST:
      status = STATUS_FAILURE_N_DOES_NOT_EXIST;
      break;
    case ERR_ART_DOES_NOT_EXIST:
      status = STATUS_FAILURE_A_DOES_NOT_EXIST;
      break;
    default:
      status = STATUS_FAILURE;
      break;
    }
    
    return status;
  }

  void ClientProtocol::listNewsgroups(void) {
    sendCommand(COM_LIST_NG);
    sendCommand(COM_END);
  }
  
  void ClientProtocol::receiveListNewsgroups(void) {
    size_t n, i;
    NewsgroupList_t newsgroupList;
    Newsgroup_t newsgroup;
    
    receiveParameter(reinterpret_cast<int*>(&n));
    
    for (i = 0; i < n; i++) {
      receiveParameter(&newsgroup.id);
      receiveParameter(newsgroup.name);
      newsgroupList.push_back(newsgroup);
    }

    expectCommand(ANS_END);
    onListNewsgroups(STATUS_SUCCESS, newsgroupList);
  }

  void ClientProtocol::createNewsgroup(const std::string& name) {
    sendCommand(COM_CREATE_NG);
    sendParameter(name);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveCreateNewsgroup(void) {
    Status_t status;

    if (receiveCommand() == ANS_ACK) {
      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onCreateNewsgroup(status);
  }

  void ClientProtocol::deleteNewsgroup(int newsgroupIdentifier) {
    sendCommand(COM_DELETE_NG);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveDeleteNewsgroup(void) {
    Status_t status;

    if (receiveCommand() == ANS_ACK) {
      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onDeleteNewsgroup(status);
  }

  void ClientProtocol::listArticles(int newsgroupIdentifier) {
    sendCommand(COM_LIST_ART);
    sendParameter(newsgroupIdentifier);
    sendCommand(COM_END);    
  }

  void ClientProtocol::receiveListArticles(void) {
    ArticleList_t articleList;
    Article_t article;
    Status_t status;
    int n;
    int i;

    if (receiveCommand() == ANS_ACK) {
      receiveParameter(&n);

      for (i = 0; i < n; i++) {
	receiveParameter(&article.id);
	receiveParameter(article.title);
	articleList.push_back(article);
      }

      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onListArticles(status, articleList);
  }

  void ClientProtocol::createArticle(int newsgroupIdentifier,
				     const std::string& title,
				     const std::string& author,
				     const std::string& text) {
    sendCommand(COM_CREATE_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(title);
    sendParameter(author);
    sendParameter(text);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveCreateArticle(void) {
    Status_t status;

    if (receiveCommand() == ANS_ACK) {
      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onCreateArticle(status);
  }
  
  void ClientProtocol::deleteArticle(int newsgroupIdentifier,
				     int articleIdentifier) {
    sendCommand(COM_DELETE_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveDeleteArticle(void) {
    Status_t status;

    if (receiveCommand() == ANS_ACK) {
      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onDeleteArticle(status);
  }

  void ClientProtocol::getArticle(int newsgroupIdentifier,
				  int articleIdentifier) {
    sendCommand(COM_GET_ART);
    sendParameter(newsgroupIdentifier);
    sendParameter(articleIdentifier);
    sendCommand(COM_END);
  }

  void ClientProtocol::receiveGetArticle(void) {
    Status_t status;
    Article_t article;

    if (receiveCommand() == ANS_ACK) {
      receiveParameter(article.title);
      receiveParameter(article.author);
      receiveParameter(article.text);
      status = STATUS_SUCCESS;
    } else {
      status = TranslateError(receiveCommand());
    }

    expectCommand(ANS_END);
    onGetArticle(status, article);
  }

  void ClientProtocol::onDataReceived(uint8_t data) {
    switch (data) {
    case ANS_LIST_NG:
      receiveListNewsgroups();
      break;
    case ANS_CREATE_NG:
      receiveCreateNewsgroup();
      break;
    case ANS_DELETE_NG:
      receiveDeleteNewsgroup();
      break;
    case ANS_LIST_ART:
      receiveListArticles();
      break;
    case ANS_CREATE_ART:
      receiveCreateArticle();
      break;
    case ANS_DELETE_ART:
      receiveDeleteArticle();
      break;
    case ANS_GET_ART:
      receiveGetArticle();
      break;
    default:
      std::cout << "Throwing away data " << static_cast<int>(data)
		<< " (this is a bad thing)" << std::endl;
      break;
    }
  }
}

