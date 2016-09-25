#include "server.h"

Server::Server(int port) {
    // setup variables
    port_ = port;
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    serve();
}

void
Server::create() {
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

    // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }
}

void
Server::close_socket() {
    close(server_);
}

void
Server::serve() {

    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);
      // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
        handle(client);
    }
    close_socket();
}

void
Server::handle(int client) 
{
    // loop to handle all requests
    while (1) 
    {
        cout << "TEST 0" << endl;
        // get a request
        string request = get_request(client);
        // break if client is done or an error occurred
        if (request.empty())
          break;
            //break;
        // parse request

        //Checking the command
        cout << "TEST 1" << endl;
        string command;
        string user;
        string subject;
        string index;
        string extra;
        int length = NULL;
        string cache;
        stringstream ss;

        //spliting up the received input
        string delimiter = "\n";
        string firstCommand = request.substr(0, request.find(" "));
        string fullCommand = request.substr(0, request.find(delimiter));
        cout << "TEST 2" << endl;
        Message message = Message();
        cout << endl << endl << "REQUEST ***************************" << endl << request << endl;
        //*********************** STORING MESSAGES
        if(firstCommand == "put")
        {
            cache = request.substr(request.find(delimiter) + 1, request.length()-1);
            message.setCache(cache);
            cout << "TEST 3" << endl;


            ss << fullCommand;
            ss >> command >> user >> subject >> length;

            
            cout << "PUT -------" << endl;
            cout << "command: " << command << endl;
            cout << "user: " << user << endl;
            cout << "sub: " << subject << endl;
            cout << "length: " << length << endl;


            cout << "TEST 4" << endl;

            if(subject.empty())
            {
                cout << "ERROR ERROR ERROR1" << endl;
                bool success = send_response(client, "error no subject\n");
            }
            else if(length == NULL)
            {
                cout << "ERROR ERROR ERROR2" << endl;
                bool success = send_response(client, "error command\n");
            }
            else
            {
                message.setCommand(command);
                message.setUser(user);
                message.setSubject(subject);
                message.setLength(length);

                //if message is too long then get the rest of the message
                
                
                cout << "CACHE ------ " << message.getCache().size() << endl;
                cout << "length: " << message.getLength() << endl;
                if (message.getCache().size() < message.getLength())
                {
                    cout << "TEST 6" << endl;
                    get_value(client,message);
                }
                //do something
                // bool success = handle_message(client,message, message.getCommand());
                // // break if an error occurred
                // if (not success)
                // {
                //     break;
                // }
                cout << "TEST 7" << endl;
                map<string, vector<Message> > ::iterator it;
                it = myMap.find(message.getUser());

                if(it!= myMap.end())
                {
                    cout << "TEST 8" << endl;
                    it->second.push_back(message);
                    bool success = send_response(client, "OK\n");
                    if (!success)
                    {
                        break;
                    }
                }
                else
                {
                    cout << "TEST 9" << endl;
                    vector<Message> v;
                    v.push_back(message);
                    cout << "TEST 9.1" << endl;
                    myMap.insert(pair<string,vector<Message> >(message.getUser(), v));
                    cout << "TEST 9.2" << endl;
                    bool success = send_response(client, "OK\n");
                    if (!success)
                    {
                        cout << "TEST 9.3" << endl;
                        break;
                    }
                    cout << "TEST 9.4" << endl;
                }
                cout << "TEST 10" << endl;
            } 
        }
        else if(firstCommand == "list")
        {
            ss << fullCommand;
            ss >> command >> user;

            cout << "LIST -------" << endl;
            cout << "command: " << command << endl;
            cout << "user: " << user << endl;


            if(user.empty())
            {
                bool success = send_response(client, "error invalid input\n");
            }
            // if(!extra.empty())
            // {
            //     subject = subject + " " + to_string(length);
            //     length = atoi(extra.c_str());
            // }

            message.setCommand(command);
            message.setUser(user);


            map<string, vector<Message> > ::iterator it;
            it = myMap.find(message.getUser());
            vector<Message> myVect = it->second;
    

            string totalString = "list " + to_string(myVect.size()) + "\n";

            for(int i = 0; i < myVect.size(); i++)
            {
                totalString += to_string(i + 1) + " " + myVect[i].getSubject() + "\n";
            }

            cout << "TOTAL STRING FOR LIST IS" << endl << totalString << endl;
            bool success = send_response(client, totalString);
            if (!success)
            {
                cout << "ERROR ERROR 2" << endl;
                send_response(client, "error");
            }
        }
        else if(firstCommand == "get")
        {
            cout << "GETTER" << endl;
            ss << request;
            ss >> command >> user >> index;

            message.setCommand(command);
            message.setUser(user);
            message.setIndex(atoi(index.c_str()));
            cout << "GETTER1" << endl;
            map<string, vector<Message> > ::iterator it;
            it = myMap.find(message.getUser());
            vector<Message> myVect;
        
            if(it != myMap.end())
            {
                 cout << "GETTER2" << endl;
                 myVect = it->second;

                 Message letter = myVect[message.getIndex() - 1];
                 string sub = letter.getSubject();
                 string words = letter.getCache();
                 string wordLength = to_string(letter.getLength());
                 cout << "GETTER4" << endl;
                 // cout << "sub: " << sub << endl;
                 // cout << "length: " << wordLength << endl;
                 // cout << "words: " << words << endl;

                 string totalString = "message " + sub + " " + wordLength + "\n" + words;

                 //cout << "TOTAL STRING IS:" << totalString;
                 bool success = send_response(client, totalString);
                 cout << "TOTAL STRING IS ()()()()()" << totalString << endl;
                 if (!success)
                 {
                     cout << "ERROR ERROR 1" << endl;
                     send_response(client, "error invalid input\n");
                 }
            }
            else
            {
                cout << "GETTER3" << endl;
                send_response(client, "error invalid get\n");
            }
        }
        else if(firstCommand == "reset\n")
        {
            myMap.clear();
            bool success = send_response(client, "OK\n");
            if (!success)
            {
                send_response(client, "error invalid input\n");
            }
        }
        else if(firstCommand == "quit")
        {
            bool success = send_response(client, "quit");
            if (!success)
            {
                send_response(client, "error invalid input\n");
            }
        }
        else
        {
            cout << "ERROR ERROR" << endl;
            bool success = send_response(client, "error command\n");
        }

    }
    close(client);
}

