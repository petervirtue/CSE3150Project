#include "ECTextDocument.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//----------------------------------------------------------------
//           TEXT DOCUMENT
//----------------------------------------------------------------

ECTextDocument ::ECTextDocument(ECEditorView &v, std::string &fileName) : view(v), fName(fileName)
{
    pageIndex = 0;

    // EXP - Setting lines from a given file
    listRows = GetLinesFromFile();
    view.SetRows(listRows, 0, 0, fName);
}

ECTextDocument ::~ECTextDocument() {}

void ECTextDocument ::SendToView(int row, int col, int page)
{
    SetLinesInFile();
    view.SetRows(listRows, row, col, fName);
}

// MOVE ARROWS

void ECTextDocument ::MoveCursor(int direction)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

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

    SendToView(curY, curX, pageIndex);
}

// Command Utils

// INSERT

bool ECTextDocument ::InsertChar(char c, int row, int col, bool undoRedo)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    string s = string(1, c);
    if (listRows.size() <= row)
    {
        listRows.push_back(s);
    }
    else if (col >= listRows[row].size())
    {
        listRows[row].append(s);
    }
    else
    {
        listRows[row].insert(col, s);
    }

    col += 1;

    if (undoRedo)
    {
        CheckCursor(curY, curX);
    }
    else
    {
        SendToView(row, col, pageIndex);
    }

    //SendToView(curY, curX, pageIndex);

    // char is always going to be inserted, deletion is really what we are looking to handle
    return true;
}

void ECTextDocument ::UndoInsert(int row, int col)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    if (row < listRows.size() && col < listRows[row].size())
    {
        listRows[row].erase(col, 1);
    }

    CheckCursor(curY, curX);
}

void ECTextDocument ::RedoInsert(int row, int col, char c)
{
    // Get cursor position
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    string s = string(1, c);
    if (listRows.size() <= row)
    {
        listRows.push_back(s);
    }
    else if (col >= listRows[row].size())
    {
        listRows[row].append(s);
    }
    else
    {
        listRows[row].insert(col, s);
    }

    CheckCursor(curY, curX);
}

// REMOVE

std::pair<char, int> ECTextDocument ::EraseChar(int row, int col, bool undoRedo)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    pair<char, int> toRemove;
    toRemove.first = '\0';
    toRemove.second = -1;

    if (listRows.size() == 0 || (listRows.size() == 1 && listRows[0].size() == 0))
    {
        return toRemove;
    }

    if (col > 0)
    {
        toRemove.first = listRows[row].at(col - 1);
        listRows[row].erase(col - 1, 1);
        col -= 1;
    }
    else if (row > 0)
    {
        string extra = listRows[row];
        listRows.erase(listRows.begin() + row);
        row--;
        col = listRows[row].size();
        toRemove.second = col;
        if (extra.size() != 0)
        {
            listRows[row].append(extra);
        }
        toRemove.first = '\n';
    }

    if (undoRedo)
    {
        CheckCursor(curY, curX);
    }
    else
    {
        SendToView(row, col, pageIndex);
    }

    return toRemove;
}

void ECTextDocument ::UndoErase(int row, int col, char c)
{
    // Get cursor position
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    string s = string(1, c);
    if (listRows.size() <= row)
    {
        listRows.push_back(s);
    }
    else if (col - 1 >= listRows[row].size())
    {
        listRows[row].append(s);
    }
    else
    {
        listRows[row].insert(col - 1, s);
    }

    CheckCursor(curY, curX);
}

void ECTextDocument ::RedoErase(int row, int col)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    if (col > 0)
    {
        listRows[row].erase(col - 1, 1);
    }
    else if (row > 0)
    {
        string extra = listRows[row];
        listRows.erase(listRows.begin() + row);
        row--;
        if (extra.size() != 0)
        {
            listRows[row].append(extra);
        }
    }

    CheckCursor(curY, curX);
}

// RETURN

