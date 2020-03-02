#include "Logger.h"

Logger<ConsoleLogPolicy> ConsoleLog("");
Logger<FileLogPolicy> FileLog("./alt.log");
Logger<MultiLogPolicy> Log("./default.log");