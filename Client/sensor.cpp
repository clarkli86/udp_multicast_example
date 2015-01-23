/***************************************************************************
                          sensor.cpp  -  description
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

#include <qsocketdevice.h>
#include <qsocketnotifier.h>
#include <qhostaddress.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qtimer.h>
#include <unistd.h>
#include "clientview.h"
#include "sensor.h"

static const int PORT = 5000;
#define BROADCAST "224.0.0.1"

Sensor::Sensor( ClientView * pClientView ) : pcv( pClientView )  {
  //bind to the broadcast address  
  if( ! bind( BROADCAST ) ) {
    QMessageBox::warning( pcv, "Error", "can't bind to the broadcast address, the application will quit!" );
//    pcv->quit();
    exit( 0 );
  }

  pTimer = new QTimer( this );
  connect( pTimer, SIGNAL( timeout() ), this, SLOT( timerDone() ));
  pTimer->start( 5000 );
               
}

Sensor::~Sensor(){
  if( pTimer ) {
    pTimer->stop();
    delete pTimer;
  }
  if( MUReceiveSocket )
    delete MUReceiveSocket;
  if( MSocketNotifier )
    delete MSocketNotifier;    
}


void Sensor::OnMReceive() {          
 
  int ByteCount, ReadCount;

  ByteCount = MUReceiveSocket->bytesAvailable();

  //new a buffer   
  char* buf = new char[ ByteCount ];

  ReadCount = MUReceiveSocket->readBlock( buf, ByteCount );
  
  QString address( buf );    
  
  newServers.insert( address );
  
  delete [] buf;
}
/*
  *bind to the specified multicast address
*/
bool Sensor::bind( const QString FakeAddress ) {    
  if( MUReceiveSocket ) {
      delete MUReceiveSocket;
      MUReceiveSocket = NULL;
  }
  if( MSocketNotifier ) {
      delete MSocketNotifier;
      MSocketNotifier = NULL;
  }

  MUReceiveSocket = new QSocketDevice( QSocketDevice::Datagram );

  QHostAddress MyAddress;
  MyAddress.setAddress( FakeAddress );

  bool result =  MUReceiveSocket->bind( MyAddress, PORT );

  MSocketNotifier = new QSocketNotifier( MUReceiveSocket->socket(), QSocketNotifier::Read, 0, "MSocketNotifier" );

  QObject::connect( MSocketNotifier, SIGNAL( activated( int ) ), this, SLOT( OnMReceive() ) );

  return result;
}

void Sensor::timerDone() {   
  oldServers = newServers;
  newServers.clear();
}

void Sensor::refresh() {   
  QPopupMenu* pServerList = pcv->getServerList();
  QString preAddress;

  int nIndex = 0;
  int nId = -1;
  while( -1 != ( nId = pServerList->idAt( nIndex ) ) ) {
    if( true == pServerList->isItemChecked( nId ) ) {
      preAddress = pServerList->text( nId );
      break;
    }
    nIndex ++;
  }

  pServerList->clear();

  set< QString >::iterator first = oldServers.begin(), end = oldServers.end();
  nIndex = 0;
  for( ; first != end; first ++ ) {
    pServerList->insertItem( *first );
   
    if( preAddress == *first )
      pServerList->setItemChecked( pServerList->idAt( nIndex ), true );

    nIndex ++;
  }  
}
