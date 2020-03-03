#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <time.h>
#include <ctime>
#include <chrono>
#include <mutex>
#include <sstream>
#include <type_traits>
#include <iomanip>

/**
 *  defines functions needed to log into a file/stream/etc
 */
class LogPolicyInterface
{
public:
	virtual void		openOstream(const std::string& name) = 0;
	virtual void		closeOstream() = 0;
	virtual void		write(const std::string& msg) = 0;
};

/*
 * Implementation which allow to write into a file
 */
class FileLogPolicy : public LogPolicyInterface
{
	std::unique_ptr< std::ofstream > out_stream;
public:
	FileLogPolicy() : out_stream(new std::ofstream) {}
	void openOstream(const std::string& name)
	{
		out_stream->open(name.c_str(), std::ios_base::binary | std::ios_base::out);
		if (!out_stream->is_open())
		{
			throw(std::runtime_error("LOGGER: Unable to open an output stream"));
		}
	}

	void closeOstream() { if (out_stream) out_stream->close(); }
	void write(const std::string& msg) { (*out_stream) << msg << std::endl; }
	~FileLogPolicy() { closeOstream(); }
};

/*
 * Implementation which writes to a console
 */
class ConsoleLogPolicy : public LogPolicyInterface
{
public:
	ConsoleLogPolicy() {}
	void openOstream(const std::string& name) {}
	void closeOstream() {}
	void write(const std::string& msg) { std::cerr << msg << std::endl; }
	~ConsoleLogPolicy() {}
};

// write to both console and a file location
class MultiLogPolicy : public LogPolicyInterface
{
	FileLogPolicy filePolicy;
	ConsoleLogPolicy consolePolicy;

public:
	MultiLogPolicy() {}
	void openOstream(const std::string& name)
	{

		filePolicy.openOstream(name);
		consolePolicy.openOstream(name);
	}
	void closeOstream()
	{

		filePolicy.closeOstream();
		consolePolicy.closeOstream();
	}
	void write(const std::string& msg)
	{

		filePolicy.write(msg);
		consolePolicy.write(msg);
	}
	~MultiLogPolicy() { closeOstream(); }
};

enum Severity
{
	debug = 1,
	info,
	notice,
	warning,
	error,
	critical,
	emergency,
};

template< typename LogPolicy >
class Logger
{
	// ensure inheritance
	static_assert(
		std::is_base_of<LogPolicyInterface, LogPolicy>::value, 
		"LogPolicy must be made from LogPolicyInterface"
	);

private:
	unsigned logLineNumber;
	std::stringstream logStream;
	LogPolicy* policy;
	std::mutex writeMutex;

private:
	std::string getTime();
	std::string getLogLineHeader();

	//Core printing functionality
	void _iPrint();

	// print implementation
	template<typename First, typename...Rest>
		void _iPrint(First parm1, Rest...parm);

public:
	Logger(const std::string& name);
	~Logger();

	template< Severity severity, typename...Args >
		void print(Args...args);
};

// ----------------- Logger ---------------------
template< typename LogPolicy >
Logger<LogPolicy>::Logger(const std::string& name)
	: logStream(), logLineNumber(1), writeMutex()
{
	policy = new LogPolicy();
	policy->openOstream(name);
}

template< typename LogPolicy >
Logger<LogPolicy>::~Logger()
{
	delete policy;
}

// base case: when there's no more parameters left
template< typename LogPolicy >
void Logger< LogPolicy >::_iPrint()
{
	policy->write(getLogLineHeader() + logStream.str());
	logStream.str("");
}

// recursively calling print...
template< typename LogPolicy >
template<typename First, typename...Rest >
void Logger< LogPolicy >::_iPrint(First parm1, Rest...parm)
{
	logStream << parm1;
	_iPrint(parm...);
}

// print message to a string stream with a mutex lock
template< typename LogPolicy >
template< Severity severity, typename...Args >
void Logger< LogPolicy >::print(Args...args)
{
	writeMutex.lock();
	switch (severity)
	{
	case Severity::debug:
		logStream << "<DEBUG>\t:  ";
		break;
	case Severity::warning:
		logStream << "<WARNING>\t:  ";
		break;
	case Severity::info:
		logStream << "<INFO>\t:  ";
		break;
	case Severity::error:
		logStream << "<ERROR>\t:  ";
		break;
	};
	_iPrint(args...);
	writeMutex.unlock();
}

// get current time
template< typename LogPolicy >
std::string Logger< LogPolicy >::getTime()
{
	std::stringstream buffer;
	auto now = std::chrono::system_clock::now();
	std::time_t t_c = std::chrono::system_clock::to_time_t(now);
	buffer << std::put_time(std::localtime(&t_c), "%F %T");

	//without the newline character
	return buffer.str();
}

template< typename LogPolicy >
std::string Logger< LogPolicy >::getLogLineHeader()
{
	std::stringstream header;

	header.str("");
	header.fill('0');
	header.width(9);
	header << logLineNumber++ << " < " << getTime() << " > - ";

	return header.str();
}

// logs to console only
extern Logger<ConsoleLogPolicy> ConsoleLog;
// logs to a file only
extern Logger<FileLogPolicy> FileLog;
// logs to both a file and the console
extern Logger<MultiLogPolicy> Log;