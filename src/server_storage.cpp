#include "server_storage.h"

namespace	oi
{

namespace
{

std::string	safety_encode(std::string const& name)
{
	std::stringstream ss(ServerStorage::storage_root);
	for (char ch: name) ss << '_' << int(ch);
	return ss.str();
}

}	// end anonymous namespace

const	std::string	ServerStorage::storage_root = "/tmp/tcp/";
static	std::mutex	ServerStorage::m_mutex;
ServerStorage*	ServerStorage::m_instance = 0;

PackedInput::PackedInput(std::string const& filename):
	std::unique_ptr<std::ifstream>(new std::ifstream(safety_encode(filename), std::ifstream::binary)),
	m_filename(filename)
{
}

PackedInput::~PackedInput() {
	auto ptr = ServerStorage::get_instance();
	if (get()) p->close(m_filename);
}

PackedOutput::PackedOutput(std::string const& filename):
	std::unique_ptr<std::ofstream>(new std::ofstream(safety_encode(filename), std::ofstream::binary)),
	m_filename(filename)
{
}

void	PackedOutput::discard() {
	auto ptr = ServerStorage::get_instance();
	if (get()) p->close(m_filename);
	remove(safety_encode(m_filename));
	reset(nullptr);
}

PackedOutput::~PackedOutput() {
	auto ptr = ServerStorage::get_instance();
	if (get()) p->close(m_filename);
}

ServerStorage::ServerStorage()
{
	m_usage.clear();
	system(("mkdir -p " + storage_root).c_str());
}

ServerStorage*	ServerStorage::get_instance()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	if (!m_instance) m_instance = new ServerStorage();
	return m_instance;
}

PackedInput	ServerStorage::read(std::string const& filename)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	if (m_usage.count(filename)) return nullptr;
	PackedInput target(filename);
	if (target->fail()) return nullptr;
	m_usage.insert(filename);
	return target;
}

PackedOutput	ServerStorage::write(std::string const& filename)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	if (m_usage.count(filename)) return nullptr;
	PackedOutput target(filename);
	if (target->fail()) return nullptr;
	m_usage.insert(filename);
	return target;
}

void	ServerStorage::close(std::string const& filename)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_usage.erase(filename);
}

}	// end namespace oi
