// Test code for editor
#include "ECTextViewImp.h"
#include <iostream>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{
    //
    ECTextViewImp wndTest;
    wndTest.AddRow("CSE 3150");
    wndTest.SetColor(0, 0, 0, TEXT_COLOR_RED);
    wndTest.SetColor(0, 1, 1, TEXT_COLOR_GREEN);
    wndTest.SetColor(0, 2, 2, TEXT_COLOR_BLUE);
    wndTest.SetColor(0, 4, 4, TEXT_COLOR_RED);
    wndTest.SetColor(0, 5, 5, TEXT_COLOR_GREEN);
    wndTest.SetColor(0, 6, 6, TEXT_COLOR_BLUE);
    wndTest.SetColor(0, 7, 7, TEXT_COLOR_MAGENTA);
    wndTest.AddRow("This is a very simple demo of the ECTextViewImp functionalities.");
    wndTest.SetColor(1, 10, 13, TEXT_COLOR_GREEN);
    wndTest.AddRow("Press ctrl-q to quit");
    wndTest.SetColor(2, 0, 1, TEXT_COLOR_GREEN);
    wndTest.SetColor(2, 0, 1, TEXT_COLOR_DEF);

    // add a status bar
    wndTest.AddStatusRow("Editor", "For demo only", true);
    
    wndTest.Show();
    
    return 0;
}
