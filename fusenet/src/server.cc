/**
 * @file
 *
 * This file contains the server implementation.
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
    std::cout << PREFIX << "Replying to create newsgroup '" 
	      << newsgroupName << "'" << std::endl;
    replyCreateNewsgroup(database->createNewsgroup(newsgroupName));
  }

  void Server::onDeleteNewsgroup(int newsgroupIdentifier) {
    std::cout << PREFIX << "Replying to delete newsgroup " 
	      << newsgroupIdentifier << std::endl;
    replyDeleteNewsgroup(database->deleteNewsgroup(newsgroupIdentifier));
  }

  void Server::onListArticles(int newsgroupIdentifier) {
    ArticleList_t articleList;
    Status_t status;

    std::cout << PREFIX << "Getting list of list articles" << std::endl;
    status = database->listArticles(newsgroupIdentifier, articleList);

    std::cout << PREFIX << "Replying to list articles" << std::endl;
    replyListArticles(status, articleList);
  }

  void Server::onCreateArticle(int newsgroupIdentifier,
			       Article_t& article) {
    Status_t status;

    std::cout << PREFIX << "Creating article " << article.id 
	      << " in newsgroup " << newsgroupIdentifier << std::endl;
    status = database->createArticle(newsgroupIdentifier, article);

    std::cout << PREFIX << "Replying to create article" << std::endl;
    replyCreateArticle(status);
  }
  
  void Server::onDeleteArticle(int newsgroupIdentifier,
			       int articleIdentifier) {
    Status_t status;

    std::cout << PREFIX << "Deleting article " << articleIdentifier
	      << " in newsgroup " << newsgroupIdentifier << std::endl;
    status = database->deleteArticle(newsgroupIdentifier, articleIdentifier);

    std::cout << PREFIX << "Replying to delete article" << std::endl;
    replyDeleteArticle(status);
  }

  void Server::onGetArticle(int newsgroupIdentifier,
			    int articleIdentifier) {
    Article_t article;
    Status_t status;

    std::cout << PREFIX << "Getting article " << articleIdentifier
	      << " in newsgroup " << newsgroupIdentifier << std::endl;
    status = database->getArticle(newsgroupIdentifier, articleIdentifier, article);

    std::cout << PREFIX << "Replying to get article" << std::endl;
    replyGetArticle(status, article);
  }

  void Server::onConnectionMade(void) {
    std::cout << PREFIX << "Connection established" << std::endl;
  }

  void Server::onConnectionLost(void) {
    std::cout << PREFIX << "Connection lost" << std::endl;
  }

}

