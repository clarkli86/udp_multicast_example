/***************************************************************************
                          clientview.h  -  description
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

#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <qwidget.h>
#include <qscrollview.h>
#include <qpixmap.h>
/**
  *@author fuckall
  This is the main widget of the client
  */
class QMenuBar;
class QPopupMenu;
class Client;
class Sensor;
class QTimer;
class ClientView : public QScrollView  {
   Q_OBJECT
public: 
	ClientView(QWidget *parent=0, const char *name=0);
	~ClientView();

  void redrawPixmap( const int width, const int height, const int top, const QPixmap& newMap );  
  void start();
  QPopupMenu* getServerList();
	
protected:
  void contentsMouseDoubleClickEvent( QMouseEvent * e );
  void contentsMouseReleaseEvent( QMouseEvent * e );
  void drawContents( QPainter * p, int clipx, int clipy, int clipw, int cliph );

private slots:
	void timeout();
public slots:
 //menu slots
  void quit();
  void changeServer( int );
  void showMenu();
  void refreshMenu();
  void showAbout();
  
  signals:
  void quitSignal(); 
  
public:
  QPixmap map;
private:
  //if the widget is fullscreen
  bool bFullScreen;
  //if the menu is visible
  bool bShowMenu;
  QMenuBar * pMenuBar;
  QPopupMenu * pServerList;
  QPopupMenu * pMenu;
  Client * pClient;
  Sensor * pSensor;
  QTimer * pTimer;
};

#endif
