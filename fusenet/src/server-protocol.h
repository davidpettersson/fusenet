#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

/**
 * @file server-protocol.h
 *
 * This file contains the message protocol class.
 *
 * @author David Pettersson <david@shebang.nu>
 */

#include <string>
#include <vector>

#include "message-protocol.h"

namespace fusenet {

  /**
   * Server protocol class. This class extends the base protocol class
   * with the ability to send and parse server messages as defined in
   * Holm's project description.
   *
   * This class currently contains the user interaction beside the
   * protocol handling. The interaction should be placed in a seperate
   * class.
   */
  class ServerProtocol : public MessageProtocol {

  public:

    /**
     * Creates a server protocol instance.
     *
     * @param transport the transport
     */
    ServerProtocol(Transport* transport) : MessageProtocol(transport) { }

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
     * Handle list newsgroups.
     */
    void handleListNewsgroups(void);

    /**
     * Handle create newsgroup.
     */
    void handleCreateNewsgroup(void);

    /**
     * Handle delete newsgroup.
     */
    void handleDeleteNewsgroup(void);

    /**
     * Handle list articles.
     */
    void handleListArticles(void);

    /**
     * Handle create article.
     */
    void handleCreateArticle(void);

    /**
     * Handle delete article.
     */
    void handleDeleteArticle(void);

    /**
     * Handle get article.
     */
    void handleGetArticle(void);

    /**
     * Called on data receival.
     *
     * @param data the data received
     */
    void onDataReceived(uint8_t data);
  };

}

#endif

