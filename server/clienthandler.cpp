#include "clienthandler.h"

//Client handling
clienthandler::clienthandler() {  }

clienthandler::~clienthandler() { stop(); }

void clienthandler::start()
{
    SleepEx(250, false);
    klog out;
    out.out(0, "[clients] Starting SSL/TLS server with cert.pem and key.pem...");
    int sock;
    SSL_CTX* ctx;

    init_ssl();
    ctx = create_context();

    configure_context(ctx);

    sock = create_socket(51005);
    out.out(0, "[clients] Created custom socket on port 51005");

    /* Handle connections */
    while (1) {
        struct sockaddr_in addr; 
        int len = sizeof(addr);
        //SSL* ssl;
        strcpy(reply, "test\n");

        client = accept(sock, (struct sockaddr*)&addr, &len);

        if (client < 0) {

            wprintf(L"accept failed with error %u\n", WSAGetLastError());
            closesocket(client);
            WSACleanup();
            //return 1;
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }
        //std::string str;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);

        clientAddr = std::string(strdup(str));

        std::string s = "[clients] Client " + clientAddr +" connected";
        out.out(0, s);
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);
        clientlist.push_back(ssl);

        //launch new thread to handle connection
        std::thread nthread([this] { this->handleConnection(); });
        nthread.detach();

        SleepEx(250, false);
    }

    closesocket(sock);
    SSL_CTX_free(ctx);
    cleanup();
}

void clienthandler::stop()
{

}

void clienthandler::init_ssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void clienthandler::cleanup()
{
    EVP_cleanup();
}

