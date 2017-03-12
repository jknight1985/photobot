#include <QCoreApplication>
#include "websocketserver.h"

int main(int argc, char *argv[])
{
    // Erstellen eines Qt-Applikationsobjekts
    // Ist zwingend erforderlich fuer viele Qt Funktionen (z.B. signals/slots)
    // QCoreApplication entspricht der Basisapplikation fue Konsolen-Programme
    QCoreApplication a(argc, argv);

    const int port = 9999;
    WebSocketServer ws(port);

    return a.exec();
}
