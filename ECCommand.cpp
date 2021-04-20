#include "ECCommand.h"

using namespace std;

//----------------------------------------------------------------
//           COMMAND HISTORY
//----------------------------------------------------------------

ECCommandHistory ::ECCommandHistory() : position(-1) {}

ECCommandHistory ::~ECCommandHistory()
{

    // delete each individual command
    for (int i = 0; i < listOfCommands.size(); i++)
    {
        delete listOfCommands[i];
    }

    // clear the list of commands and reset the position
    listOfCommands.clear();
    position = -1;
}

void ECCommandHistory ::ExecuteCmd(ECCommand *pCmd)
{
    // Instead make sure to change the position, if this does not work then we may want to look into doing command insertion
    if (pCmd->Execute())
    {
        position = listOfCommands.size();
        listOfCommands.push_back(pCmd);
    }
}

bool ECCommandHistory ::Undo()
{
    if (position == -1)
    {
        return false;
    }

    listOfCommands[position]->UnExecute();
    position--;
    return true;
}

bool ECCommandHistory ::Redo()
{
    if (position == listOfCommands.size() - 1)
    {
        return false;
    }
    position++;
    listOfCommands[position]->ReExecute();
    return true;
}