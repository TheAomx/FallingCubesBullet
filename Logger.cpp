/* 
 * File:   Logger.cpp
 * Author: aomx
 * 
 * Created on 3. August 2013, 14:08
 */

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
    if (instance == NULL) {
        pthread_mutex_init(&mutex1, NULL);
        pthread_mutex_lock( &mutex1 );
        if (instance == NULL) {
            instance = new Logger();
        }
        pthread_mutex_unlock( &mutex1 );

    }
    return instance;
}

void Logger::writeMessage (string msg) {
//    DBG("logger writing message");
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    char buf[80];
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", now);
    fprintf(this->logFile, "%s %s\n", buf, msg.c_str());
 }

