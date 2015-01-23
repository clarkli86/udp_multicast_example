/***************************************************************************
                          sensor.h  -  description
                             -------------------
    begin                : ?  1? 2 2006
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

#ifndef SENSOR_H
#define SENSOR_H


#include <qobject.h>
#include <set>
using namespace std;

/**
  *@author fuckall
  */
class QSocketDevice;
class QSocketNotifier;
class ClientView;
class QTimer;

class Sensor : public QObject  {
   Q_OBJECT    
public: 
	Sensor( ClientView * pCientView );
	~Sensor();
  void refresh();
private:
  //bind the multicast address
  bool bind( const QString );
  
public slots:
  //receive udp packet
  void OnMReceive();
private slots:
  //time out
  void timerDone();

private:
  QSocketDevice * MUReceiveSocket;
  QSocketNotifier * MSocketNotifier;
  ClientView * pcv;
  QTimer * pTimer;
  set< QString > newServers;
  set< QString > oldServers;
};

#endif
