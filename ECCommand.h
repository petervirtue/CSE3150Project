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
    // Init
    ECCommandHistory();
    virtual ~ECCommandHistory();

    // Undo/Redo Management
    bool Undo();
    bool Redo();

    // Execution
    void ExecuteCmd(ECCommand *pCmd);

    // Get
    int GetSize() { return listOfCommands.size(); };
    int GetPos() { return position; };

private:
    std::vector<ECCommand *> listOfCommands;
    int position;
};

#endif