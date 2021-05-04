#include "ECEditorView.h"

using namespace std;

//----------------------------------------------------------------
//           EDITOR VIEW
//----------------------------------------------------------------

ECEditorView ::ECEditorView(ECTextViewImp &imp) : tvimp(imp)
{
    tvimp.AddStatusRow("Editing - Line 1 - Page 1", "Peter Virtue - CSE 3150", false);
    pageY = 0;
    curY = 0;
    curX = 0;
}

ECEditorView ::~ECEditorView() {}

//----------------------------------------------------------------
//           GENERAL FUNCTIONALITY
//----------------------------------------------------------------

void ECEditorView ::ShowView()
{
    tvimp.Show();
}

void ECEditorView ::Quit()
{
    tvimp.Quit();
}

int ECEditorView ::GetKeyPressed()
{
    return tvimp.GetPressedKey();
}

int ECEditorView ::GetCursorX()
{
    return curX;
}

int ECEditorView ::GetCursorY()
{
    return curY;
}

void ECEditorView ::Refresh()
{
    tvimp.Refresh();
}

void ECEditorView ::Clear()
{
    tvimp.InitRows();
}

void ECEditorView ::AttachController(ECObserver *ob)
{
    tvimp.Attach(ob);
}

//----------------------------------------------------------------
//           COLORED ROWS
//----------------------------------------------------------------

void ECEditorView ::AddColoredRow(std::vector<int> c)
{
    searchWords.push_back(c);
}

void ECEditorView ::SetColoredRows()
{
    for (int i = 0; i < searchWords.size(); i++)
    {
        tvimp.SetColor(searchWords[i][0], searchWords[i][1], searchWords[i][2], TEXT_COLOR_RED);
    }
}

void ECEditorView ::ClearColoredRows()
{

    // Set old rows back to white
    for (int i = 0; i < searchWords.size(); i++)
    {
        tvimp.SetColor(searchWords[i][0], searchWords[i][1], searchWords[i][2], TEXT_COLOR_WHITE);
    }


    // Clear rows
    searchWords.clear();
}

//----------------------------------------------------------------
//           ROW HANDLING
//----------------------------------------------------------------

void ECEditorView ::AddRow(string row)
{
    tvimp.AddRow(row);
}

void ECEditorView ::SetRows(std::vector<string> rowsIn, int row, int col, std::string fName, int eMode, std::string find)
{
    // cursor handling
    curX = col;
    curY = row;

    // page handling

    // num of rows in view
    int rows = tvimp.GetRowNumInView();
    int page = curY / rows;

    // start and end points for view
    int start = page * rows;
    int end = (rows * (page + 1) > rowsIn.size() - 1 || rowsIn.size() < rows) ? rowsIn.size() : rows * (page + 1);

    tvimp.InitRows();

    for (int i = start; i < end; i++)
    {
        tvimp.AddRow(rowsIn[i]);
    }

    pageY = curY - (page * rows);

    tvimp.SetCursorX(curX);
    tvimp.SetCursorY(pageY);

    // Change the status bar based on mode

    string left = "Editing " + fName + " - Line " + to_string(curY + 1) + " - Page " + to_string(page + 1);
    string right = "Peter Virtue - CSE 3150";
    bool light = false;

    if (eMode == 1)
    {
        left = "SEARCH: " + find;
        light = true;
        SetColoredRows();
    }
    else if (eMode == 2)
    {
        left = "REPLACE WITH: " + find;
        light = true;
        SetColoredRows();
    }

    // Set the status rows
    tvimp.ClearStatusRows();
    tvimp.AddStatusRow(left, right, light);

    tvimp.Refresh();
}