#include "planet.h"
#include "point.h"
#include "vector3f.h"

std::vector<Point*> gridBox;
int size = 104; //needs multiple of 4 in order to divide quads
int totalsize = size*size;
int platformSize = 10;
int wireframeMode = 0;
//int faultNum2 = 401; //for some reason faultNum doesnt work
const int faultNum = 501; //for some reason this # needs to be re-assigned each time at below
//faultNum = 401;

float camPos[] = {5, 5, 7};
float scenePos[] = {0, 0, 0};
float lineCol[3] = {0.5,0.5,0.5};
float verts[8][3] = { {-1,-1,1}, {-1,1,1}, {1,1,1}, {1,-1,1}, {-1,-1,-1}, {-1,1,-1}, {1,1,-1}, {1,-1,-1} };
float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };

float heightFactor = 0.5;        //fault algorithm metaVar
float heightMin = 0.85;          //maximum heights
float heightMax = 1.4;           //these are ration to original heights

int initD = 0;

std::vector<GLfloat> vertices;
std::vector<GLfloat> normals;
std::vector<GLfloat> texcoords;
std::vector<GLushort> indices;

//Point **heightmap;
std::vector< std::vector<Point*> > heightmap;

void SolidSphere(float radius, int rings, int sectors)
{
  float const R = 1./(float)(rings-1);
  float const S = 1./(float)(sectors-1);
  int r, s;

  vertices.resize(rings * sectors * 3);
  normals.resize(rings * sectors * 3);
  texcoords.resize(rings * sectors * 2);
  std::vector<GLfloat>::iterator v = vertices.begin();
  std::vector<GLfloat>::iterator n = normals.begin();
  std::vector<GLfloat>::iterator t = texcoords.begin();
  for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
    float const y = sin( -M_PI_2 + M_PI * r * R );
    float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
    float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
      *t++ = s*S;
      *t++ = r*R;

      *v++ = x * radius;
      *v++ = y * radius;
      *v++ = z * radius;

      *n++ = x;
      *n++ = y;
      *n++ = z;
  }

  indices.resize(rings * sectors * 4);
  std::vector<GLushort>::iterator i = indices.begin();
  for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
    *i++ = r * sectors + s;
    *i++ = r * sectors + (s+1);
    *i++ = (r+1) * sectors + (s+1);
    *i++ = (r+1) * sectors + s;
  }
} //end of solidsphere

void drawFace(int a, int b, int c, int d, float v[8][3]){
	glBegin(GL_POLYGON);
		glVertex3fv(v[a]);
		glVertex3fv(v[b]);
		glVertex3fv(v[c]);
		glVertex3fv(v[d]);
	glEnd();
}

void cube(float v[8][3])
{//cube drawn from some ccw some clockwise, do all clockwise
	glColor3fv(cols[0]);
	drawFace(3, 2, 1, 0, v);
	glColor3fv(cols[1]);
	drawFace(4, 0, 1, 5, v);  //now clockwise, was 1 0 4 5
	glColor3fv(cols[2]);
	drawFace(1, 2, 6, 5, v);
	glColor3fv(cols[3]);
	drawFace(3, 7, 6, 2, v); // just fixed, rightside greenface
	glColor3fv(cols[4]);
	drawFace(6, 7, 4, 5, v);  //now clockwise, was 6 5 4 7
	glColor3fv(cols[5]);
	drawFace(4, 7, 3, 0, v);  //now clockwise, was 4 0 3 7
}

void drawBox(float* c, float w, float h, float d)
{
	float vertices[8][3] = { {c[0]-w/2, c[1]-h/2, c[2]+d/2},
							 {c[0]-w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]+h/2, c[2]+d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]+d/2}, 
							 {c[0]-w/2, c[1]-h/2, c[2]-d/2}, 
							 {c[0]-w/2, c[1]+h/2, c[2]-d/2}, 
							 {c[0]+w/2, c[1]+h/2, c[2]-d/2},
							 {c[0]+w/2, c[1]-h/2, c[2]-d/2} };
	cube(vertices);
}

