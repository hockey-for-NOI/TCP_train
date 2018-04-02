#pragma once

#include "protocol.h"

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

class	ServerChild: public std::thread
{
public:
	static	const	int	BUF_SIZE = 8192;
	static	const	int	MAX_NAME_LEN = 100;
	static	const	int	MAX_FILE_LEN = 1048576;
	static	const	std::string	storage_path;
	ServerChild() = delete;
	ServerChild(std::mutex *global_mutex, std::condition_variable *cond_var, int *pipe);
	inline	int	get_status() {return m_status.load();}
private:
	std::mutex*	m_global_mutex;
	std::condition_variable	*m_cond_var;
	int	*m_pipe;

	char	m_buf[BUF_SIZE];

	void	child_core();
};

}	// end namespace oi
