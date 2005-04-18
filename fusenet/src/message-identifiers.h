#ifndef MESSAGE_IDENTIFIERS_H
#define MESSAGE_IDENTIFIERS_H

/**
 * @file
 *
 * This file contains the message identifier.
 */

namespace fusenet {

  /**
   * Enumeration of all message identifiers that are used in the
   * protocol between the server and the client. The code has more or
   * less been taken from the protocol.h header file by Per Holm and
   * friends.
   */

  typedef enum {
    // Command identifiers
    COM_LIST_NG    = 1,           //!< List newsgroups
    COM_CREATE_NG  = 2,           //!< Create newsgroup
    COM_DELETE_NG  = 3,           //!< Delete newsgroup
    COM_LIST_ART   = 4,           //!< List articles
    COM_CREATE_ART = 5,           //!< Create article
    COM_DELETE_ART = 6,           //!< Delete article
    COM_GET_ART    = 7,           //!< Get article
    COM_END        = 8,           //!< Command end (not really a command)
    
    // Answer identifiers
    ANS_LIST_NG    = 20,          //!< Answer list newsgroups
    ANS_CREATE_NG  = 21,          //!< Answer create newsgroup
    ANS_DELETE_NG  = 22,          //!< Answer delete newsgroup
    ANS_LIST_ART   = 23,          //!< Answer list articles
    ANS_CREATE_ART = 24,          //!< Answer create article
    ANS_DELETE_ART = 25,          //!< Answer delete article
    ANS_GET_ART    = 26,          //!< Answer get article
    ANS_END        = 27,          //!< Answer end
    ANS_ACK        = 28,          //!< Acknowledge (not really answers)
    ANS_NAK        = 29,          //!< Negative acknowledge (not really answers)

    // Parameter identifiers
    PAR_STRING     = 40,          //!< String
    PAR_NUM        = 41,          //!< Number
        
    // Error code identifiers
    ERR_NG_ALREADY_EXISTS  = 50,  //!< Newsgroup already exists
    ERR_NG_DOES_NOT_EXIST  = 51,  //!< Newsgroup does not exist
    ERR_ART_DOES_NOT_EXIST = 52   //!< Article does not exist
  }
  MessageIdentifier_t;
}

#endif

