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
    void ExecuteMove(ECCommand *pCmd);
    int GetSize() { return listOfCommands.size(); };
    int GetPos() { return position; };

private:
    // your code goes here

    std::vector<ECCommand *> listOfCommands;
    std::vector<ECCommand *> listMovements;
    int position;
};

#endif