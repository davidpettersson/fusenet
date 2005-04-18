#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

/**
 * @file
 *
 * This file contains the server protocol interface.
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
   * All servers should inherit the functionality from this class.
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
     *
     * @param newsgroupList the list of newsgroups
     */
    void replyListNewsgroups(NewsgroupList_t& newsgroupList);

    /**
     * Create newsgroup.
     *
     * @param newsgroupName the name of the newsgroup
     */
    virtual void onCreateNewsgroup(std::string& newsgroupName) = 0;

    /**
     * Reply create newsgroup.
     *
     * @param status the status
     */
    void replyCreateNewsgroup(Status_t status);

    /**
     * Delete newsgroup.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    virtual void onDeleteNewsgroup(int newsgroupIdentifier) = 0;

    /**
     * Reply create newsgroup.
     *
     * @param status the status
     */
    void replyDeleteNewsgroup(Status_t status);

    /**
     * List articles.
     *
     * @param newsgroupIdentifier the newsgroup identifiers
     */
    virtual void onListArticles(int newsgroupIdentifier) = 0;

    /**
     * Reply list articles.
     *
     * @param status the status
     * @param articleList the article list
     */
    void replyListArticles(Status_t status,
			   ArticleList_t& articleList);
    
    /**
     * Create article.
     *
     * @param newsgroupIdentifier the newsgroup identifier.
     * @param article the article
     */
    virtual void onCreateArticle(int newsgroupIdentifier,
				 Article_t& article) = 0;

    /**
     * Reply create article.
     *
     * @param status the status
     */
    void replyCreateArticle(Status_t status);

    /**
     * Delete article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    virtual void onDeleteArticle(int newsgroupIdentifier,
				 int articleIdentifier) = 0;

    /**
     * Reply delete article.
     *
     * @param status the status
     */
    void replyDeleteArticle(Status_t status);

    /**
     * Get article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    virtual void onGetArticle(int newsgroupIdentifier,
			      int articleIdentifier) = 0;

    /**
     * Reply get article.
     *
     * @param status the status
     * @param article the article
     */
    void replyGetArticle(Status_t status,
			 Article_t& article);

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

