#include <qapplication.h>
#include <qevent.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "serverframe.h"

#define LOCKFILE "/tmp/sashiwuli"

void
termHandler(int signo) {
	unlink(LOCKFILE);
	exit(1);
}

int
main(int argc, char *argv[]) {
	QApplication    app(argc, argv);
	int		fd = open(LOCKFILE, O_CREAT | O_EXCL);
	
	if (fd > 0) {
		ServerFrame	*server = new ServerFrame();
		server -> getUniqueFileName(LOCKFILE);
		
		signal(SIGINT, termHandler);
		signal(SIGTERM, termHandler);
		signal(SIGKILL, termHandler);
		
		app.setMainWidget(server);
		server -> show();
		return app.exec();
	} else
		exit(1);
}
