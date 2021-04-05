#include "ECEditorController.h"

#include <string>

using namespace std;

//----------------------------------------------------------------
//           EDITOR CONTROLLER
//----------------------------------------------------------------

ECEditorController ::ECEditorController(ECTextDocument &td, ECEditorView &v) : doc(td), view(v)
{
    view.AttachController(this);
}

ECEditorController ::~ECEditorController() {}

void ECEditorController ::Update()
{

    // Get the key pressed
    int key = view.GetKeyPressed();
    int row = 0; //view.GetCursorX();
    int col = view.GetCursorY();

    // Once testing is over, this is where the KeyPressHandler will be called so that we're not doing the handline in this file
    // But for now, handle in here

    if (key == ENTER)
    {
        // Retuen key pressed, add new row
        //view.Refresh();
        AddNewLine(0, 0);
    }
    else if (key == CTRL_Q)
    {
        // Control Q pressed
        //view.Quit();
        view.Clear();
        view.Quit();
    }
    else if (key == BACKSPACE)
    {
        RemoveCharAt(0, 0);
    }
    else if (key == ARROW_LEFT || key == ARROW_RIGHT || key == ARROW_UP || key == ARROW_DOWN)
    {
        MoveCursor(key);
    }
    else
    {
        // Any other key pressed, this is what we really care about right now

        // Trying to insert character
        char toInsert = char(key);
        InsertCharAt(0, 0, toInsert);

        // DEBUG
        // view.AddRow("KEY PRESSED");
        // view.AddRow(string(1, toInsert));
    }
}

void ECEditorController ::ShowEditor()
{
    view.ShowView();
    view.AddRow("ShowEditor ran");
}

void ECEditorController ::InsertCharAt(int row, int col, const char c)
{
    InsertCommand *ic = new InsertCommand(this->doc, row, col, c);
    histCmds.ExecuteCmd(ic);
}

void ECEditorController ::RemoveCharAt(int row, int col)
{
    RemoveCommand *rc = new RemoveCommand(this->doc, row, col);
    histCmds.ExecuteCmd(rc);
}

void ECEditorController ::MoveCursor(int direction)
{
    ArrowCommand *ac = new ArrowCommand(this->doc, direction);
    histCmds.ExecuteCmd(ac);
}

void ECEditorController ::AddNewLine(int row, int col)
{
    EnterCommand *ec = new EnterCommand(this->doc, row, col);
    histCmds.ExecuteCmd(ec);
}
