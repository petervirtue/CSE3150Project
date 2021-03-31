#include "ECEditorView.h"

using namespace std;

//----------------------------------------------------------------
//           EDITOR VIEW
//----------------------------------------------------------------

ECEditorView ::ECEditorView(ECTextViewImp &imp) : tvimp(imp)
{
    tvimp.AddStatusRow("Editing - Project One Part One", "Peter Virtue - CSE3150", false);
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
    return tvimp.GetCursorX();
}

int ECEditorView ::GetCursorY()
{
    return tvimp.GetCursorY();
}

void ECEditorView ::Refresh()
{
    tvimp.Refresh();
}

void ECEditorView ::Clear()
{
    tvimp.InitRows();
    tvimp.AddRow("Clear Ran");
}

void ECEditorView ::AttachController(ECObserver *ob)
{
    tvimp.Attach(ob);
}

void ECEditorView ::SetRows(std::vector<string> rowsIn, int row, int col)
{
    tvimp.InitRows();

    for (int i = 0; i < rowsIn.size(); i++)
    {
        tvimp.AddRow(rowsIn[i]);
    }

    tvimp.SetCursorX(col);
    tvimp.SetCursorY(row);

    tvimp.Refresh();
}