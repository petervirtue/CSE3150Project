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
    bool Execute();
    void UnExecute();
    void ReExecute();

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
    bool Execute();
    void UnExecute();
    void ReExecute();

private:
    ECTextDocument &doc;
    int rowPos;
    int colPos;
    int delX;
    char charRemoved;
};

// Command to add new row or break one line into two
class EnterCommand : public ECCommand
{
public:
    EnterCommand(ECTextDocument &docIn, int row, int col);
    ~EnterCommand();
    bool Execute();
    void UnExecute();
    void ReExecute();

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
    bool Execute();
    void UnExecute(){};
    void ReExecute(){};

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
    ECTextDocument(ECEditorView &v, std::string &fileName);
    virtual ~ECTextDocument();

    // Get
    std::vector<std::string> GetRows();
    ECEditorView GetEditorView();

    // Text Manipulation
    bool InsertChar(char c, int row, int col, bool undoRedo);
    std::pair<char, int> EraseChar(int row, int col, bool undoRedo);
    void AddNewLine(int row, int col);
    void MoveCursor(int direction);

    // Undo/Redo Functions

    // Insert
    void UndoInsert(int row, int col);
    void RedoInsert(int row, int col, char c);

    // Remove
    void UndoErase(int row, int col, char c);
    void RedoErase(int row, int col);

    // New Line
    void UndoNewLine(int row, int col);
    void RedoNewLine(int row, int col);

    // General
    void CheckCursor(int row, int col);

    // View Management
    void SendToView(int row, int col, int page);

    // File Management
    std::vector<std::string> GetLinesFromFile();
    void SetLinesInFile();

private:
    ECEditorView &view;
    std::string fName;
    std::vector<std::string> listRows;
    int pageIndex;
};
#endif