CXXFLAGS=-lopencv_core -lopencv_highgui

main: main.cpp trace.cpp linedebug.cpp
	g++ $(CXXFLAGS) -o $@ $^
	cp main /var/www/html/imgprocess/cgi/processImage

clean:
	rm main
