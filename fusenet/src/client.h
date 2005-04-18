#ifndef CLIENT_H
#define CLIENT_H

/**
 * @file
 *
 * This file contains the client interface class.
 */

#include "client-protocol.h"

namespace fusenet {

  /**
   * This is the basic client implementation.
   */
  class Client : public ClientProtocol {

  public:

    /**
     * Creates a client instance and gives it a transport to use for
     * future communication.
     *
     * @param transport the transport
     */
    Client(Transport* const transport) : ClientProtocol(transport) { }

    /**
     * Destroys an instance.
     */
    virtual ~Client(void) { };

  private:

    /**
     * Called on made connection.
     */
    void onConnectionMade(void);

    /**
     * Called on list newsgroups.
     *
     * @param status the status
     * @param newsgroupList the newsgroup list
     */
    void onListNewsgroups(Status_t status,
			  NewsgroupList_t& newsgroupList);

    /**
     * Called on create newsgroup.
     *
     * @param status the status
     */
    void onCreateNewsgroup(Status_t status);

    /**
     * Called on delete newsgroup.
     *
     * @param status the status
     */
    void onDeleteNewsgroup(Status_t status);

    /**
     * Called on list articles.
     *
     * @param status the status
     * @param articleList the article list
     */
    void onListArticles(Status_t status,
			ArticleList_t& articleList);

    /**
     * Called on create article.
     *
     * @param status the status
     */
    void onCreateArticle(Status_t status);

    /**
     * Called on delete article.
     *
     * @param status the status
     */
    void onDeleteArticle(Status_t status);

    /**
     * Called on get article.
     *
     * @param status the status
     * @param article the article
     */
    void onGetArticle(Status_t status,
		      Article_t& article);

    /**
     * Called on lost connection.
     */
    void onConnectionLost(void);

    /**
     * Print the help message.
     */
    void printHelpMsg(void);

    /**
     * Ask user for string value.
     *
     * @param question
     * @return the answer
     */
    std::string askString(const std::string& question);
    
    /**
     * Ask user for integer value.
     *
     * @param question
     * @return the answer
     */
    int askInteger(const std::string& question);
    
    /**
     * Interact with the user.
     */
    void interact(void);
  };

}

#endif

