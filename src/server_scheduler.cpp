#include "server_scheduler.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace	oi
{

ServerScheduler::ServerScheduler(int n)
{
	m_queue.resize(n);
	for (int i=0; i<n; i++) m_queue[i] = {i - 1, i + 1};
	m_queue[n - 1].second = -1;
	m_front_idle = 0; m_back_idle = n - 1;
	m_front_daze = -1; m_back_daze = -1;
}

void	ServerScheduler::push(int id, int fd)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_fd[id] = fd;

	if (m_queue[id].first == -1) m_front_idle = m_queue[id].second;
	else m_queue[m_queue[id].first].second = m_queue[id].second;
	
	if (m_queue[id].second == -1) m_back_idle = m_queue[id].first;
	else m_queue[m_queue[id].second].first = m_queue[id].first;

	if (m_front_daze == -1)
	{
		m_front_daze = m_back_daze = id;
		m_queue[id].first = m_queue[id].second = -1;
	}
	else
	{
		m_queue[m_back_daze].second = id;
		m_queue[id].first = m_back_daze;
		m_back_daze = id;
		m_queue[id].second = -1;
	}
}

void	ServerScheduler::remove(int id)
{
	std::unique_lock<std::mutex> lck(m_mutex);

	if (m_queue[id].first == -1) m_front_daze = m_queue[id].second;
	else m_queue[m_queue[id].first].second = m_queue[id].second;
	
	if (m_queue[id].second == -1) m_back_daze = m_queue[id].first;
	else m_queue[m_queue[id].second].first = m_queue[id].first;

	if (m_front_idle == -1)
	{
		m_front_idle = m_back_idle = id;
		m_queue[id].first = m_queue[id].second = -1;
	}
	else
	{
		m_queue[m_back_idle].second = id;
		m_queue[id].first = m_back_idle;
		m_back_idle = id;
		m_queue[id].second = -1;
	}
}

void	ServerScheduler::flush(int id)
{
	std::unique_lock<std::mutex> lck(m_mutex);

	if (m_queue[id].first == -1) m_front_daze = m_queue[id].second;
	else m_queue[m_queue[id].first].second = m_queue[id].second;
	
	if (m_queue[id].second == -1) m_back_daze = m_queue[id].first;
	else m_queue[m_queue[id].second].first = m_queue[id].first;

	if (m_front_daze == -1)
	{
		m_front_daze = m_back_daze = id;
		m_queue[id].first = m_queue[id].second = -1;
	}
	else
	{
		m_queue[m_back_daze].second = id;
		m_queue[id].first = m_back_daze;
		m_back_daze = id;
		m_queue[id].second = -1;
	}
}

int	ServerScheduler::get()
{
	std::unique_lock<std::mutex> lck(m_mutex);

	if (m_front_idle != -1) return m_front_idle;

	shutdown(m_fd[m_front_daze], SHUT_RDWR);

	return m_front_daze;
}

}	// end namespace oi
