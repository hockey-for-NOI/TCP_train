#include "server.h"
#include <unp.h>

namespace	oi
{

ServerChild::ServerChild(std::mutex *global_mutex, std::condition_variable *cond_var, int *pipe):
	std::thread([this](){child_core();}),
	m_global_mutex(global_mutex),
	m_cond_var(cond_var),
	m_pipe(pipe),
	m_status(0);
{
}

void	ServerChild::child_core()
{
	while (true)
	{
		std::unique_lock<std::mutex> lck(global_mutex);
		cond_var.wait(lck);
		int pipe = *m_pipe;
		m_status.store(1);
		lck.unlock();

		try
		{
			int tmp = read(pipe, m_buf, 1);
			if (tmp == -1) throw std::runtime_error("Read Error.");
			unsigned opc;
			memcpy(&opc, m_buf, sizeof(unsigned));
			if (opc != Protocol::READ_MAGIC && opc != Protocol::WRITE_MAGIC)
				throw std::runtime_error("OPCode Error.");
			
			tmp = read(pipe, m_buf, 1);
			if (tmp == -1) throw std::runtime_error("Read Error.");
			unsigned namelen;
			memcpy(&namelen, m_buf, sizeof(unsigned));
			if (namelen > MAX_NAME_LEN) throw std::runtime_error("Filename Too Long.");
			
			tmp = read(pipe, m_buf, namelen);
			if (tmp == -1) throw std::runtime_error("Read Error.");
			m_buf[namelen] = 0;

			if (opc == Protocol::READ_MAGIC)
			{
			}
			else
			{
			}
		}
		catch	(std::exception &e)
		{
		}

		m_status.store(0);
	}
}

}	// end namespace oi
