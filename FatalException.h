/* 
 * File:   FatalException.h
 * Author: aomx
 *
 * Created on 3. August 2013, 10:51
 */

#ifndef FATALEXCEPTION_H
#define	FATALEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class FatalException : public exception {
public:
    FatalException(string msg);
    const char* what() const throw();
    void handleException();
private:
    string msg;

};

#endif	/* FATALEXCEPTION_H */

