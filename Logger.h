/* 
 * File:   Logger.h
 * Author: aomx
 *
 * Created on 3. August 2013, 14:08
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <stdio.h>
#include <string>

using namespace std;

class Logger {
    public:
        static Logger* getInstance();
        void writeMessage(string msg);

    private:
        Logger();
        virtual ~Logger();

        static Logger *instance;
       
        FILE *logFile;

};

#endif	/* LOGGER_H */

