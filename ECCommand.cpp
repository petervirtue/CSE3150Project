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

    // BROKEN
    // position = listOfCommands.size();
    // listOfCommands.push_back(pCmd);
    // pCmd->Execute();

    // BROKEN FOR WHEN NEW COMMANDS ARE ADDED
    // position++;
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
    //position++;
    return true;
}
//----------------------------------------------------------------
//           OLD COMMAND HISTORY
//----------------------------------------------------------------

// ECCommandHistory ::ECCommandHistory() : position(-1) {}

// ECCommandHistory ::~ECCommandHistory()
// {
//     for (int i = 0; i < listOfCommands.size(); i++)
//     {
//         delete listOfCommands[i];
//     }

//     listOfCommands.clear();
//     position = -1;
// }

// bool ECCommandHistory ::Undo()
// {

//     // check to make sure we can
//     if (position < 0)
//     {
//         return false;
//     }
//     else if (position < listOfCommands.size())
//     {
//         listOfCommands[position]->UnExecute();
//         position--;

//         return true;
//     }

//     position--;

//     // un execute the previous command and move position back
//     return false;
// }

// bool ECCommandHistory ::Redo()
// {
//     if (position < 0)
//     {
//         return true;
//     }
//     else if (position < listOfCommands.size())
//     {
//         listOfCommands[position]->Execute();
//         position++;

//         return true;
//     }

//     return false;
// }

// void ECCommandHistory ::ExecuteCmd(ECCommand *pCmd)
// {
//     // push back and execute
//     listOfCommands.push_back(pCmd);
//     position++;
//     pCmd->Execute();
// }

// void ECCommandHistory ::ExecuteMove(ECCommand *pCmd)
// {
//     pCmd->Execute();
// }