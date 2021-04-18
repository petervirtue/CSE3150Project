#include "ECEditorController.cpp"
#include "ECTextDocument.cpp"
#include "ECEditorView.cpp"
#include "ECCommand.cpp"
#include "ECTextViewImp.h"
#include <iostream>
#include <fstream>

using namespace std;

//----------------------------------------------------------------
//           EDITOR
//----------------------------------------------------------------

// function for reading lines from a file

vector<string> readLinesFromFile(const string fName)
{
}

int main(int argc, char *argv[])
{
    // check for arguments
    if (argc != 2)
    {
        cout << "ERROR - Usage: ./edit <filename>" << endl;
        return 0;
    }
    else
    {
        string fName = argv[1];
        ECTextViewImp *imp = new ECTextViewImp();
        ECEditorView *ev = new ECEditorView(*imp);
        ECTextDocument *doc = new ECTextDocument(*ev, fName);
        ECEditorController ec = ECEditorController(*doc, *ev);
        ec.ShowEditor();
        return 0;
    }
}