Planet::Planet(){
  // Initialise the heightmap
  //heightmap = new Point*[size];
  //for (int i = 0; i < size-1; ++i)
  //  heightmap[i] = new Point[size];

  tryC();
    printf("faultNum %d\n",faultNum);
    for (int i=0; i <faultNum; i++){
     tryB();
    }
    tryA();

  makeHeightmap();
  calculateNormals(); //add normal values to final vertices in gridbox    
}

Planet::~Planet(){
  //delete[] heightmap;
}

void Planet::tryA(){
	int gridSize = gridBox.size();
  //printf("tryA here, %d\n", gridSize);
  //now fix the seam
  for (int i = 0; i < gridSize; i ++){
    if ((i>3) && ((i%size == size-2)||(i%size == size-1)||(i%size == 0)||(i%size == 1)||(i%size == 2)) ){ //last row of elements, average them
      //printf("imod and size is %d %d\n", i%size,size);
      Point *x = gridBox.at(i); //the last guys
      Point *y = gridBox.at(i-1); //the first guy
      float a = (x->s + y->s)/2;
      //float b = (x->s + y->s)/2;
      x->s = a;
      //y->s = b;
    } //end of if
  } //end of for
  //now give vertices # back
  for (int i = 0; i < gridSize; i++) {
    Point *x = gridBox.at(i);
    //if (i > gridSize-200)
    //printf("tryA: x->pxyz i and s %d %f %f %f\n", i, x->px, x->py, x->pz);
    float v = vertices.at(i*3);
    float w = vertices.at(i*3+1);
    float q = vertices.at(i*3+2);
float v2 = 0;
float w2 = 0;
float q2 = 0;
    if (x->s < 1){
    float z = (1-float(x->s))*0.5;
    v2 = v* (1-z);
    w2 = w* (1-z);
    q2 = q* (1-z);
    }
    if (x->s >= 1){
    float z = (float(x->s)-1)*0.5;
    v2 = v* (1+z);
    w2 = w* (1+z);
    q2 = q* (1+z);
    }
    //v2 = v* float(x->s);
    //w2 = w* float(x->s);
    //q2 = q* float(x->s);

x->px = v2;
x->py = w2;
x->pz = q2;
    //x->px = (vertices.at(i)* x->s)/vertices.at(i);
    //x->py = (vertices.at(i+1)* x->s)/vertices.at(i+1);
    //x->pz = (vertices.at(i+2)* x->s)/vertices.at(i+2);
    //printf("x-xyz y-s %f %f %f %f\n",x->px, x->py,x->pz, x->s);
    //printf("x-xyz y-s %f %f %f %f\n",v2, w2,q2, x->s);
    
  }
  //end of for*/
  //; //transform final vertices in gridbox to 2D array
  
}

