#ifndef ECEditorView_h
#define ECEditorView_h

#include "ECTextViewImp.h"

#include <vector>
#include <string>

class ECEditorView
{
public:
    ECEditorView(ECTextViewImp &imp);
    ~ECEditorView();

    // Observer functions

    void AttachController(ECObserver *ob);

    // Showing/Quitting the view
    void ShowView();
    void Quit();
    void Refresh();
    void Clear();

    // Row management
    void AddRow(std::string row);
    void RemoveRow(int row);
    void SetRows(std::vector<std::string> rowsIn, int row, int col);

    // Get Key pressed
    int GetKeyPressed();

    // Cursor Positions
    int GetCursorX();
    int GetCursorY();

private:
    ECTextViewImp tvimp;
};

#endif