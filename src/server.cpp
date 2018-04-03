#include "server.h"
#include <signal.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <cstring>

namespace	oi
{

Server::Server(int num_child):
	m_pipe(num_child, -1),
	m_cond_var(num_child),
	m_scheduler(num_child)
{
	struct sigaction act;
	sigaction(SIGPIPE, 0, &act);
	act.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &act, 0);

	for (int i=0; i<num_child; i++)
		m_child.emplace_back(i, &m_scheduler, &m_mutex, &m_cond_var[i], &m_pipe[i]);
}

int	Server::run(int port)
{
	struct sockaddr_in servaddr;
	unsigned socklen = sizeof(servaddr);
	int listenfd;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) return -1;
	
	int optval = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(int)) < 0) return -1;
	if (setsockopt(listenfd, IPPROTO_TCP, TCP_NODELAY, (void*)&optval, sizeof(int)) < 0) return -1;

	int bufsize = ServerChild::BUF_SIZE << 2;
	if (setsockopt(listenfd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(int)) < 0) return -1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(int)) < 0) return -1;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, m_child.size() + 1);

	while (1)
	{
		int connfd = accept(listenfd, (struct sockaddr*)&servaddr, &socklen);
		if (connfd < 0) continue;

		int child_id = m_scheduler.get();
		m_pipe[child_id] = connfd;
		while (m_pipe[child_id] != -1)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_cond_var[child_id].notify_one();
		}
	}
}

}	// end namespace oi