void Planet::tryB(){
//printf("tryB size of gridbox %d\n",gridBox.size());

  if (gridBox.size() > 0) {
  //printf("tryB\n");
  //apply fault algorithm
  //1. choose 2 points,
  //p1= along x-axis(2450..2499), y-axis(50*(0..49))
  //p2= along first line(0..49), right edge (49*(1..51))
  int p1select = 0;
  int p2select = 0;
  float aSlope = 0; //y = mx + b, this is m
  float aInt = 0;   //y = mx + b, this is b
  p1select = rand()%100;
  p2select = rand()%100;
  if (p1select < 40){
    p1select = 0; //along y axis
    p1select = (rand()%size)*size; //0-4950, up by 50
  } else {
    p1select = 1; //along x axis
    p1select = rand()%size + size*(size-1); //2450-2499
  }//end of if
  if (p2select < 40){
    p2select = 0; //along top
    p2select = rand()%size; //0-49, 0..size
  } else {
    p2select = 1; //along right edge
    p2select = (size-1)+size*((rand()%size)); //49..2499
  }//end of if
  //2. find slope
  Point *p1 = gridBox.at(p1select);
  Point *p2 = gridBox.at(p2select);
  aSlope = (p2->dy - p1->dy) / (p2->dx - p1->dx);
  //3. find intercept
  aInt = p1->dy - aSlope*p1->dx;
  //3.2 now find the new intercept & inverted slope to apply plane cutting through the sphere effect
  float bSlope = -aSlope; // y=mx+b this is m
  //printf("tryB122\n");
  if (aSlope != 0)
    bSlope = -1/aSlope;
  float bInt =(aSlope*(size/2)+aInt) -bSlope*(size/2);   //y = mx + b, this is b, b' = y-x*bS, y = aS*size/2 + aI
  //printf("intercept slope a b %f %f %f %f\n", aInt, aSlope, bInt, bSlope);
  //4. move it, if the point is above the function
  int aDecide = rand()%100;
  //if (aDecide < 30)
  //  aDecide = -1; //-1 means goes down
  float aDecideH = heightFactor*float(rand()%100)/1000; //amount of displacement
  //printf("tryB19\n");
//heightMin = 0.9;          //maximum heights
//heightMax = 1.1;           //these are ration to 
//printf("heightMinMax %f %f %f\n", heightMin, heightMax,aDecideH);
  if (aDecide < 30)
    aDecideH = -aDecideH; //invert height
  for (int i = 0; i < totalsize; i++){
    Point *x = gridBox.at(i);
    //now instead of doing fault algorithm in 2D, this is sphere which requires a kink half way of sphere cutting into two hemispheres
    //printf("for loop i %d\n",i);
    if (x->dx <= (size+1)/2) { //left half, original alg
      if (x->dy >= (aSlope*(x->dx) + aInt)) {
      //this spot is above function line, change height
//printf("heightMinMax height s %f %f %f %f\n", heightMin, heightMax,aDecideH,x->s);

        if (x->s < 1.1)  //apply maximum height
          x->s += aDecideH;
      } else {
        if (x->s > 0.95)  //apply minimum height
          x->s -= aDecideH;
      }
    } else { //right half, flip the slope
      if (x->dy >= (bSlope*(x->dx) + bInt)) {
      //this spot is above function line, change height
        if (x->s < 1.1)  //apply maximum height
          x->s += aDecideH;
      } else {
        if (x->s > 0.95)  //apply minimum height
          x->s -= aDecideH;
      }
    }
  }//end of for  
} //end of if gridBox not empty
}

void Planet::tryC(){
  SolidSphere(4, size, size);
  gridBox.clear();
  //this function will create the grid for terrain
  //50x50
  //initialize height map
  int iterCount = 0;          // 0 .. size for y
  int iterCount2 = 0;         //0 .. size for x axis
  for (int i = 0; i < totalsize*3; i++) {
    if ( ((i%3) == 2) && (i != 0) ){ //begin chopping
      iterCount2++;
      Point *aVertex = new Point();
      gridBox.push_back(aVertex);
      //here is the reference book
      Point *x = gridBox.at(iterCount2-1);
      if ((i!=0) && ((i%size) == 0))
        iterCount ++; //start from 1st line, 0..size-1
      float thisX = 0;
      float thisY = 0;
      float thisZ = 0;

      thisX = vertices.at(i-2);
      thisY = vertices.at(i-1);
      thisZ = vertices.at(i);
      x->px = thisX;
      x->py = thisY;    //x,z is actually 3d coordinate
      x->pz = thisZ;    //store actual xyz values

      //now give them 2D coordinate for fault algorithm
      x->dx = float(iterCount2%size);
      x->dy = float(size-iterCount);
      //printf("the chopped coordinates %f %f %f\n",x->px,x->py,x->pz);
    //printf("thisX dY dZ : %f %f %f\n", x->dx, x->dy, x->dz);
    //now give them colour
    int q3, w3, e3 = 0; //for colour
    float r3, t3, y3 = 0;
    q3 = rand()%100;     //this determines speed
    w3 = rand()%100;
    e3 = rand()%100;
    r3 = float(q3)/100;  //this determines pR
    t3 = float(w3)/100;  //this determines pG
    y3 = float(e3)/100;  //this determines pB
    x->pR = r3;
    x->pG = t3;
    x->pB = y3;
    } //end of if chopping
  } //end of for
}

