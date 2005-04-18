#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

/**
 * @file
 *
 * This file contains the client protocol interface.
 */

#include <string>
#include <vector>

#include "fusenet-types.h"
#include "message-protocol.h"

namespace fusenet {
  
  /**
   * Client protocol class. This class extends the message protocol
   * class with the ability to send and parse client messages as
   * defined in Holm's project description.
   *
   * All client applications should inherit from this class. However,
   * to be useful, the clients must not return from the
   * onConnectionMade call without having called on the command
   * methods, or the client will never be able to communicate.
   */
  class ClientProtocol : public MessageProtocol {

  public:

    /**
     * Creates a client protocol instance.
     *
     * @param transport the transport
     */
    ClientProtocol(Transport* transport) : MessageProtocol(transport) { }

    /**
     * Called on made connection.
     */
    virtual void onConnectionMade(void) = 0;

    /**
     * List newsgroups.
     */
    void listNewsgroups(void);

    /**
     * List newsgroups callback.
     *
     * @param status the status
     * @param newsgroupList the newsgroup list
     */
    virtual void onListNewsgroups(Status_t status,
				  NewsgroupList_t& newsgroupList) = 0;

    /**
     * Create newsgroup.
     *
     * @param name the name of the new newsgroup
     */
    void createNewsgroup(const std::string& name);

    /**
     * Create newsgroup callback.
     *
     * @param status the status
     */
    virtual void onCreateNewsgroup(Status_t status) = 0;

    /**
     * Delete newsgroup.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void deleteNewsgroup(int newsgroupIdentifier);

    /**
     * Delete newsgroup callback.
     *
     * @param status the status
     */
    virtual void onDeleteNewsgroup(Status_t status) = 0;

    /**
     * List articles.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void listArticles(int newsgroupIdentifier);

    /**
     * List articles callback.
     *
     * @param status the status
     * @param articleList the article list
     */
    virtual void onListArticles(Status_t status,
				ArticleList_t& articleList) = 0;
    
    /**
     * Create article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param title the article title
     * @param author the article author
     * @param text the article text
     */
    void createArticle(int newsgroupIdentifier,
		       const std::string& title,
		       const std::string& author,
		       const std::string& text);

    /**
     * Create article callback.
     *
     * @param status the status
     */
    virtual void onCreateArticle(Status_t status) = 0;

    /**
     * Delete article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void deleteArticle(int newsgroupIdentifier,
		       int articleIdentifier);

    /**
     * Delete article callback.
     *
     * @param status the status.
     */
    virtual void onDeleteArticle(Status_t status) = 0;

    /**
     * Get article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void getArticle(int newsgroupIdentifier,
		    int articleIdentifier);

    /**
     * Get article callback.
     *
     * @param status the status
     * @param article the article
     */
    virtual void onGetArticle(Status_t status,
			      Article_t& article) = 0;

    /**
     * Called on lost connection.
     */
    virtual void onConnectionLost(void) = 0;

  private:

    /**
     * Receive list newsgroup answer.
     */
    void receiveListNewsgroups(void);

    /**
     * Receive create newsgroup answer.
     */
    void receiveCreateNewsgroup(void);

    /**
     * Receive create newsgroup answer.
     */
    void receiveDeleteNewsgroup(void);

    /**
     * Receive list newsgroup answer.
     */
    void receiveListArticles(void);

    /**
     * Receive create article answer.
     */
    void receiveCreateArticle(void);

    /**
     * Receive delete article answer.
     */
    void receiveDeleteArticle(void);

    /**
     * Receive get article answer.
     */
    void receiveGetArticle(void);

    /**
     * Called on data receival.
     *
     * @param data the data received
     */
    void onDataReceived(uint8_t data);
  };

}

#endif

