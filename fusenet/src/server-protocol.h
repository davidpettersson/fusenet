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

  typedef std::pair<int, std::string> Newsgroup_t;

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
    ServerProtocol(const Transport* transport) : MessageProtocol(transport) { }

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
     */
    void createNewsgroup(void);

    /**
     * Delete newsgroup.
     */
    void deleteNewsgroup(void);

    /**
     * List articles.
     */
    void listArticles(void);

    /**
     * Create article.
     */
    void createArticle(void);

    /**
     * Delete article.
     */
    void deleteArticle(void);

    /**
     * Get article.
     */
    void getArticle(void);

  private:

    /**
     * Called on made connection.
     */
    void onConnectionMade(void);

    /**
     * Receive newsgroup list.
     */
    void receiveListNewsgroups(void);

    /**
     * Called on data receival.
     *
     * @param data the data received
     */
    void onDataReceived(uint8_t data);
  };

}

#endif