//This function transform vector gridBox into 2D array.
void Planet::makeHeightmap(){
	
  heightmap.clear();
  //printf("gridbox size: %d\n", gridBox.size());

  //int tsize = gridBox.size() - 1;
  //int rsize = 104;
  int a = 0;
  
	for (int i = 0; i < size; i++){
    std::vector<Point*> tempvector;

		for (int j = 0; j < size; j++){
      tempvector.push_back(gridBox[a]);
      //printf("a: %d, i: %d, j: %d\n", a, i, j);
			//heightmap.push_back(gridBox[a]);
			a++;
		}

    heightmap.push_back(tempvector);
	}
}

void Planet::calculateNormals(){
	int a = 0;

	for (int i = 0; i < heightmap.size(); i++){
		for (int j = 0; j < heightmap[i].size(); j++){

			Vector3f normal; //define default normal (0, 0, 0)
			Vector3f up, down, right, left; //direction vectors

			if (i > 0){ //check if allowed to go up

				Vector3f a = Vector3f(heightmap[i][j]->px, heightmap[i][j]->py, heightmap[i][j]->pz);
        Vector3f b = Vector3f(heightmap[i -1][j]->px, heightmap[i -1][j]->py, heightmap[i -1][j]->pz);

        up = a - b;

        //up = heightmap[i][j] - heightmap[i - 1][j];


			}

			if (i<size - 1){ //check if allowed to go down

        Vector3f a = Vector3f(heightmap[i][j]->px, heightmap[i][j]->py, heightmap[i][j]->pz);
        Vector3f b = Vector3f(heightmap[i +1][j]->px, heightmap[i +1][j]->py, heightmap[i +1][j]->pz);
				down = a - b;
			}

			if (j>0){ //check if allowed to go left
				//left = heightmap[i][j] - heightmap[i][j - 1];
        Vector3f a = Vector3f(heightmap[i][j]->px, heightmap[i][j]->py, heightmap[i][j]->pz);
        Vector3f b = Vector3f(heightmap[i][j-1]->px, heightmap[i][j-1]->py, heightmap[i][j-1]->pz);
        
        left = a - b;
			}

			if (j<size - 1){ //check if allowed to go right
        Vector3f a = Vector3f(heightmap[i][j]->px, heightmap[i][j]->py, heightmap[i][j]->pz);
        Vector3f b = Vector3f(heightmap[i][j+1]->px, heightmap[i][j+1]->py, heightmap[i][j+1]->pz);
        right = a - b;
        //right = heightmap[i][j] - heightmap[i][j + 1];
			}

			normal = normal + (up*left).normalized();
			normal = normal + (right*up).normalized();
			normal = normal + (left*down).normalized();
			normal = normal + (down*right).normalized();
			//update vertices' normals in gridbox;
			gridBox.at(a)->nx = normal.x;
			gridBox.at(a)->ny = normal.y;
			gridBox.at(a)->nz = normal.z;
			a++;
		}
	}
}

