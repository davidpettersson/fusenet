#ifndef MEMORY_SERVER_H
#define MEMORY_SERVER_H

/**
 * @file memory-server.h
 *
 * This file contains the message protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <string>
#include <vector>

#include "server-protocol.h"

namespace fusenet {

  /**
   * Memory server class. This class contains the server application
   * that uses random access memory as a backend.
   */
  class MemoryServer : public ServerProtocol {

  public:

    /**
     * Creates a memory server instance.
     *
     * @param transport the transport
     */
    MemoryServer(Transport* transport);

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
     * Internal newsgroup counter.
     */
    int newsgroupCounter;

    /**
     * Internal newsgroup list.
     */
    NewsgroupList_t newsgroupList;
  };

}

#endif

