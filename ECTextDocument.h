#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECTextViewImp.h"
#include "ECCommand.h"
#include "ECEditorView.h"

#include <vector>
#include <string>

// Text Document

class ECTextDocument;

//----------------------------------------------------------------
//           COMMANDs
//----------------------------------------------------------------

// Insert
class InsertCommand : public ECCommand
{
public:
    InsertCommand(ECTextDocument &docIn, int row, int col, char charIn);
    ~InsertCommand();
    void Execute();
    void UnExecute();

private:
    ECTextDocument &doc;
    int rowPos;
    int colPos;
    char charToInsert;
};

// Command to remove a character
class RemoveCommand : public ECCommand
{
public:
    RemoveCommand(ECTextDocument &docIn, int row, int col);
    ~RemoveCommand();
    void Execute();
    void UnExecute();

private:
    ECTextDocument &doc;
    int rowPos;
    int colPos;
};

// Command to add new row or break one line into two
class EnterCommand : public ECCommand
{
public:
    EnterCommand(ECTextDocument &docIn, int row, int col);
    ~EnterCommand();
    void Execute();
    void UnExecute();

private:
    ECTextDocument &doc;
    int rowPos;
    int colPos;
};

class ArrowCommand : public ECCommand
{
public:
    ArrowCommand(ECTextDocument &docIn, int key);
    ~ArrowCommand();
    void Execute();
    void UnExecute();

private:
    ECTextDocument &doc;
    int arrowKey;
};

class KeyEvtHdler
{
};

class ArrowKeyHdler : public KeyEvtHdler
{
};
class ECTextDocument
{
public:
    // Init
    ECTextDocument(ECEditorView &v);
    virtual ~ECTextDocument();

    // Get
    std::vector<std::string> GetRows();
    ECEditorView GetEditorView();

    // Text Manipulation
    void InsertChar(char c);
    void EraseChar(int row, int col);
    void AddNewLine(int row, int col);
    void MoveCursor(int direction);

    //char GetCharAt(int row, int col) const;

    void Test();

    // Cursor Action
    int GetCursorX();
    int GetCursorY();

    // View Management
    void SendToView(int row, int col);

private:
    ECEditorView &view;
    std::vector<std::string> listRows;
    int curX;
    int curY;
};
#endif