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
    // Page Index
    pageIndex = 0;

    // Editing Mode
    editMode = 0;

    // Search & Replace
    search = "";
    replace = "";

    // EXP - Setting lines from a given file
    listRows = GetLinesFromFile();
    view.SetRows(listRows, 0, 0, fName, editMode, "");
}

ECTextDocument ::~ECTextDocument() {}

//----------------------------------------------------------------
//           GENERAL FUNCTIONALITY
//----------------------------------------------------------------

void ECTextDocument ::SendToView(int row, int col, int page)
{
    SetLinesInFile();
    string find = "";
    if (editMode == 1)
    {
        find = search;
    }
    else if (editMode == 2)
    {
        find = replace;
    }

    view.SetRows(listRows, row, col, fName, editMode, find);
}

//----------------------------------------------------------------
//           EDITING MODE
//----------------------------------------------------------------

void ECTextDocument ::EnterEditingMode()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Set to Editing Mode
    editMode = 0;
    search = "";
    replace = "";

    // Clear colored lines
    view.ClearColoredRows();

    // Send to View
    SendToView(curY, curX, pageIndex);
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

    // char is always going to be inserted, deletion is really what we are looking to handle
    return true;
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

//----------------------------------------------------------------
//           SEARCH MODE
//----------------------------------------------------------------

void ECTextDocument ::EnterSearchMode()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Search Mode Enter
    editMode = 1;

    // Set replace to search incase we are coming from a replace
    search = replace;
    replace = "";

    // Highlight and send search if there is a predefined search
    ExecuteSearch();

    // Send to View
    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::AddSearchChar(char c)
{

    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Add the string
    string s = string(1, c);
    search.append(s);

    // Send to View
    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::RemoveSearchChar()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Remove the char
    if (search.size() > 0)
    {
        search.erase(search.size() - 1, 1);
    }

    // Send to View
    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::ExecuteSearch()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Clear rows first
    view.ClearColoredRows();

    // Find the occurrences
    if (search.size() != 0)
    {
        for (int i = 0; i < listRows.size(); i++)
        {
            for (int j = 0; j < listRows[i].size(); j++)
            {
                if (listRows[i].substr(j, search.size()) == search)
                {
                    int s = search.size() - 1;
                    vector<int> v;
                    v.push_back(i);
                    v.push_back(j);
                    v.push_back(j + s);
                    view.AddColoredRow(v);
                }
            }
        }
    }

    // Send to View
    SendToView(curY, curX, pageIndex);
}

//----------------------------------------------------------------
//           REPLACE MODE
//----------------------------------------------------------------

void ECTextDocument ::EnterReplaceMode()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Search Mode Enter
    replace = "";
    editMode = 2;

    // Send to View
    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::AddReplaceChar(char c)
{

    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Add the string
    string s = string(1, c);
    replace.append(s);

    // Send to View
    SendToView(curY, curX, pageIndex);
}

void ECTextDocument ::RemoveReplaceChar()
{
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Remove the char
    if (replace.size() > 0)
    {
        replace.erase(replace.size() - 1, 1);
    }

    // Send to View
    SendToView(curY, curX, pageIndex);
}

std::pair<std::vector<std::pair<int, int> >, std::pair <std::string, std::string> > ECTextDocument ::ExecuteReplace()
{

    // Return Format - PAIR(VECTOR[PAIR <ROW, COL>], PAIR<replaceWith, toReplace>)
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Clear rows first
    view.ClearColoredRows();

    // Populate Return Data
    vector<std::pair<int, int> > locs;
    std::pair<string, string> reps;
    reps.first = replace;
    reps.second = search;

    // Find the occurrences and add locations, need to do a second double loop in order to make sure replaces work fine
    if (search.size() != 0)
    {
        for (int i = 0; i < listRows.size(); i++)
        {
            for (int j = 0; j < listRows[i].size(); j++)
            {
                if (listRows[i].substr(j, search.size()) == search)
                {
                    // Send Location to the vector
                    std::pair<int, int> loc;
                    loc.first = i;
                    loc.second = j;
                    locs.push_back(loc);

                    // Replace by using erase and insert instead of replace
                    listRows[i].erase(j, search.size());
                    listRows[i].insert(j, replace);
                    j = j + replace.size();
                }
            }
        }
    }

    // Send to View
    CheckCursor(curY, curX);

    // Return Data
    std::pair<std::vector<std::pair<int, int> >, std::pair <std::string, std::string> > ret;
    ret.first = locs;
    ret.second = reps;
    return ret;
}

