#ifndef SERVER_H
#define SERVER_H

/**
 * @file
 *
 * This file contains the server interface.
 */

#include <string>
#include <vector>

#include "server-protocol.h"
#include "database.h"

namespace fusenet {

  /**
   * Server class.
   */
  class Server : public ServerProtocol {

  public:

    /**
     * Creates a server instance.
     *
     * @param transport the transport
     * @param database the database
     */
    Server(Transport* transport, Database* database);

    /**
     * List newsgroups callback.
     */
    void onListNewsgroups(void);

    /**
     * Create newsgroup.
     *
     * @param newsgroupName the newsgroup name
     */
    void onCreateNewsgroup(std::string& newsgroupName);

    /**
     * Delete newsgroup.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void onDeleteNewsgroup(int newsgroupIdentifier);

    /**
     * List articles.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void onListArticles(int newsgroupIdentifier);

    /**
     * Create article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param article the article
     */
    void onCreateArticle(int newsgroupIdentifier,
			 Article_t& article);

    /**
     * Delete article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void onDeleteArticle(int newsgroupIdentifier,
			 int articleIdentifier);

    /**
     * Get article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void onGetArticle(int newsgroupIdentifier,
		      int articleIdentifier);

  private:

    /**
     * Called on made connection.
     */
    void onConnectionMade(void);

    /**
     * Called on lost connection.
     */
    void onConnectionLost(void);

    /**
     * Database to use.
     */
    Database* database;
  };

}

#endif