void ECTextDocument ::AddNewLine(int row, int col)
{
    int curX = col; //view.GetCursorX();
    int curY = row; //view.GetCursorY();

    if (listRows.size() == 0)
    {
        listRows.push_back("");
        listRows.push_back("");
        curX = 0;
        curY = 1;
    }
    else
    {
        string extra = listRows[curY].substr(curX);
        listRows[curY].erase(listRows[curY].begin() + curX, listRows[curY].end());
        curY++;
        listRows.insert(listRows.begin() + curY, extra);
        //listRows.push_back(extra);
        curX = 0;
    }

    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::UndoNewLine(int row, int col)
{
    int curY = view.GetCursorY();
    int curX = view.GetCursorX();

    string extra = listRows[row + 1];
    listRows.erase(listRows.begin() + row + 1);
    if (extra.size() != 0)
    {
        listRows[row].append(extra);
    }

    CheckCursor(curY, curX);
}

void ECTextDocument ::RedoNewLine(int row, int col)
{
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    if (listRows.size() == 0)
    {
        listRows.push_back("");
        listRows.push_back("");
    }
    else
    {
        string extra = listRows[row].substr(col);
        listRows[row].erase(listRows[row].begin() + col, listRows[col].end());
        row++;
        listRows.insert(listRows.begin() + row, extra);
        //listRows.push_back(extra);
    }

    CheckCursor(curY, curX);
}

// CHECK CURSOR

void ECTextDocument ::CheckCursor(int row, int col)
{

    // Make sure the rows are safe
    if (row >= listRows.size())
    {
        row = (listRows.size() != 0 ? listRows.size() - 1 : 0);
        col = (listRows[row].size() != 0 ? listRows[row].size() : 0);
    }

    // Make sure the columns are safe
    if (col > listRows[row].size())
    {
        col = (listRows[row].size() != 0 ? listRows[row].size() : 0);
    }

    SendToView(row, col, pageIndex);
}

// Getting and setting lines from file

std::vector<std::string> ECTextDocument ::GetLinesFromFile()
{
    // open the file
    ifstream src;
    src.open(fName);

    // list of lines & current line
    vector<string> lines;
    string cur;

    // check to see if the file exists
    if (!src)
    {
        ofstream f(fName);
        f.close();
    }
    else
    {
        while (getline(src, cur))
        {
            lines.push_back(cur);
        }

        src.close();
    }

    // return the lines
    return lines;
}

void ECTextDocument ::SetLinesInFile()
{
    ofstream file(fName);

    for (int i = 0; i < listRows.size(); i++)
    {
        file << listRows[i] << endl;
    }

    file.close();
}

//-------------------------------------------------------------
//           INSERT COMMAND
//-------------------------------------------------------------

InsertCommand ::InsertCommand(ECTextDocument &docIn, int row, int col, char charIn) : doc(docIn), rowPos(row), colPos(col), charToInsert(charIn) {}

// tried doing something here and it broke, trying doing nothing
InsertCommand ::~InsertCommand() {}

bool InsertCommand ::Execute()
{
    return doc.InsertChar(charToInsert, rowPos, colPos, false);
}

void InsertCommand ::UnExecute()
{
    //doc.UndoInsert(rowPos, colPos);
    doc.EraseChar(rowPos, colPos + 1, true);
}

void InsertCommand ::ReExecute()
{
    doc.InsertChar(charToInsert, rowPos, colPos, true);
    //doc.RedoInsert(rowPos, colPos, charToInsert);
}

//-------------------------------------------------------------
//           REMOVE COMMAND
//-------------------------------------------------------------

RemoveCommand ::RemoveCommand(ECTextDocument &docIn, int row, int col) : doc(docIn), rowPos(row), colPos(col)
{
}

RemoveCommand ::~RemoveCommand() {}

bool RemoveCommand ::Execute()
{
    pair<char, int> result = doc.EraseChar(rowPos, colPos, false);
    charRemoved = result.first;
    delX = result.second;

    if (charRemoved == '\0')
    {
        return false;
    }
    else if (charRemoved == '\n')
    {
        charRemoved = '\0';
    }

    return true;
}

void RemoveCommand ::UnExecute()
{
    //doc.InsertChar(charRemoved, rowPos, colPos);
    if (delX != -1)
    {
        doc.RedoNewLine(rowPos - 1, delX);
    }
    else
    {
        //doc.UndoErase(rowPos, colPos, charRemoved);
        doc.InsertChar(charRemoved, rowPos, colPos, true);
    }
}

void RemoveCommand ::ReExecute()
{
    //doc.UndoInsert(rowPos, colPos);
    //doc.RedoErase(rowPos, colPos);
    doc.EraseChar(rowPos, colPos, true);
}

//-------------------------------------------------------------
//           ARROW COMMAND
//-------------------------------------------------------------

ArrowCommand ::ArrowCommand(ECTextDocument &docIn, int key) : doc(docIn), arrowKey(key) {}

ArrowCommand ::~ArrowCommand() {}

bool ArrowCommand ::Execute()
{
    doc.MoveCursor(arrowKey);
    return false;
}

//-------------------------------------------------------------
//           RETURN COMMAND
//-------------------------------------------------------------

EnterCommand ::EnterCommand(ECTextDocument &docIn, int row, int col) : doc(docIn), rowPos(row), colPos(col) {}

EnterCommand ::~EnterCommand() {}

bool EnterCommand ::Execute()
{
    doc.AddNewLine(rowPos, colPos);
    return true;
}

void EnterCommand ::UnExecute()
{
    doc.UndoNewLine(rowPos, colPos);
}

void EnterCommand ::ReExecute()
{
    doc.RedoNewLine(rowPos, colPos);
}