void ECTextDocument ::UndoReplace(std::vector<std::pair<int, int> > locs, std::string replacedWith, std::string old)
{
    // Return Format - PAIR(VECTOR[PAIR <ROW, COL>], PAIR<replaceWith, toReplace>)
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Have to go backwards to account for locations being put in forward order
    for (int i = locs.size() - 1; i >= 0; i--)
    {
        // Replace
        listRows[locs[i].first].erase(locs[i].second, replacedWith.size());
        listRows[locs[i].first].insert(locs[i].second, old);
    }

    // Send to View
    CheckCursor(curY, curX);
}

std::vector<std::pair<int, int> > ECTextDocument ::RedoReplace(std::string toReplace, std::string replacedWith)
{
    // Return Format - PAIR(VECTOR[PAIR <ROW, COL>]
    // Cursor
    int curX = view.GetCursorX();
    int curY = view.GetCursorY();

    // Clear rows first
    view.ClearColoredRows();

    // Populate Return Data
    vector<std::pair<int, int> > locs;

    // Find the occurrences and add locations, need to do a second double loop in order to make sure replaces work fine
    if (toReplace.size() != 0)
    {
        for (int i = 0; i < listRows.size(); i++)
        {
            for (int j = 0; j < listRows[i].size(); j++)
            {
                if (listRows[i].substr(j, toReplace.size()) == toReplace)
                {

                    // Send Location to the vector
                    std::pair<int, int> loc;
                    loc.first = i;
                    loc.second = j;
                    locs.push_back(loc);

                    // Replace by using erase and insert instead of replace
                    listRows[i].erase(j, toReplace.size());
                    listRows[i].insert(j, replacedWith);
                    j = j + replacedWith.size();
                }
            }
        }
    }

    // Send to View
    CheckCursor(curY, curX);

    // Return Data
    return locs;
}

//-------------------------------------------------------------
//           INSERT COMMAND
//-------------------------------------------------------------

InsertCommand ::InsertCommand(ECTextDocument &docIn, int row, int col, char charIn) : doc(docIn), rowPos(row), colPos(col), charToInsert(charIn) {}

InsertCommand ::~InsertCommand() {}

bool InsertCommand ::Execute()
{
    return doc.InsertChar(charToInsert, rowPos, colPos, false);
}

void InsertCommand ::UnExecute()
{
    doc.EraseChar(rowPos, colPos + 1, true);
}

void InsertCommand ::ReExecute()
{
    doc.InsertChar(charToInsert, rowPos, colPos, true);
}

//-------------------------------------------------------------
//           REMOVE COMMAND
//-------------------------------------------------------------

RemoveCommand ::RemoveCommand(ECTextDocument &docIn, int row, int col) : doc(docIn), rowPos(row), colPos(col) {}

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
    if (delX != -1)
    {
        doc.RedoNewLine(rowPos - 1, delX);
    }
    else
    {
        doc.InsertChar(charRemoved, rowPos, colPos, true);
    }
}

void RemoveCommand ::ReExecute()
{
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

//-------------------------------------------------------------
//           REPLACE COMMAND
//-------------------------------------------------------------

ReplaceCommand ::ReplaceCommand(ECTextDocument &docIn) : doc(docIn) {}

ReplaceCommand ::~ReplaceCommand() {}

bool ReplaceCommand ::Execute()
{
    std::pair<std::vector<std::pair<int, int> >, std::pair <string, string> > p = doc.ExecuteReplace();
    locations = p.first;
    toReplace = p.second.second;
    replacedWith = p.second.first;
    return true;
}

void ReplaceCommand ::UnExecute()
{
    doc.UndoReplace(locations, replacedWith, toReplace);
}

void ReplaceCommand ::ReExecute()
{
    locations = doc.RedoReplace(toReplace, replacedWith);
}