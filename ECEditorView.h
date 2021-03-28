#ifndef ECEditorView_h
#define ECEditorView_h

#include "ECTextViewImp.h"
#include "ECTextDocument.h"

class ECEditorView : ECObserver
{
public:
    // Observer functions
    virtual ~ECEditorView();
    virtual void Update();

private:
    ECTextViewImp tv;
    ECTextDocument subject;
};

#endif