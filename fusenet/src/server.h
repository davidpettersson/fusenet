#ifndef SERVER_H
#define SERVER_H

/**
 * @file server.h
 *
 * This file contains the message protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
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
     */
    Server(Transport* transport, Database* database);

    /**
     * List newsgroups callback.
     */
    void onListNewsgroups(void);

    /**
     * Create newsgroup.
     */
    void onCreateNewsgroup(std::string& newsgroupName);

    /**
     * Delete newsgroup.
     */
    void onDeleteNewsgroup(int newsgroupIdentifier);

    /**
     * List articles.
     */
    void onListArticles(int newsgroupIdentifier);

    /**
     * Create article.
     */
    void onCreateArticle(int newsgroupIdentifier,
			 Article_t& article);

    /**
     * Delete article.
     */
    void onDeleteArticle(int newsgroupIdentifier,
			 int articlIdentifier);

    /**
     * Get article.
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
     * Database.
     */
    Database* database;
  };

}

#endif

