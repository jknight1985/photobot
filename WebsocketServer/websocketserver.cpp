#include "websocketserver.h"
#include <QWebSocket>
#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent) :
    //Hier werden die Member-Objekte noch bevor der Constructor aufgerufen wird initialisiert
    //WebSocketServer ist abgeleitet vom QObject -->  WebSocketServer: QObject(parent)
    //nachfolgend werden die Parameter fuer das QObject uebergeben
    QObject(parent),
    //nachfolgend werden die Parameter fuer den m_pWebSocketServer uebergeben
    m_pWebSocketServer(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode, this)
{

    // Der m_pWebSocketServer wartet mit dem Befehl auf die websocket-clients
    if (m_pWebSocketServer.listen(QHostAddress::Any, port)) {

        qDebug() << "Websocketserver listening on port" << port;


        /* Signale werden mit Slots ueber den Befehl connect verbunden
         * D.h. wenn ein Signal ausgeloest wird, wird der verbundene slot aufgerufen
         * Qt uebernimmt dabei das Event-Handling
         *
         * Ein sehr maechtiges Feature in Qt
         *
         * connect(Pointer zu ObjektA,
         *          SIGNAL( Signal von ObjektA),
         *          Pointer zu ObjektB,
         *          SLOT( Slot von ObjektB));
         *
         * In dieser Variante wird erst waehrend der Programmlaufzeit erkannt ob signale oder slots nicht vorhanden sind.
         *
         *
        */

        connect(&m_pWebSocketServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        connect(&m_pWebSocketServer, SIGNAL(closed()), this, SIGNAL(closed()));

        /*
         *
         * Alternative Variante:
         * hier wird bereits beim Kompilieren geprueft, ob signale oder slots vorhanden sind
         *
         *
        connect(&m_pWebSocketServer, SIGNAL(newConnection()), this, &WebSocketServer::onNewConnection);
        connect(&m_pWebSocketServer, SIGNAL(closed()), this, &WebSocketServer::closed);
        */
    }
}

WebSocketServer::~WebSocketServer()
{
    // Destructor
    // Wird vor dem Loeschen der Funktion aufgerufen
    //
    m_pWebSocketServer.close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebSocketServer::onNewConnection()
{

    QWebSocket *pSocket = m_pWebSocketServer.nextPendingConnection();

    // siehe oben
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    // pSocket wird in die Liste m_clients hinzugefuegt
    // alternative m_clients.append(pSocket)
    m_clients << pSocket;

    qDebug() << "Connected to client with Name:" << pSocket->peerName() << "Ip:" << pSocket->peerAddress();
}

void WebSocketServer::processTextMessage(QString message)
{
    // message ist ein Objekt des Typs QString
    // QString eine der meistbenutzten Klassen in Qt
    // Kann alles im Umgang mit Zeichenketten machen
    // Stueckeln, Umwandeln, Ausgabe von Zahlen in unterschiedlichen Formaten...
    qDebug() << "Message received:" << message;


    // hier wird der sender() (ein QObject-Pointer) in einen Pointer des Typs QWebSocket umgewandelt.
    // Sichere Casting Variante, da geprueft wird, ob ein Cast moeglich ist
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    //Wichtige Pruefung, da nicht immer eine Umwandlung moeglich sein muss
    if (pClient) {
        pClient->sendTextMessage(message);
    }
}

void WebSocketServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