void Planet::display(){
float origin[3] = {0,0,0};
/*  if (initD == 0){
    initD++;
    tryC();
    faultNum = 401; //for some reason faultnum needs to be re-assigned
    printf("faultNum %d\n",faultNum);
    for (int i=0; i <faultNum; i++){
     tryB();
    }
    tryA();    
  } else {*/
    //glPointSize(5);
    ////glColor3f(1,1,1);
    origin[0] = origin[1] = origin[2] = 0;
    //drawBox(origin, 0.5, 0.5, 0.5);

  //1.clear frame buffer
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear the color buffer, set the background
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //campos x y z, targetpos x y z, gamma?
  //gluLookAt(0,0,1.00f,0,0,0,0,1,0);
  //gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
  glColor3f(1,1,1);
  //glRotatef(scenePos[0],scenePos[1],scenePos[2], 0); //rotate whole scene
  glRotatef(scenePos[1],0,1,0); //around y axis
  //glRotatef(scenePos[0],1,0,0); //around x axis
  //glRotatef(scenePos[2],0,0,1); //around x axis

  for (int j = 0; j <gridBox.size()-size-1; j++){
      if ((j%size==1)  || j%size==103|| j%size==102 || (j%size==3) || (j%size==5)) {
      glPointSize(5);
      glBegin(GL_POINTS);
      //glBegin(GL_LINE_STRIP);
      //glVertex3f(gridBox.at(j)->px, gridBox.at(j)->py, gridBox.at(j)->pz);
      //glVertex3f(gridBox.at(j+1)->px, gridBox.at(j+1)->py, gridBox.at(j+1)->pz);

      glEnd();
      }
  }
  int rowCount = 0;
  int colCount = 0;
  //glBegin(GL_POINTS);
  //glPolygonModel( GL_FRONT_AND_BACK, GL_LINE);
  //glBegin(GL_LINE_STRIP);
  glBegin(GL_QUADS);
  //glBegin(GL_POLYGON);
  //int verticesSize = vertices.size()-size*3-3;
  //instead of vertices size use gridSize

  //go all the way but the last line
  //iterate thru gridBox, and show
  float m_ambient_planet[] = {0.0f, 0.0f, 0.0f, 1.0f};
  float m_specular_planet[] = {0.3f, 0.3f, 0.3f, 1.0f};
  float m_diffuse_planet[] = {0.0f, 1.0f, 0.0f, 1.0f};
  float shiny_factor_planet = 5.0f;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient_planet);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular_planet);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny_factor_planet);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse_planet);

  for (int i = 0; i < gridBox.size()-size; i++){
    if ( (i != 0 ) && (i % size == 0) ) //not first index, and first columns for each row
      rowCount++;
    //if (rowCount>100)
      //printf("rowCount %d\n",rowCount); //0-103
//&& (i%size != size-1)  (rowCount < size) &&   && (i%size != size-1) 
    //if ((i>0) && (i%size != 0) && (i%size < size-1) ){ //ring# under size, not first cols and last
      if ( (i>0) && ((i%size)!=0) && ((i%size) < size-1) && ((i%size)>=1) && ((i%size) != (104))&& ((i%size) != (103)) ){
      colCount++;
      //give them colour
      //glColor3f(gridBox.at(i)->pR,gridBox.at(i)->pG,gridBox.at(i)->pB);
      float colIntensity = 0;
      //normalized colour intensity
      colIntensity = ((gridBox.at(i)->s)-heightMin)/(heightMax-heightMin);
      if (colIntensity<=1)
        glColor3f(colIntensity*0.5+0.5,colIntensity*0.5+0.5,colIntensity*0.5+0.5);  //1.0 
      if (colIntensity<0.8)
        glColor3f(colIntensity*0.6+0.3,colIntensity*0.6+0.3,colIntensity*0.6+0.3);  //0.9
      if (colIntensity<0.6)
        glColor3f(colIntensity*0.5+0.2,colIntensity*0.5+0.2,colIntensity*0.5+0.2);  //0.7
      if (colIntensity<0.4)
        glColor3f(colIntensity*0.4+0.1,colIntensity*0.4+0.1,colIntensity*0.4+0.1);  //0.5
      if (colIntensity<0.2)
        glColor3f(colIntensity*0.3+0.1,colIntensity*0.3+0.1,colIntensity*0.3+0.1);  //0.4
      if (colIntensity<0.1)
        glColor3f(0.1,0.1,0.1);  //0.1
      //printf("colIntensity %f %f %f\n",heightMin,heightMax,colIntensity);
      //glVertex3f(gridBox.at(i-1)->px,gridBox.at(i-1)->py,gridBox.at(i-1)->pz); //at index0
      //glVertex3f(gridBox.at(i)->px,gridBox.at(i)->py,gridBox.at(i)->pz); //at index1
      //glVertex3f(gridBox.at(i+size+1)->px,gridBox.at(i+size+1)->py,gridBox.at(i+size+1)->pz); //at index1 + size -1
      
      glNormal3f(gridBox.at(i)->nx, gridBox.at(i)->ny, gridBox.at(i)->nz);
      glVertex3f(gridBox.at(i)->px,gridBox.at(i)->py,gridBox.at(i)->pz); //at index1
      
      glNormal3f(gridBox.at(i-1)->nx, gridBox.at(i-1)->ny, gridBox.at(i-1)->nz);
      glVertex3f(gridBox.at(i-1)->px,gridBox.at(i-1)->py,gridBox.at(i-1)->pz); //at index1 + size

      glNormal3f(gridBox.at(i+size-1)->nx, gridBox.at(i+size-1)->ny, gridBox.at(i+size-1)->nz);
      glVertex3f(gridBox.at(i+size-1)->px,gridBox.at(i+size-1)->py,gridBox.at(i+size-1)->pz); //at index1 + size -1

      glNormal3f(gridBox.at(i+size)->nx, gridBox.at(i+size)->ny, gridBox.at(i+size)->nz);
      glVertex3f(gridBox.at(i+size)->px,gridBox.at(i+size)->py,gridBox.at(i+size)->pz); //at index1 + size
      
    }//end of if
    /*if ((rowCount < size-1) && (i!=0) && (i%(size-1)==0) ){
      //glVertex3f(gridBox.at(i-(size-1))->px,gridBox.at(i-(size-1))->py,gridBox.at(i-(size-1))->pz); //at index1 - size
      glVertex3f(gridBox.at(i-size+1)->px,gridBox.at(i-size+1)->py,gridBox.at(i-size+1)->pz); //at index0
      glVertex3f(gridBox.at(i)->px,gridBox.at(i)->py,gridBox.at(i)->pz); //to myself
      //glVertex3f(gridBox.at(i+size-1)->px,gridBox.at(i+size-1)->py,gridBox.at(i+size-1)->pz); //to below me
      glVertex3f(gridBox.at(i+size)->px,gridBox.at(i+size)->py,gridBox.at(i+size)->pz); //at index104 is next row 0 to below me
      glVertex3f(gridBox.at(i+1)->px,gridBox.at(i+1)->py,gridBox.at(i+1)->pz); //at last index 103

    }//end of if*/

  }//end of for
  glEnd();

  //now draw the seam
  for (int i = 0; i < gridBox.size()-1; i ++){
    if ((i%size == size-1) && (i<(size*(size-1)-1)*3) ){ //last row of elements, average them
      //glPointSize(5);
      //glBegin(GL_POINTS);
      glBegin(GL_QUADS);
      //glBegin(GL_LINE_STRIP);
     //glVertex3f(vertices.at(i*3+3), vertices.at(i*3+1+3),vertices.at(i*3+2+3)); //last guys, next row
      glNormal3f(gridBox.at(i)->nx, gridBox.at(i)->ny, gridBox.at(i)->nz);
      glVertex3f(gridBox.at(i)->px,gridBox.at(i)->py,gridBox.at(i)->pz); //at index1
      glNormal3f(gridBox.at(i-1)->nx, gridBox.at(i-1)->ny, gridBox.at(i-1)->nz);
      glVertex3f(gridBox.at(i-1)->px,gridBox.at(i-1)->py,gridBox.at(i-1)->pz); //at index1 + size
      glNormal3f(gridBox.at(i+size-1)->nx, gridBox.at(i+size-1)->ny, gridBox.at(i+size-1)->nz);
      glVertex3f(gridBox.at(i+size-1)->px,gridBox.at(i+size-1)->py,gridBox.at(i+size-1)->pz); //at index1 + size -1
      glNormal3f(gridBox.at(i+size)->nx, gridBox.at(i+size)->ny, gridBox.at(i+size)->nz);
      glVertex3f(gridBox.at(i+size)->px,gridBox.at(i+size)->py,gridBox.at(i+size)->pz); //at index1 + size
      //printf("here is the last guys index %d %d and move index by size-1, %d,real index n adjusted %d, above %d\n", i%size, size, i,(i-size+1)*3, i*3);
      glEnd();
   } //end of if
   if ((i%size == 0) && (i<(size*(size-1)-1)*3) && (i>0) && (i < size*(size-1)) ){ //0 - size seam, not first row, not last row
      //glPointSize(5);
      //glBegin(GL_POINTS);
      glBegin(GL_QUADS);
      //glBegin(GL_LINE_STRIP);
     //glVertex3f(vertices.at(i*3+3), vertices.at(i*3+1+3),vertices.at(i*3+2+3)); //last guys, next row
      glNormal3f(gridBox.at(i)->nx, gridBox.at(i)->ny, gridBox.at(i)->nz);
      glVertex3f(gridBox.at(i)->px,gridBox.at(i)->py,gridBox.at(i)->pz); //at index1
      glNormal3f(gridBox.at(i+size)->nx, gridBox.at(i+size)->ny, gridBox.at(i+size)->nz);
      glVertex3f(gridBox.at(i+size)->px,gridBox.at(i+size)->py,gridBox.at(i+size)->pz); //at index1 + size
      
      glNormal3f(gridBox.at(i-1)->nx, gridBox.at(i-1)->ny, gridBox.at(i-1)->nz);
      glVertex3f(gridBox.at(i-1)->px,gridBox.at(i-1)->py,gridBox.at(i-1)->pz); //at index1 + size -1
      glNormal3f(gridBox.at(i+(size-1))->nx, gridBox.at(i+(size-1))->ny, gridBox.at(i+(size-1))->nz);
      glVertex3f(gridBox.at(i+(size-1))->px,gridBox.at(i+(size-1))->py,gridBox.at(i+(size-1))->pz); //at index1 + size      
//printf("here is the last guys index %d %d and move index by size-1, %d,real index n adjusted %d, above %d\n", i%size, size, i,(i-size+1)*3, i*3);
      glEnd();
   } //end of if
  } //end of for draw the seam

 // glBegin();
