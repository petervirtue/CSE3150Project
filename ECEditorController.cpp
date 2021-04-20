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
    int row = view.GetCursorY();
    int col = view.GetCursorX();

    // Once testing is over, this is where the KeyPressHandler will be called so that we're not doing the handline in this file
    // But for now, handle in here

    if (key == ENTER)
    {
        // Retuen key pressed, add new row
        //view.Refresh();
        AddNewLine(row, col);
    }
    else if (key == CTRL_Q)
    {
        // Control Q pressed
        //view.Quit();
        view.Clear();
        view.Quit();
    }
    else if (key == CTRL_R)
    {
        // CURRENTLY BROKEN
        Redo();
    }
    else if (key == CTRL_Z)
    {
        // CURRENTLY BROKEN
        Undo();
    }
    else if (key == BACKSPACE)
    {
        RemoveCharAt(row, col);
    }
    else if (key == ARROW_LEFT || key == ARROW_RIGHT || key == ARROW_UP || key == ARROW_DOWN)
    {
        MoveCursor(key);
    }
    else
    {
        // Any other key pressed

        char toInsert = char(key);
        InsertCharAt(row, col, toInsert);
    }

    // DEBUG
    //view.status = "position: " + to_string(histCmds.GetPos()) + " | histCmds size: " + to_string(histCmds.GetSize());
    //view.Refresh();
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

bool ECEditorController ::Undo()
{
    return histCmds.Undo();
}

bool ECEditorController ::Redo()
{
    return histCmds.Redo();
}