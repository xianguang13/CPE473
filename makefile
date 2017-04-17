parser.o:
	g++ ./Program1/src/main.cpp ./Program1/src/render.cpp ./Program1/src/pixelray.cpp -I./glm-master -I./Program1/src/ -I./Program1/resources -o raytrace
