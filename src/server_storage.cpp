#include "server.h"

namespace	oi
{

ServerStorage::m_instance = 0;

ServerStorage*	ServerStorage::get_instance()
{
	if (!m_instance) m_instance = new ServerStorage();
	return m_instance;
}

}	// end namespace oi
