#include "clienthandler.h"

//Client handling
clienthandler::clienthandler() { start(); }

clienthandler::~clienthandler() { stop(); }

void clienthandler::start()
{

}

void clienthandler::stop()
{

}


//Order checker

orderchecker::orderchecker()
{
	start();
};
orderchecker::~orderchecker()
{
	stop();
};
int orderchecker::start()
{
    manager db;
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(-1);
        return 1;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_ORDER_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    //

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            for (size_t i = 127; i > 0; i--)
                if ((int)recvbuf[i] == -52) recvbuf[i] = 0;

            printf("[~] Order received: %s\n", recvbuf);
            char* c = recvbuf;
            std::string s = c;
            std::vector<std::string> orderinfo = db.explode(s, '|');

            int al = 0;
            if (orderinfo[1] == "luRxb1O")
                al = 4;
            else if (orderinfo[1] == "DgwYwTA")
                al = 2;
            else if (orderinfo[1] == "FpXpTjP")
                al = 3;
            else if (orderinfo[1] == "pa0OUmb")
                al = 1;

            int i = 0;
            if (orderinfo[3] != "null")
                i = 1;

            Order _o = {
                orderinfo[0],
                orderinfo[2],
                al,
                i
            };

            if (!db.doesOrderExist(_o.order_id))
                db.addOrder(_o);
        }
        else if (iResult == 0) {
            printf("Connection closing...\n");
            exit(-1);
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);
    return 1;
}
void orderchecker::stop()
{

}

accountchecker::accountchecker() {

}

accountchecker::~accountchecker() {

}

void accountchecker::start() {
    while (true)
    {
        manager db;
        std::vector<std::string> v(db.explode(db.getAllUids(), '\n'));
        for (auto a : v)
        {
            int id = stoi(a);

            std::string timeindb = db.getSubEndDate(id);
            if (timeindb == "\n")
                continue;

            timeindb = db.explode(timeindb, ' ')[0];

            struct tm tm;

            time_t start;
            int yy, month, dd, hh, mm, ss;
            const char* zstart = timeindb.c_str();

            sscanf(zstart, "%d-%d-%d", &yy, &month, &dd);


            tm.tm_year = yy - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = dd;
            tm.tm_isdst = -1;

            time_t now = time(0);

            start = mktime(&tm);

            if (start < now)
            {
                 db.expiredUser(id);
            }

            //printf("\n%o: %o/%o/%o\n", id, tm.tm_year, tm.tm_mon, tm.tm_mday);
            //printf("\n%o: %o/%o/%o\n", id, yy, month, dd);

            //scanf("%d/%d/%d", &d, &m, &y);
        }

        SleepEx(600000, false);
    }

}

void accountchecker::stop() {

}