#include "ECEditorController.cpp"
#include "ECTextDocument.cpp"
#include "ECEditorView.cpp"
#include "ECCommand.cpp"
#include "ECTextViewImp.h"

using namespace std;

int main(int argc, char *argv[])
{
    ECTextViewImp *imp = new ECTextViewImp();
    ECEditorView *ev = new ECEditorView(*imp);
    ECTextDocument *doc = new ECTextDocument(*ev);
    ECEditorController ec = ECEditorController(*doc, *ev);
    ec.ShowEditor();

    return 0;
}