#pragma once

#include "protocol.h"
#include "server_storage.h"
#include "server_scheduler.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socked.h>

#include <stdexcept>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>

namespace	oi
{

class	ServerChild
{
public:
	static	const	int	BUF_SIZE = 8192;
	static	const	int	MAX_NAME_LEN = 100;
	static	const	int	MAX_FILE_LEN = 1048576;
	static	const	std::string	storage_path;
	ServerChild() = delete;
	ServerChild(int m_id, ServerScheduler *scheduler,
			std::mutex *global_mutex, std::condition_variable *cond_var, int *pipe);
private:
	int	m_id;
	ServerScheduler	*m_scheduler;
	std::mutex	*m_global_mutex;
	std::condition_variable	*m_cond_var;
	int	*m_pipe;
	std::thread	m_thread;

	char	m_buf[BUF_SIZE];

	void	child_core();
};

class	Server
{
public:
	static	const	int	CHILD_NUM = 16;
	Server();
private:
	std::vector <ServerChild>	m_child;
	std::vector <int>	m_pipe;
	std::vector <std::condition_variable>	m_cond_var;
	std::mutex	m_mutex;
	ServerScheduler	m_scheduler;
};

}	// end namespace oi
