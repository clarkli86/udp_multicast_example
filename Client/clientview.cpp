/***************************************************************************
                          clientview.cpp  -  description
                             -------------------
    begin                : ?  1? 1 2006
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

#include "clientview.h"
#include <qpainter.h>           
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <unistd.h>
#include "client.h"
#include "sensor.h"

ClientView::ClientView(QWidget *parent, const char *name ) : QScrollView(parent,name)  {
   bFullScreen = false;
   bShowMenu = true;
   setCaption( "Lan Screen Capture" );     
   //set the timer
   pTimer = new QTimer( this );
   connect( pTimer, SIGNAL( timeout() ), this, SLOT( timeout() ) );
   pTimer->start( 500 );
}


ClientView::~ClientView(){
       delete pTimer;	
}


void ClientView::start() {
  pClient = new Client( this );
   pSensor = new Sensor( this );
   //initial the menubar
   pMenuBar = new QMenuBar( this );

   pMenu = new QPopupMenu( this );

   pServerList = new QPopupMenu( this );
   //register the signal
   connect( pServerList, SIGNAL( activated(int) ), this, SLOT( changeServer( int ) ) );

   //insert into the option
   pMenu->insertItem( "&Refresh server list", this, SLOT( refreshMenu() ) );
   pMenu->insertItem( "&Select a server" , pServerList );
   pMenu->insertItem( "Show&Menu", this, SLOT( showMenu() ) );
   pMenu->setItemChecked( pMenu->idAt( 2 ), bShowMenu );
   pMenu->insertItem( "&Quit", this, SLOT( quit() ) );

   pMenuBar->insertItem( "&Option", pMenu );

   QPopupMenu * pAbout = new QPopupMenu( this );
   pAbout->insertItem( "&About Lan Screen Capture", this, SLOT( showAbout() ) );
   pMenuBar->insertItem( "&Help", pAbout );
}

void ClientView::drawContents( QPainter * p, int clipx, int clipy, int clipw, int cliph ) {   
  if( true == bFullScreen )
  //hide the scroll bar
    resizeContents( 0, 0 );
  else
    if( contentsWidth() != map.width() || contentsHeight() != map.height() )
        resizeContents( map.width(), map.height() );  

  p->drawPixmap( clipx, clipy, map, clipx, clipy, clipw, cliph );
  
}        

void ClientView::contentsMouseDoubleClickEvent( QMouseEvent * e ) {
    if( LeftButton != e->button() )
      return;
      
    if( false == bFullScreen ) {  
      showFullScreen();
      pMenuBar->hide();
      bFullScreen = true;
    }
    else {
      showNormal();
      pMenuBar->show();
      bFullScreen = false;
    }
}       

void ClientView::contentsMouseReleaseEvent ( QMouseEvent * e ) {
  if( RightButton != e->button() )
    return;                
 
  pMenu->popup( e->globalPos() );
}

void ClientView::redrawPixmap( const int width, const int height, const int top, const QPixmap& newMap ) {
  //resize the map
    if( map.width() != width || map.height() != height )
      map.resize( width, height );
  //bitBit the new Map to the specified to the map   
    bitBlt( &map, 0, top, &newMap, 0, 0, newMap.width(), newMap.height(), CopyROP );
//    updateContents();
}

//it's called after every 0.5s
void ClientView::timeout() {
	//redraw the content
	updateContents();
}

void ClientView::quit() {
  //tell the qapplication to quit
  emit quitSignal();
}

void ClientView::changeServer( int nId ) {
  //uncheck all the items    
  int index = 0;
  int id = -1;
  while( -1 != ( id = pServerList->idAt( index ) ) ) {    
    pServerList->setItemChecked( id, false );
    index ++;
  }
     
  //check or uncheck the selected item
  pServerList->setItemChecked( nId, true );

  QString address = pServerList->text( nId );
  
  if( ! pClient->bind( address ) ) {
    QMessageBox::warning( this, "Error", "Can't bind to the address " + address );
    pServerList->setItemChecked( nId, false );
  } 
}

void ClientView::showMenu() {
  bShowMenu = ! bShowMenu;
  pMenu->setItemChecked( pMenu->idAt( 2 ), bShowMenu );

  if( bShowMenu == true )
    pMenuBar->show();
  else
    pMenuBar->hide();
}

void ClientView::refreshMenu() {  
  pSensor->refresh(); 
}

void ClientView::showAbout() {
  QMessageBox::about( this, "About Lan Screen Capture", "Lan Screen Capture Release 1.0 for Linux Red Hat\nProgram:\n\tHongLiang Li\n\tQian Zhu\n\tQi Xie\nSpecial thanks for Robert Luo\nBug Report: thenshesaid2003@yahoo.com.cn" );
}

QPopupMenu* ClientView::getServerList() {
	return pServerList;
}
