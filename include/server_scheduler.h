#pragma once

#include <mutex>
#include <vector>
#include <utility>

namespace	oi
{

class	ServerScheduler
{
public:
	ServerScheduler(int n);
	void	push(int id, int fd);
	void	remove(int id);
	void	flush(int id);
	int	get();
public:
	std::vector < std::pair<int, int> >	m_queue;
	std::vector <int> m_fd;
	int	m_front_idle, m_back_idle, m_front_daze, m_back_daze;
	std::mutex	m_mutex;
};

}	// end namespace oi
