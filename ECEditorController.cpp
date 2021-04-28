#include "ECEditorController.h"

#include <string>

using namespace std;

//----------------------------------------------------------------
//           EDITOR CONTROLLER
//----------------------------------------------------------------

ECEditorController ::ECEditorController(ECTextDocument &td, ECEditorView &v) : doc(td), view(v)
{
    view.AttachController(this);
    editMode = EDITING;
}

ECEditorController ::~ECEditorController() {}

void ECEditorController ::Update()
{
    // Get the key pressed and the location of it
    int key = view.GetKeyPressed();
    int row = view.GetCursorY();
    int col = view.GetCursorX();

    // Once testing is over, this is where the KeyPressHandler will be called so that we're not doing the handline in this file (TBD, switching to this method at this point would be a lot of work)
    // But for now, handle in here

    if (editMode == FIND)
    {
        // Find mode controlling
        if (key == ESC)
        {
            EnterEditingMode();
        }
        else if (key == BACKSPACE)
        {
            RemoveSearchChar();
        }
        else if (key == ENTER)
        {
            SendSearch();
        }
        else if (char(key) == '/')
        {
            EnterReplaceMode();
        }
        else if (key != ARROW_DOWN || key != ARROW_UP || key != ARROW_LEFT || key != ARROW_RIGHT)
        {
            char toInsert = char(key);
            AddSearchChar(toInsert);
        }
    }
    else if (editMode == REPLACE)
    {
        // Replace mode controlling
        if (key == ESC)
        {
            EnterEditingMode();
        }
        else if (key == BACKSPACE)
        {
            RemoveReplaceChar();
        }
        else if (key == ENTER)
        {
            SendReplace();
        }
        else if (key != ARROW_DOWN || key != ARROW_UP || key != ARROW_LEFT || key != ARROW_RIGHT)
        {
            char toInsert = char(key);
            AddReplaceChar(toInsert);
        }
    }
    else
    {
        // Editing Mode Controlling
        if (key == ENTER)
        {
            // Retuen key pressed, add new row
            AddNewLine(row, col);
        }
        else if (key == CTRL_Q)
        {
            view.Clear();
            view.Quit();
        }
        else if (key == CTRL_R)
        {
            Redo();
        }
        else if (key == CTRL_Z)
        {
            Undo();
        }
        else if (key == BACKSPACE)
        {
            RemoveCharAt(row, col);
        }
        else if (key == CTRL_F)
        {
            EnterSearchMode();
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
    }

    // DEBUG
    //view.status = "position: " + to_string(histCmds.GetPos()) + " | histCmds size: " + to_string(histCmds.GetSize());
    //view.Refresh();
}

// EDITING MODE

void ECEditorController ::EnterEditingMode()
{
    editMode = EDITING;
    doc.EnterEditingMode();
}

// SEARCH MODE

void ECEditorController ::EnterSearchMode()
{
    editMode = FIND;
    doc.EnterSearchMode();
    // Document Enter Search Mode
}

void ECEditorController ::AddSearchChar(const char c)
{
    // Document Add Search Char
    doc.AddSearchChar(c);
}

void ECEditorController ::RemoveSearchChar()
{
    // Document Remove Search Char
    doc.RemoveSearchChar();
}

void ECEditorController ::SendSearch()
{
    // Document Send Search
    doc.ExecuteSearch();
}

// REPLACE MODE

void ECEditorController ::EnterReplaceMode()
{
    editMode = REPLACE;
    doc.EnterReplaceMode();
    // Document Enter Replace Mode
}

void ECEditorController ::AddReplaceChar(const char c)
{
    doc.AddReplaceChar(c);
}

void ECEditorController ::RemoveReplaceChar()
{
    doc.RemoveReplaceChar();
}

void ECEditorController ::SendReplace()
{
    ReplaceCommand *rc = new ReplaceCommand(this->doc);
    histCmds.ExecuteCmd(rc);

    EnterEditingMode();
}

void ECEditorController ::ShowEditor()
{
    view.ShowView();
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