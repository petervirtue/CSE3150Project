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
    virtual bool Execute() = 0;
    virtual void UnExecute() = 0;
    virtual void ReExecute() = 0;
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
    int GetSize() { return listOfCommands.size(); };
    int GetPos() { return position; };

private:
    std::vector<ECCommand *> listOfCommands;
    int position;
};

#endif