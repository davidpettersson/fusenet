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
    virtual void onListNewsgroups(void) = 0;

    /**
     * Reply list newsgroups.
     */
    void replyListNewsgroups(NewsgroupList_t& newsgroupList);

    /**
     * Create newsgroup.
     */
    virtual void onCreateNewsgroup(std::string& newsgroupName) = 0;

    /**
     * Reply create newsgroup.
     */
    void replyCreateNewsgroup(Status_t status);

    /**
     * Delete newsgroup.
     */
    virtual void onDeleteNewsgroup(int newsgroupIdentifier) = 0;

    /**
     * Reply create newsgroup.
     */
    void replyDeleteNewsgroup(Status_t status);

    /**
     * List articles.
     */
    virtual void onListArticles(int newsgroupIdentifier) = 0;

    /**
     * Reply list articles.
     */
    void replyListArticles(Status_t status,
			   ArticleList_t& articleList);
    
    /**
     * Create article.
     */
    virtual void onCreateArticle(int newsgroupIdentifier,
				 Article_t& article) = 0;

    /**
     * Delete article.
     */
    virtual void onDeleteArticle(int newsgroupIdentifier,
				 int articlIdentifier) = 0;

    /**
     * Get article.
     */
    virtual void onGetArticle(int newsgroupIdentifier,
			      int articleIdentifier) = 0;

    /**
     * Called on made connection.
     */
    virtual void onConnectionMade(void) = 0;

    /**
     * Called on lost connection.
     */
    virtual void onConnectionLost(void) = 0;

  private:

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
     * Send status message.
     */
    void sendStatus(Status_t);

    /**
     * Called on data receival.
     *
     * @param data the data received
     */
    void onDataReceived(uint8_t data);
  };

}

#endif

