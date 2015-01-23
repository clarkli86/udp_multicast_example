/***************************************************************************
                          client.h  -  description
                             -------------------
    begin                : Sun Jan 1 2006
    copyright            : (C) 2006 by fuckall
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CLIENT_H
#define CLIENT_H

#include <qobject.h>


/**
  *@author fuckall
    This class is used to receive the udp packet from the server and convert it to a pixmap
  */
class QSocketDevice;
class QSocketNotifier;
class ClientView;

class Client : public QObject  {
    Q_OBJECT
public: 
	Client( ClientView *  );
	~Client();
  //bind the multicast address
  bool bind( const QString );

private:
  bool setSocketOpt( const QSocketDevice *, const QString );
   
public slots:
  //receive udp packet
  void OnMReceive();    
  
private:
  ClientView * pcv;     
  QSocketDevice * MUReceiveSocket;
  QSocketNotifier * MSocketNotifier;
};

#endif
