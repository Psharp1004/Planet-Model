#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace::std;
class Planet{
	public:
	Planet();
	~Planet();
	void tryA(void);
	void tryB(void);
	void tryC(void);
	void display();
	void update();
	void keyboard(unsigned char, int, int);
	void makeHeightmap();
	void calculateNormals();
};

//added two functions: makeHeightmap, calculateNormals