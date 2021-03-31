#include "ECTextDocument.h"

#include <vector>
#include <string>

using namespace std;

//----------------------------------------------------------------
//           TEXT DOCUMENT
//----------------------------------------------------------------

//(ECEditorView &v) : view(v) {}
ECTextDocument ::ECTextDocument(ECEditorView &v) : view(v)
{
    curX = view.GetCursorX();
    curY = view.GetCursorY();
}

ECTextDocument ::~ECTextDocument() {}

void ECTextDocument ::SendToView(int row, int col)
{
    view.SetRows(listRows, row, col);
}

void ECTextDocument ::InsertChar(char c)
{
    //view.AddRow("Insert Char Running");
    string s = string(1, c);
    if (listRows.size() <= curY)
    {
        listRows.push_back(s);
    }
    else if (curX >= listRows[curY].size())
    {
        listRows[curY].append(s);
    }
    else
    {
        listRows[curY].insert(curX, s);
    }

    curX += 1;

    SendToView(curY, curX);
}

void ECTextDocument ::EraseChar(int row, int col)
{
    if (curX > 0)
    {
        listRows[curY].erase(curX - 1, 1);
        curX -= 1;
    }
    else if (curY > 0)
    {
        string extra = listRows[curY];
        listRows.erase(listRows.begin() + curY);
        curY--;
        curX = listRows[curY].size();
        if (extra.size() != 0)
        {
            listRows[curY].append(extra);
        }
        else if (curX > 0)
        {
            listRows[curY].erase(curX - 1, 1);
            curX--;
        }
    }

    SendToView(curY, curX);
}

void ECTextDocument ::MoveCursor(int direction)
{
    if (direction == ARROW_LEFT)
    {
        if (curX > 0)
        {
            curX--;
        }
        else if (curY > 0)
        {
            curY--;
            curX = listRows[curY].size();
        }
    }
    else if (direction == ARROW_RIGHT)
    {
        if (curX < listRows[curY].size())
        {
            curX++;
        }
        else if (curY < listRows.size() - 1)
        {
            curY++;
            curX = 0;
        }
    }
    else if (direction == ARROW_UP)
    {
        if (curY > 0)
        {
            curY--;
            if (curX > listRows[curY].size())
            {
                curX = listRows[curY].size();
            }
        }
        else
        {
            curX = 0;
        }
    }
    else if (direction == ARROW_DOWN)
    {
        if (curY < listRows.size() - 1)
        {
            curY++;
            if (curX > listRows[curY].size())
            {
                curX = listRows[curY].size();
            }
        }
        else
        {
            curX = listRows[curY].size();
        }
    }

    SendToView(curY, curX);
}

void ECTextDocument ::AddNewLine(int row, int col)
{
    string extra = listRows[curY].substr(curX);
    listRows[curY].erase(listRows[curY].begin() + curX, listRows[curY].end());
    curY++;
    listRows.insert(listRows.begin() + curY, extra);
    //listRows.push_back(extra);
    curX = 0;

    SendToView(curY, curX);
}

void ECTextDocument ::Test()
{
    listRows.push_back("Testing running");
}

int ECTextDocument ::GetCursorX()
{
    return curX;
}
int ECTextDocument ::GetCursorY()
{
    return curY;
}

//-------------------------------------------------------------
//           INSERT COMMAND
//-------------------------------------------------------------

InsertCommand ::InsertCommand(ECTextDocument &docIn, int row, int col, char charIn) : doc(docIn), rowPos(row), colPos(col), charToInsert(charIn) {}

// tried doing something here and it broke, trying doing nothing
InsertCommand ::~InsertCommand() {}

void InsertCommand ::Execute()
{
    doc.InsertChar(charToInsert);
    //doc.Test();
}

void InsertCommand ::UnExecute()
{
    doc.EraseChar(rowPos, colPos);
}

//-------------------------------------------------------------
//           REMOVE COMMAND
//-------------------------------------------------------------

RemoveCommand ::RemoveCommand(ECTextDocument &docIn, int row, int col) : doc(docIn), rowPos(row), colPos(col) {}

RemoveCommand ::~RemoveCommand() {}

void RemoveCommand ::Execute()
{
    doc.EraseChar(rowPos, colPos);
}

void RemoveCommand ::UnExecute()
{
}

//-------------------------------------------------------------
//           ARROW COMMAND
//-------------------------------------------------------------

ArrowCommand ::ArrowCommand(ECTextDocument &docIn, int key) : doc(docIn), arrowKey(key) {}

ArrowCommand ::~ArrowCommand() {}

void ArrowCommand ::Execute()
{
    doc.MoveCursor(arrowKey);
}

void ArrowCommand ::UnExecute()
{
}

//-------------------------------------------------------------
//           RETURN COMMAND
//-------------------------------------------------------------

EnterCommand ::EnterCommand(ECTextDocument &docIn, int row, int col) : doc(docIn), rowPos(row), colPos(col) {}

EnterCommand ::~EnterCommand() {}

void EnterCommand ::Execute()
{
    doc.AddNewLine(rowPos, colPos);
}

void EnterCommand ::UnExecute()
{
}