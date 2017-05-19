CC = g++ -std=c++11

pipeline: main.o instruction.o function.o rpt.o fetch.o execute.o memory.o write.o regfile.o
	g++ -o pipeline main.o instruction.o function.o rpt.o fetch.o execute.o memory.o write.o regfile.o
main.o: main.cpp instruction.h function.h rpt.h fetch.h execute.h memory.h write.h regfile.h
	g++ -c -g main.cpp
instruction.o: instruction.cpp function.h rpt.h regfile.h 
	g++ -c -g instruction.cpp
rpt.o: rpt.cpp regfile.h
	g++ -c -g rpt.cpp
fetch.o: fetch.cpp regfile.h
	g++ -c -g fetch.cpp
execute.o: execute.cpp rpt.h regfile.h
	g++ -c -g execute.cpp
memory.o: memory.cpp rpt.h regfile.h	
	g++ -c -g memory.cpp
write.o: write.cpp rpt.h regfile.h
	g++ -c -g write.cpp
function.o: function.cpp regfile.h	
	g++ -c -g function.cpp
regfile.o: regfile.cpp
	g++ -c -g regfile.cpp
clean:
	rm -f main.o instruction.o function.o rpt.o fetch.o execute.o memory.o write.o regfile.o 
