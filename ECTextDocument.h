#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECTextViewImp.h"
#include "ECCommand.h"
#include "ECEditorView.h"

#include <utility>
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

class ReplaceCommand : public ECCommand
{
public:
    ReplaceCommand(ECTextDocument &docIn);
    ~ReplaceCommand();
    bool Execute();
    void UnExecute();
    void ReExecute();

private:
    ECTextDocument &doc;
    std::string toReplace;
    std::string replacedWith;
    std::vector<std::pair<int, int> > locations;
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
    void UndoNewLine(int row, int col);
    void RedoNewLine(int row, int col);

    // Cursor Management
    void MoveCursor(int direction);
    void CheckCursor(int row, int col);

    // Editing Mode
    void EnterEditingMode();

    // Search Mode
    void EnterSearchMode();
    void AddSearchChar(char c);
    void RemoveSearchChar();
    void ExecuteSearch();

    // Replace Mode
    void EnterReplaceMode();
    void AddReplaceChar(char c);
    void RemoveReplaceChar();
    std::pair<std::vector<std::pair<int, int> >, std::pair <std::string, std::string> > ExecuteReplace();
    void UndoReplace(std::vector<std::pair<int, int> > locs, std::string replacedWith, std::string old);
    std::vector<std::pair<int, int> > RedoReplace(std::string toReplace, std::string replacedWith);

    // View Management
    void SendToView(int row, int col, int page);

    // File Management
    std::vector<std::string> GetLinesFromFile();
    void SetLinesInFile();

private:
    // View
    ECEditorView &view;

    // File name
    std::string fName;

    // Rows & page index
    std::vector<std::string> listRows;
    int pageIndex;

    // Mode
    int editMode;

    // Search & Replace Strings
    std::string search;
    std::string replace;
};
#endif