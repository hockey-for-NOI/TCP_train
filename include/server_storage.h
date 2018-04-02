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

class	PackedInput : public std::unique_ptr<std::ifstream>
{
public:
	PackedInput() = delete;
private:
	PackedInput(std::string const& filename);
	~PackedInput();
	std::string	m_filename;
};

class	PackedOutput : public std::unique_ptr<std::ofstream>
{
public:
	PackedOutput() = delete;
	void	discard();
private:
	PackedOutput(std::string const& filename);
	~PackedOutput();
	std::string	m_filename;
};

class	ServerStorage
{
public:
	static	std::string	storage_root;
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