void clienthandler::configure_context(SSL_CTX* ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "C:\\cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "C:\\key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

SSL_CTX* clienthandler::create_context()
{
    const SSL_METHOD* method;
    SSL_CTX* ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

std::string clienthandler::encryptDecrypt(std::string toEncrypt)
{

    char key[3] = { '#', '#', '#' }; //Any char will work | this is not the key we use
    std::string output = toEncrypt;

    for (int i = 0; i < toEncrypt.size(); i++)
        output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return output;
}

void clienthandler::sendpacket(SSL* _ssl, std::string request)
{
    SSL_write(_ssl , encryptDecrypt(request).c_str(), BuffSize);
}

int clienthandler::receivepacket(SSL* _ssl, char buf[BuffSize])
{
    int res = SSL_read(_ssl, buf, BuffSize);
    strcpy(buf, encryptDecrypt(buf).c_str());
    return res;
}

void clienthandler::handleConnection()
{
    std::string _clientAddr = clientAddr;
    SSL* _ssl = ssl;
    int _client = client;
    try {
        
        if (SSL_accept(_ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        
        for (;;)
        {
            char buf[1024];
            //int result = SSL_read(_ssl, buf, 1024);
            int result = receivepacket(_ssl, buf);

            std::string sr(buf);
            if (result == -1)
            {
                out.out(0, "[clients] [" + _clientAddr + "] closed connection.");
                SleepEx(1, false);
                return;
                //continue;
            }

            std::string sout = "[clients] [" + _clientAddr + "] Received: " + sr;
            out.out(0, sout);
            //F = first connect
            //L = logging in
            //U = client ping
            //printf("\n%c\n", buf[0]);
            if (buf[0] == 'F') //first connect will send simple 'F-*hwid*'
            {
                std::string hwid = db.explode(sr, '-')[1];
                std::string s = "INSERT INTO connectionlog (addrinfo, hwidinfo) VALUES ('" + _clientAddr + "', '" + hwid + "');";
                db.ExecuteNonQuery(s);

                char* _reply = "F-ok";
                std::string ts(_reply);
                sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                out.out(0, sout);
                sendpacket(_ssl, _reply);

                //wait for login

            }
            else if (buf[0] == 'L')
            {
                std::string retStr = "";
                //check user and update hwid if needed then start -U loop
                std::string uname = db.explode(sr, '-')[1];
                std::string pwd = db.explode(sr, '-')[2];
                std::string hwid = db.explode(sr, '-')[3];
                std::string ver = db.explode(sr, '-')[4];
                //check if username and password exist together
                if (uname.find('\'') != std::string::npos || uname.find('\"') != std::string::npos || uname.find('\\') != std::string::npos || uname.find('/') != std::string::npos
                    || uname.find('(') != std::string::npos || uname.find(')') != std::string::npos || uname.find('=') != std::string::npos)
                {
                    char* _reply = "L-Error-USERNAME-BAD";
                    std::string ts(_reply);
                    sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                    out.out(0, sout);
                    sendpacket(_ssl, _reply);
                    continue;
                }

                if (db.doesUsernameExist(uname)) {
                    if (db.checkPassword(uname, pwd))
                    {
                        if (db.isAccountLocked(uname))
                        {
                            std::string ts = "L-Error-Locked-" + db.getLockedReason(uname);
                            sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                            out.out(0, sout);
                            sendpacket(_ssl, ts.c_str());
                            continue;
                        }
                        std::string cHwid = db.getHwid(std::to_string(db.getUid(uname)));
                        if (cHwid == "not_set")
                        {
                            db.setHwid(db.getUid(uname), hwid);
                            cHwid = db.getHwid(std::to_string(db.getUid(uname)));
                        }

                        if (cHwid != hwid)
                        {
                            char* _reply = "L-Error-Wrong-HWID";
                            std::string ts(_reply);
                            sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                            out.out(0, sout);
                            sendpacket(_ssl, _reply);

                            time_t now = time(0);
                            struct tm* ntm = gmtime(&now);
                            std::string _time = std::to_string(ntm->tm_year + 1900) + "-" + std::to_string(ntm->tm_mon + 1) + "-" + std::to_string(ntm->tm_mday) + "-" + std::to_string(ntm->tm_hour) + "-" + std::to_string(ntm->tm_min) + "-" + std::to_string(ntm->tm_sec);

                            db.lockAccount(db.getUid(uname), "User tried accessing software with incorrect HWID! { Old: " + cHwid + ", New: " + hwid + " } @ " + _time);
                            continue;
                        }
                        ver = ver.substr(0, ver.length() - 1);
                        if (ver != std::string(CurrentClientVersion))
                        {
                            char* _reply = "L-Error-Wrong-ver";
                            std::string ts(_reply);
                            sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                            out.out(0, sout);
                            sendpacket(_ssl, _reply);
                            continue;
                        }

                        char* _reply = "L-ok";
                        std::string ts(_reply);
                        sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                        out.out(0, sout);
                        sendpacket(_ssl, _reply);

                        //out.out(0, "[clients] connected users: " + std::to_string(ch.clientlist.size()));
                        for (;; SleepEx(600000, false))
                        {

                        }
                    }
                    else
                    {
                        char* _reply = "L-Error-Wrong-PWD";
                        std::string ts(_reply);
                        sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                        out.out(0, sout);
                        sendpacket(_ssl, _reply);
                        continue;
                    }
                }
                else {
                    char* _reply = "L-Error-USERNAME-BAD";
                    std::string ts(_reply);
                    sout = "[clients] [" + _clientAddr + "] Sent: " + ts;
                    out.out(0, sout);
                    sendpacket(_ssl, _reply);
                    continue;
                }
            }
        }
    }
    catch (...)
    {
        out.out(2, "[clients] there was an error with one of the clients, removing them from currently connected!");
        for (size_t i = 0; i < clientlist.size(); i++)
        {
            if (clientlist[i] == _ssl)
                clientlist.erase(clientlist.begin() + i);
        }
        SSL_shutdown(_ssl);
        SSL_free(_ssl);
        closesocket(_client);
    }
    //SSL_shutdown(_ssl);
    //SSL_free(_ssl);
    //closesocket(_client);
    //strcpy(_reply, reply);
    //example write:
    //  (_ssl, _reply, strlen(reply));
    

    // std::terminate();
}

int clienthandler::create_socket(int port)
{
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    SleepEx(250, false);
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }
    SleepEx(250, false);

    if (listen(s, 1) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return s;
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
    klog out;
    manager db;
    WSADATA wsaData;
    int iResult;
    out.out(0, "[orders] Setting up order server...");

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

    out.out(0, "[orders] Server ready!");

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    out.out(0, "[orders] Client connected!");

    // No longer need server socket
    closesocket(ListenSocket);


    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            for (size_t i = 127; i > 0; i--)
                if ((int)recvbuf[i] == -52) recvbuf[i] = 0;

            char* c = recvbuf;
            std::string s = c;
            out.out(0, "[orders] Recieved order > " + s);
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
            out.out(0, "[orders] Connection closing. :" + WSAGetLastError());
            exit(-1);
        }
        else {
            out.out(0, "[orders] recv failed with error :" + WSAGetLastError());
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




//Account checking
accountchecker::accountchecker() {

}

accountchecker::~accountchecker() {

}

void accountchecker::start() {
    while (true)
    {
        manager db;
        klog out;
        std::vector<std::string> v(db.explode(db.getAllUids(), '\n'));
        out.out(0, "[accounts] Checking for outdated or broken accounts!");
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
                 out.out(0, "[accounts] User id " + std::to_string(id) + "'s subscription has expired!");
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