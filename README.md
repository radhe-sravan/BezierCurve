This is a git repository for Bezier Curve implementation in C using OpenGL. Check the project wiki for details about Bezier Curve.

USAGE

Compilation : 

Run 'make' in the home diectory of the project. Two files bezier1 and bezier2 are generated. The essential working of both the files is same and they differ only in the underlying implementation. bezier1.c is the implementation using De Casteljau Algorithm and bezier2.c is using Bernstein Polynomial. 

Running the code :

1. Run either of './bezier1' or './bezier2'. 
2. In the console enter the number of control points.
3. Mark the points in the new screen which pops up.
4. After the require number of points are plotted, the bezier curve is drawn.
5. You can change the position of any of the control points by doing a right click and dragging them to the required position. The curve moves dynamically with the change in the control point.
