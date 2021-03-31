#ifndef ECCommand_h
#define ECCommand_h

#include "ECTextDocument.h"

#include <vector>

//----------------------------------------------------------------
//           COMMAND
//----------------------------------------------------------------

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};

//----------------------------------------------------------------
//           COMMAND HISTORY
//----------------------------------------------------------------

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    void ExecuteCmd(ECCommand *pCmd);

private:
    // your code goes here

    std::vector<ECCommand *> listOfCommands;
    int position;
};

#endif