#ifndef ECEditorView_h
#define ECEditorView_h

#include "ECTextViewImp.h"

#include <vector>
#include <string>

//----------------------------------------------------------------
//           EDITOR VIEW
//----------------------------------------------------------------

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
    void SetRows(std::vector<std::string> rowsIn, int row, int col, std::string fName, int eMode, std::string find);

    // Colored Rows
    void SetColoredRows();
    void AddColoredRow(std::vector<int> c);
    void ClearColoredRows();
    
    // Get Key pressed
    int GetKeyPressed();

    // Cursor Positions
    int GetCursorX();
    int GetCursorY();
    int GetViewRows();

    std::string status;

private:
    ECTextViewImp tvimp;
    int curX;
    int curY;
    int pageY;
    int pageIndex;
    std::vector<std::vector<int> > searchWords;
};

#endif