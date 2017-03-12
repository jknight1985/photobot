#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QWebSocketServer>

//hier wird QWebSocket bereits deklariert,
//damit unten der Pointer verwendet werden kann
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, QObject *parent = Q_NULLPTR);
    ~WebSocketServer();

signals:
    // Hier kann man eigene Signale anlegen und in Funktionen ueber emit aufrufen
    // Z.B. irgendeineFuncDieserKlasse() { emit(closed()); } oder emit closed();
    // Mehr Infos zu signals und slots in websocketserver.cpp
    void closed();

private slots:
    // Hier werden die Slots angelegt, in diesem Fall als private slots.
    // Slots sind gleichzeitig ganz normale Funktionen
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

public slots:
    // Hier die slots, die public sind.

private:
    QWebSocketServer m_pWebSocketServer;

    // QList ist eine sortierte Liste
    // Sie enthält Objekte oder Pointer, die in den Klammern angegeben sind
    // Hier sind es QWebSocket-Pointer
    //
    // Objekten muessen bestimmte Methoden besitzen (copy-constructor, = operator,...)
    // Viele Objekte von Qt und einfache structs besitzen diese meistens schon.
    // Pointer koennen hingegen ohne Einschraenkungen in Listen gespeichert werden
    // Daher werden in diesem Fall Pointer verwendet
    QList<QWebSocket *> m_clients;
};

#endif // WEBSOCKETSERVER_H
