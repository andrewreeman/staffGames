#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <QString>

class Except_StaffGames : public std::exception{

public:
    Except_StaffGames(QString functionName) : m_functionName(functionName){}
    virtual const char* what() const throw() { return ( QString("Error in ") + m_functionName + ".").toLatin1(); }

private:
    QString m_functionName;

};

class Except_MemoryAlloc : public Except_StaffGames{
public:
    Except_MemoryAlloc(QString functionName) : Except_StaffGames(functionName){}
};

class Except_OutOfBounds : public Except_StaffGames{
public:
  Except_OutOfBounds(QString functionName) : Except_StaffGames(functionName){}
};

#endif // EXCEPTIONS_H
