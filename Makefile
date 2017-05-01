parser.o: 
	g++ ./src/main.cpp ./src/pixelcolor.cpp ./src/pixelray.cpp ./src/render.cpp -I./glm-master -o raytrace

clean:
	\rm *.out
