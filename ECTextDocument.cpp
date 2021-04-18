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

void ECTextDocument ::InsertChar(char c, int row, int col)
{
    int curX = col; //view.GetCursorX();
    int curY = row; //view.GetCursorY();

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

    SendToView(curY, curX, pageIndex);
}

char ECTextDocument ::EraseChar(int row, int col)
{
    int curX = col; //view.GetCursorX();
    int curY = row; //view.GetCursorY();

    char toRemove;

    if (curX > 0)
    {
        toRemove = listRows[curY].at(curX - 1);
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
    }

    SendToView(curY, curX, pageIndex);

    return toRemove;
}

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

void InsertCommand ::Execute()
{
    doc.InsertChar(charToInsert, rowPos, colPos);
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
    charRemoved = doc.EraseChar(rowPos, colPos);
}

void RemoveCommand ::UnExecute()
{
    doc.InsertChar(charRemoved, rowPos, colPos);
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