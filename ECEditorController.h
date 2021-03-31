#ifndef ECEditorController_h
#define ECEditorController_h

#include "ECTextDocument.h"
#include "ECObserver.h"
#include "ECEditorView.h"

// Editor Controller
class ECEditorController : public ECObserver
{
public:
    // init with Text Document & Editor View
    //ECEditorController(ECTextDocument &td, ECEditorView &v);
    ECEditorController(ECTextDocument &td, ECEditorView &v);
    virtual ~ECEditorController();

    // Observer
    void Update();

    // Get
    ECTextDocument GetDocument() { return doc; }
    //ECEditorView GetEditorView();

    // Functions
    void InsertCharAt(int row, int col, const char c);
    void RemoveCharAt(int row, int col);
    void AddNewLine(int row, int col);
    void MoveCursor(int direction);
    bool Undo();
    bool Redo();

    // Show the editor
    void ShowEditor();

private:
    // Document and View
    ECTextDocument &doc;
    ECEditorView &view;

    // Command History
    ECCommandHistory histCmds;
};

#endif