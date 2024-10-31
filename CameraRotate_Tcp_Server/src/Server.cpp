#include "sessionclient.h"
#include "server.h"
#include "camera_driver.hpp"

#include <iostream>




/* ------------------------------------------------------- */
/* ------------------------ Server ----------------------- */
/* ------------------------------------------------------- */

Server *Server::Run(int port)
{
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
		return 0;

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int is_ok = bind(sd, (sockaddr *)&addr, sizeof(addr));
	if (is_ok == -1)
		return 0;

	is_ok = listen(sd, listen_queue_len);
	if (is_ok == -1)
		return 0;

	return new Server(addr, sd);
}

SessionClient *Server::AcceptSession()
{
	sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int sd = accept(GetFd(), (sockaddr *)&addr, &len);
    SessionClient *session = new SessionClient(addr, sd);
	return session;
}

/* ------------------------------------------------------- */
/* -------------- main and thread function --------------- */
/* ------------------------------------------------------- */

void *handle_session(void *session)
{
    SessionClient *sess = reinterpret_cast<SessionClient*>(session);
	while (true) {		        
        bool ok = sess->EnterInteractiveMode();
        //if(map_sess.)map_sess.insert(std::map<int, SessionClient*>::value_type(sess->GetFd(),sess));

        std::cout<<"Server receive: ";
		if (!ok) {
			delete sess;
			pthread_exit(0);
			return 0;
		}
	}
	return sess;
}

