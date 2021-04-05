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

void ECEditorView ::ShowView()
{
    tvimp.Show();
}

void ECEditorView ::Quit()
{
    tvimp.Quit();
}

void ECEditorView ::AddRow(string row)
{
    tvimp.AddRow(row);
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

void ECEditorView ::SetRows(std::vector<string> rowsIn, int row, int col)
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

    string left = "Editing - Line " + to_string(curY + 1) + " - Page " + to_string(page + 1);

    tvimp.ClearStatusRows();
    tvimp.AddStatusRow(left, "Peter Virtue - CSE 3150", false);

    // DEBUG
    //tvimp.AddStatusRow(status, " ", false);
    //string testing = "page = " + to_string(page) + " | curY = " + to_string(curY) + " | pageY = " + to_string(pageY) + " | start = " + to_string(start) + " | end = " + to_string(end);
    //tvimp.AddStatusRow(testing, "", false);

    tvimp.Refresh();
}