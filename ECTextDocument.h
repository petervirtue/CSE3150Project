#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECTextViewImp.h"
#include "ECCommand.h"
#include "ECEditorView.h"

#include <vector>
#include <string>

class ECTextDocument;

//----------------------------------------------------------------
//           COMMANDS
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
    char charRemoved;
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

//----------------------------------------------------------------
//           TEXT DOCUMENT
//----------------------------------------------------------------

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
    void InsertChar(char c, int row, int col);
    char EraseChar(int row, int col);
    void AddNewLine(int row, int col);
    void MoveCursor(int direction);

    // View Management
    void SendToView(int row, int col, int page);

private:
    ECEditorView &view;
    std::vector<std::string> listRows;
    int pageIndex;
};
#endif