#include <pthread.h>
#include <ctime>

#include "Logger.h"
#include "header.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

Logger::Logger() {
	this->logFile = fopen("log.txt", "w");
}

Logger::~Logger() {
	fclose(this->logFile);
}

Logger* Logger::getInstance() {
	static Logger instance;
	return &instance;
}

void Logger::writeMessage(string msg) {
	//    DBG("logger writing message");
	time_t t = time(0); // get time now
	struct tm * now = localtime(& t);
	char buf[80];
	strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", now);
	fprintf(this->logFile, "%s %s\n", buf, msg.c_str());
}