//**************************************************

void
Server::get_value(int client, Message& message) 
{
    //cout << "MAYDAY1 !!!!!!!!!!!!!!!!!!!!!" << endl;
    string temp = "";
    while(message.getCache().size() < message.getLength())
    {
        //cout << "MAYDAY2" << endl;
        int nread = recv(client,buf_,1024,0);
        //cout << "MAYDAY3" << endl;
        if (nread < 0) 
        {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return;
        } 
        else if (nread == 0) 
        {
            // the socket is closed
            //cout << "socket is closed" << endl;
            return;
        }
        //cout << "MAYDAY5" << endl;
        temp = string(buf_,nread);
        message.setCache(message.getCache() + temp);
        //cout << "THE CACHE SIZE NOW:" << message.getCache().size() << endl;
        //cout << "THE LENGTH NOW:" << message.getLength() << endl;
        //cout << "0000000000000000000000000000" << endl;
        //cout << message.getCache() << endl;
    }
    message.setCache(message.getCache());
    //cout << "MAYDAY6" << endl;   
}

bool
Server::handle_message(int client, Message& message, string command) 
{
    if(command == "put")
    {
        string response = "OK\n";
        return send_response(client, response);
    }
    else if(command == "list")
    {
        string response = message.getListString();
        const char* ptr = response.c_str();
        return send(client, ptr, response.size(), 0);
    }
    return false;
  
}

bool
Server::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) 
    {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) 
        {
            if (errno == EINTR) 
            {
                // the socket call was interrupted -- try again
                continue;
            }
            else 
            {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } 
        else if (nwritten == 0) 
        {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

string
Server::get_request(int client) {
    string request = "";
    // read until we get a newline
    while (request.find("\n") == string::npos) {
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    return request;
}
