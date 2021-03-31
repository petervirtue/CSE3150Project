edit: ECEditor.o ECTextViewImp.o
	g++ ECEditor.o ECTextViewImp.o -o edit

ECTextViewImp.o: ECTextViewImp.cpp
	g++ -c ECTextViewImp.cpp

ECEditor.o: ECEditor.cpp
	g++ -c ECEditor.cpp

clean:
	rm *.o edit
