#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

/**
 * @file client-protocol.h
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
   * Newsgroup type.
   */
  typedef std::pair<int, std::string> Newsgroup_t;

  /**
   * Article type.
   */
  typedef std::pair<int, std::string> Article_t;

  /**
   * Client protocol class. This class extends the base protocol class
   * with the ability to send and parse client messages as defined in
   * Holm's project description.
   *
   * This class currently contains the user interaction beside the
   * protocol handling. The interaction should be placed in a seperate
   * class.
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
     * List newsgroups.
     */
    void listNewsgroups(void);

    /**
     * List newsgroups callback.
     */
    void onListNewsgroups(std::vector<Newsgroup_t>& newsgroupList);

    /**
     * Create newsgroup.
     *
     * @param name the name of the new newsgroup
     */
    void createNewsgroup(const std::string& name);

    /**
     * Create newsgroup callback.
     *
     * @param success if successful
     */
    void onCreateNewsgroup(bool success);

    /**
     * Delete newsgroup.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void deleteNewsgroup(int newsgroupIdentifier);

    /**
     * Delete newsgroup callback.
     *
     * @param success if successful
     */
    void onDeleteNewsgroup(bool success);

    /**
     * List articles.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     */
    void listArticles(int newsgroupIdentifier);

    /**
     * List articles answer.
     *
     * @param success
     * @param articleList the article list
     */
    void onListArticles(bool succces,
			std::vector<Article_t> articleList);

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
     * Delete article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void deleteArticle(int newsgroupIdentifier,
		       int articleIdentifier);

    /**
     * Get article.
     *
     * @param newsgroupIdentifier the newsgroup identifier
     * @param articleIdentifier the article identifier
     */
    void getArticle(int newsgroupIdentifier,
		    int articleIdentifier);

  private:

    /**
     * Called on made connection.
     */
    void onConnectionMade(void);

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

    /**
     * Called on data receival.
     *
     * @param data the data received
     */
    void onDataReceived(uint8_t data);
  };

}

#endif

