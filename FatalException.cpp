/* 
 * File:   FatalException.cpp
 * Author: aomx
 * 
 * Created on 3. August 2013, 10:51
 */

#include "FatalException.h"

#include "header.h"
#include "Logger.h"

FatalException::FatalException(const string msg) {
    this->msg = msg;        
    Logger::getInstance()->writeMessage("Fatal exception occured: " +  this->msg);
}

FatalException::~FatalException ()  throw() {

}

void FatalException::handleException() {
    DBG("Fatal exception occured: %s", this->msg.c_str());
    
    exit(0);
}

const char* FatalException::what() const throw() {
    return this->msg.c_str();
}

