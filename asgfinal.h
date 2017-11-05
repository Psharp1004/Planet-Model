#include "point.h"
#include "planet.h"
#include "advcam.h"

class AsgFinal{

public:

	Planet planet;
	AdvancedCamera *camera;
	
	AsgFinal();
	~AsgFinal();

	void display();
	void update();
	void keyboard(unsigned char, int, int);
	void reshape(int, int);
	void mousePassive(int, int);
};
