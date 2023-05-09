#include "gamebasewindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication *application = new QApplication(argc, argv);

    GameBaseWindow *base = new GameBaseWindow(application);
    base->initComponments();
    base->run();

    return application->exec();
}
