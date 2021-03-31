#include "ECCommand.h"

using namespace std;

//----------------------------------------------------------------
//           COMMAND HISTORY
//----------------------------------------------------------------

ECCommandHistory ::ECCommandHistory() : position(-1) {}

ECCommandHistory ::~ECCommandHistory()
{
    for (int i = 0; i < listOfCommands.size(); i++)
    {
        delete listOfCommands[i];
    }

    listOfCommands.clear();
    position = -1;
}

bool ECCommandHistory ::Undo()
{

    // check to make sure we can
    if (position < 0)
    {
        return false;
    }

    // un execute the previous command and move position back
    listOfCommands[position]->UnExecute();
    position--;

    return true;
}

bool ECCommandHistory ::Redo()
{
    if (position < -1)
    {
        // still return true
        return true;
    }

    // advance position and execute
    position++;
    listOfCommands[position]->Execute();

    return false;
}

void ECCommandHistory ::ExecuteCmd(ECCommand *pCmd)
{
    // push back and execute
    listOfCommands.push_back(pCmd);
    position++;
    pCmd->Execute();
}