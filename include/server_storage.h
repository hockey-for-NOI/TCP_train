#pragma once

#include <mutex>
#include <string>
#include <fstream>
#include <memory>
#include <unordered_set>

namespace	oi
{

class	PackedInput;
class	PackedOutput;
class	ServerStorage;

class	PackedInput : public std::shared_ptr<std::ifstream>
{
public:
	PackedInput();
	~PackedInput();
private:
	PackedInput(std::string const& filename);
	std::string	m_filename;

	friend class ServerStorage;
};

class	PackedOutput : public std::shared_ptr<std::ofstream>
{
public:
	PackedOutput();
	~PackedOutput();
	void	discard();
private:
	PackedOutput(std::string const& filename);
	std::string	m_filename;

	friend class ServerStorage;
};

class	ServerStorage
{
public:
	static	const	std::string	storage_root;
	static	ServerStorage*	get_instance();

	PackedInput	read(std::string const& filename);
	PackedOutput	write(std::string const& filename);
	void	close(std::string const& filename);
private:
	ServerStorage();
	static	ServerStorage*	m_instance;
	static	std::mutex	m_mutex;
	std::unordered_set <std::string>	m_usage;
};

}	// end namespace oi