//draw sea
    float m_ambient_water[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float m_specular_water[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float m_diffuse_water[] = {0.0f, 0.0f, 1.0f, 1.0f};
    float shiny_factor_water = 10.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient_water);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular_water);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny_factor_water);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse_water);
     
     glutSolidSphere(4.0f, 50, 50);
  //glEnd();

  //glutSwapBuffers();
  //glutPostRedisplay();
//}//end of if else
}

void Planet::keyboard(unsigned char c, int x, int y){

int mod =glutGetModifiers();

switch(c){
    case 'r':       //reset scene
    case 'R':
      gridBox.clear();
      //printf("gridBox clear from pressing r\n");
      tryC();
      //printf("tryC from pressing r, faultNum %d\n", faultNum);
      for (int i = 0; i< faultNum; i++)
        tryB();
      tryA();

      makeHeightmap();
      calculateNormals();
      break;
    case 'c':       //reset scene position
    case 'C':
      scenePos[0] = 0;
      scenePos[1] = 0;
      scenePos[2] = 0;
      break;
    case 'j':
      scenePos[1]-=0.5; //rotate around y axis
      break;
    case 'l':
      scenePos[1]+=0.5;
      break;
    case 'i':
      if (scenePos[0]<50)
        scenePos[0]+=0.5;
      break;
    case 'k':
      if (scenePos[0]>-50)
        scenePos[0]-=0.5;
      break;
    case 'q':
    case 'Q':
        exit(0);
        break;

    case 27:
	exit(0);
	break;
    default:
	break;
}
}


//added: line 30 (define heightmap). line 167, line 168. function makeHeightmap
//calculateNormals
