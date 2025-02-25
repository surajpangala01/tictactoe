#include "network/WebSocket.h"
#include "axmol.h"

class GameController;

class Online: public ax::network::WebSocket::Delegate
{
    GameController* controller;

public:
    Online(GameController*);
    char mark;
    ax::network::WebSocket* _ws;
    bool init();

    // void returnMove(int row,int col);

    void onOpen(ax::network::WebSocket* ws);
    virtual void onMessage(ax::network::WebSocket* ws, const ax::network::WebSocket::Data& data);
    virtual void onClose(ax::network::WebSocket* ws);
    //    virtual void onClose(ax::network::WebSocket* ws);
    virtual void onError(ax::network::WebSocket* ws, const ax::network::WebSocket::ErrorCode& error);
    void sendMove(int, int);
    ~Online();
    // void sendMove(int row,int col)
};