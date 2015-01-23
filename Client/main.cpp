/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sun Jan  1 14:08:46 CST 2006
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

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qpushbutton.h>    
#include "clientview.h"

int main(int argc, char *argv[])
{      
  QApplication a(argc, argv);

  ClientView cv( NULL, "Lan Screen Capture" );

  QObject::connect( &cv, SIGNAL( quitSignal() ), &a, SLOT( quit() ) );

  cv.start();
  cv.resize( 400, 400 );
  a.setMainWidget( &cv );
  cv.show();  
  
  return a.exec();
}


