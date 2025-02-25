#include "Online.h"
#include "../Controller.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
// #include "WebSocketClass.h"
// #include "MultiPlayerScene.h"
#include<string>


Online::Online(GameController* obj)
{
    controller = obj;
}

bool Online::init()
{
    // Create and initialize the WebSocket
    _ws = new ax::network::WebSocket();
    if (!_ws->open(static_cast<ax::network::WebSocket::Delegate*>(this),
                   "wss://d2157b21-f882-4c88-9741-ac6503368c64-00-35p1srv82b1u3.pike.replit.dev/ws"))
    {
        AXLOG("Failed to initialize WebSocket");
        return false;

    }
//    else
//    {
//        AXLOG("Hello\n");
//    }
    return true;
}

void Online::onOpen(ax::network::WebSocket* ws)
{
    AXLOG("WebSocket connection opened");
    // ws->send("Hello Server!"); // Example: send a message
}

void Online::onMessage(ax::network::WebSocket* ws, const ax::network::WebSocket::Data& data)
{
    AXLOG("in onMessage");
    std::string stringifiedJSON(data.bytes, data.bytes + data.len);
    AXLOG("%s", stringifiedJSON.c_str());
    if (data.isBinary)
    {
        AXLOG("Received binary message of length: %d", (int)data.len);
        return;
    }

    rapidjson::Document document;
    document.Parse(stringifiedJSON.c_str());
    if (document.HasParseError())
    {
        AXLOG("error parsing JSON");
        return;
    }
    int type;
    if (document.HasMember("op") && document["op"].IsInt())
    {
        type = document["op"].GetInt();
    }
    else
    {
        AXLOG("Error in ws data sent");
    }
    //        MultiPlayerScene* MPScene;
    switch (type)
    {
    case 1:

        if (document.HasMember("data") && document["data"].IsString()){
            mark = document["data"].GetString()[0];
            controller -> mark = mark;
            controller -> boardLoad();
        }

        else
        {
            AXLOG("Some error in getting data in create ws packet");
            return;
        }

    case 2:
        if (document.HasMember("data") && document["data"].IsObject())
        {
            auto data = document["data"].GetObject();
            int row, col;
            if (data.HasMember("row") && data["row"].IsInt())
            {
                row = data["row"].GetInt();
            }
            else
            {
                AXLOG("No data ");
            }
            if (data.HasMember("col") && data["col"].IsInt())
            {
                col = data["col"].GetInt();
            }
            else
            {
                AXLOG("No data ");
                return;
            }
            AXLOG("%d , %d", row, col);
            controller->recieveMove(row,col);

        }
            break;
    case 3:
        if (document.HasMember("data") && document["data"].IsString()){
            auto val = document["data"].GetString();
            controller->loadEndGameScene(val[0]);

        }
        break;

    }
}

void Online::onClose(ax::network::WebSocket* ws)
{
    AXLOG("WebSocket connection closed");
    AX_SAFE_DELETE(_ws);
}

void Online::onError(ax::network::WebSocket* ws, const ax::network::WebSocket::ErrorCode& error)
{
    AXLOG("WebSocket error occurred : %d", error);
    controller->serverError();
}

Online::~Online()
{
    if (_ws)
    {
        _ws->close();
    }
}

 void Online::sendMove(int row,int col){
        rapidjson::Document document;
        document.SetObject();

        auto allocator = document.GetAllocator();

        document.AddMember("op",2,allocator);


        rapidjson::Value data(rapidjson::kObjectType);
        data.AddMember("row", row, allocator);
        data.AddMember("col", col, allocator);
        document.AddMember("data", data, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        AXLOG("%s",buffer.GetString());
        _ws->send(buffer.GetString());
    }
