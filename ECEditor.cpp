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

int main(int argc, char *argv[])
{
    // Check to make sure the arguments are valid. I did not implement functionality to ensure .txt files as the directions indicated it is assumed that we will only get files of this type
    if (argc != 2)
    {
        cout << "ERROR - Usage: ./edit <filename>" << endl;
        return 0;
    }
    else
    {
        // File Name
        string fName = argv[1];

        // Implementations setup
        ECTextViewImp *imp = new ECTextViewImp();
        ECEditorView *ev = new ECEditorView(*imp);
        ECTextDocument *doc = new ECTextDocument(*ev, fName);
        ECEditorController ec = ECEditorController(*doc, *ev);

        // Show the Editor
        ec.ShowEditor();

        // Mandatory Return
        return 0;
    }
}