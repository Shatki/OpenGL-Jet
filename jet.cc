// OpenGL Jet program.
// ECE8893, Georgia Tech, Fall 2012

#include <iostream>

#ifdef LINUX
//Linux Headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows Headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

#include <fstream>
#include <vector>

using namespace std;

GLfloat updateRate = 1.0; // Change this later


class Vertex
{
	public:
    GLfloat x, y, z;
	
	public:
    Vertex(GLfloat a, GLfloat b, GLfloat c)
	{
		x=a;
		y=b;
		z=c;
	}
    Vertex()
    {
    x=-1;
    y=-1;
    z=-1;
    }

};

class Normal : public Vertex
{
public:
    
    Normal(GLfloat a, GLfloat b, GLfloat c) : public Vertex(a, b, c) {}
public:
  
};

class Face
{
public:
	
Face()
{
	x = Vertex() ;
	y = Vertex() ;
	z = Vertex() ;
}
	
Face(Vertex a, Vertex b, Vertex c, Normal na, Normal nb, Normal nc, int mat_inx) 
	{
		
	x = a ;
	y = b ;
	z = c ;
	nx = na ;
	ny = nb ;
	nz = nc ;
	mat_inx = mat_inx ;

	}

	Vertex x, y, z ;
	Normal nx, ny, nz ;
	int mtl ;
};

class RGB
{
	public:
    GLfloat r, g, b;
	public:
    RGB(GLfloat x, GLfloat y, GLfloat z) : r(x), g(y), b(z) {}

};


class Material
{
public:


	GLfloat Ns, Tr;
	RGB Ka;             // Ambient lighting in RGB
    RGB Kd;             // Diffuse lighting in RGB
    RGB Ke;             // Emissive lighting in RGB
    RGB Ks;             // Specular lighting in RGB
    
	unsigned int mtl_index;	
	
	
	public:
		Material(GLfloat ns, GLfloat tr, RGB ka, RGB kd, RGB ke, RGB ks, int a )
		{
			Ns=ns;
			Tr=tr;
			mtl_index=a;
			Ka=ka;
			Kd=kd;
			Ke=ke;
			Ks=ks;
			
		}
		Material()
		{	
			Ns=0;
			Tr=0;
			mtl_index=0;
			Ka.r=0;
			Ka.g=0;
			Ka.b=0;
			Kd.r=0;
			Kd.g=0;
			Kd.b=0;
			Ke.r=0;
			Ke.g=0;
			Ke.b=0;
			Ks.r=0;
			Ks.g=0;
			Ks.b=0;
			
		}	
		
};


vector<Material> materials;
vector<Vertex> vertices;
vector<Normal> normals;
vector<Face> faces;

bool wirefame=false;
bool x_rotation=false;
bool y_rotation=false;
bool z_rotation = false ;

float x_angle=0;
float y_angle=0;
float z_angle = 0 ;   //initial rotation angle


float scale = 1.0 ;


// code to read the obj file
/*void Read()
{
  ifstream ifs("jet.obj");
  int material=0;
  if (!ifs) return;
  while(ifs)
    {
      string header;
      ifs >> header;
      if (header == string("mtllib"))
        {
          string mtllib;
          ifs >> mtllib;
          cout << header << " " << mtllib << endl;
        }
      else if (header == string("v"))
        {
          float v[3];
          ifs >> v[0] >> v[1] >> v[2];
         // cout << header << " " << v[0] << " " << v[1] << " " << v[2] << endl;
           vertices.push_back(Vertex(v[0], v[1], v[2])) ;
        }
      else if (header == string("vn"))
        {
          // more here
          
          float vn[3];
          ifs >> vn[0] >> vn[1] >> vn[2];
          normals.push_back(Normal(vn[0], vn[1], vn[2])) ;
        
        }
        
        else if (header == string("usemtl"))
      {
        ifs >> material ;
      }
      
      else if((header== string ("f"))
      {
      	string f[3] ;
        int f_v[3] ;
        int blank[3] ;
        int f_n[3] ;
        ifs >> f[0] >> f[1] >> f[2];
        
        for (int i = 0; i < 3; i++)
        {
          sscanf((f[i].c_str()), "%i/%i/%i", &f_v[i], &blank[i], &f_n[i]);
        }
        faces.push_back(Face(vertices[f_v[0] - 1], vertices[f_v[1] - 1], vertices[f_v[2] - 1],
                             normals[f_n[0] - 1], normals[f_n[1] - 1], normals[f_n[2] - 1],
                             mtl)) ;
        faces[faces.size() - 1].mtl = mtl ;
      }  
    
      
}*/

void loadModel()
{
  //open file
  ifstream inputFile ("jet.obj");

  string line;
  int currMaterial = 0;

  if(inputFile.is_open())
    {
      while(inputFile.good())
        {
          getline(inputFile, line);

          if(line[0] == 'u')
            {
              sscanf(line.c_str(), "usemtl %d", &currMaterial);
            }

          //process vertex
          else if(line[0] == 'v' && line[1] == ' ')
            {
              vertex_t vert;
              sscanf(line.c_str(), "%*s %f %f %f", &vert.x, &vert.y, &vert.z);
              verticies.push_back(vert);
              //cout << "Vertex! ";
            }
          //process normal
          else if(line[0] == 'v' && line[1] == 'n')
            {
              normal_t normal;
              sscanf(line.c_str(), "%*s %f %f %f", &normal.i, &normal.j, &normal.k);
              normals.push_back(normal);
            }
          //process texture
          else if(line[0] == 'v' && line[1] == 't')
            {
              texture_t tex;
              sscanf(line.c_str(), "%*s %f %f", &tex.u, &tex.v);
              texCords.push_back(tex);
            }
          //process face
          else if(line[0] == 'f')
            {
              size_t first;
              size_t second;

              triangle_t tri;

              tri.material = currMaterial;

              first = line.find("/");
              second = line.find("/", first+1);


              if(second - first == 1) //then there isn't texture data
                {
                  sscanf(line.c_str(), "%*s %d//%d %d//%d %d//%d" , &tri.a, &tri.normalIdxA, &tri.b, &tri.normalIdxB, &tri.c, &tri.normalIdxC);

                }
              else //there is texture data
                {
                  sscanf(line.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d" , &tri.a, &tri.texA, &tri.normalIdxA, &tri.b, &tri.texB, &tri.normalIdxB, &tri.c, &tri.texC, &tri.normalIdxC);

                }
              //cout << "Triangle: " << tri.a << ", " << tri.b << ", " << tri.c << endl;
              //cout << line << endl;

              triangles.push_back(tri);

            }
        }
    }
}



// Code to read material file
void loadMaterials()
{
  ifstream ifs("jet.mtl");
  if (!ifs) return;
  cout.precision(4);
  Material temp = Material();
  while(ifs)
    {
      string header;
      ifs >> header;
      
     if (header == string("Ns"))
        {
          float Ns;
          ifs >> Ns;
          temp. mtl_index=n;
		  //cout << header << " " << Ns << endl;
        }
      else if (header == string("Tr"))
        {
            float Tr;
            ifs >> Tr;
            temp.Tr = Tr;
        }
        else if (header == string("Ka"))
        {
            float Ka[3];
            ifs >> Ka[0] >> Ka[1] >> Ka[2];
            temp.Ka = RGB(Ka[0], Ka[1], Ka[2]);
        }
        else if (header == string("Kd"))
        {
            float Kd[3];
            ifs >> Kd[0] >> Kd[1] >> Kd[2];
            temp.Kd = RGB(Kd[0], Kd[1], Kd[2]);
        }
        else if (header == string("Ks"))
        {
            float Ks[3];
            ifs >> Ks[0] >> Ks[1] >> Ks[2];
            temp.Ks = RGB(Ks[0], Ks[1], Ks[2]);
        }
        else if (header == string("Ke"))
        {
            float Ke[3];
            ifs >> Ke[0] >> Ke[1] >> Ke[2];
            temp.Ke = RGB(Ke[0], Ke[1], Ke[2]);
            
        else if (header == string("newmtl"))
        {
          int n;
          ifs >> n;
          temp. mtl_index=n;
        }
    
            materials.push_back(new Material(temp));
        }
    }
    }
  
}

void setMaterial(int materialId)
{
  float specular[4];
  float ambient[4];
  float diffuse[4];
  float emmisive[4];
  float shiny;
  
  // Ugly but works
  
  specular[0] = materials[materialId].sr;
  specular[1] = materials[materialId].sg;
  specular[2] = materials[materialId].sb;
  specular[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  
  ambient[0] = materials[materialId].ar;
  ambient[1] = materials[materialId].ag;
  ambient[2] = materials[materialId].ab;
  ambient[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  
  diffuse[0] = materials[materialId].dr;
  diffuse[1] = materials[materialId].dg;
  diffuse[2] = materials[materialId].db;
  diffuse[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  
  emmisive[0] = materials[materialId].er;
  emmisive[1] = materials[materialId].eg;
  emmisive[2] = materials[materialId].eb;
  emmisive[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmisive);
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[materialId].shiny);
}

void drawModel()
{
  // Your code to draw the model here
  
  glBegin(GL_TRIANGLES);
  
  for(int i = 0; i < faces.size(); i++)
    {
    
    //set the material 
         setMaterial(faces[i].mtl) ;
      
	// Draw vertices here

    glVertex3f(faces[i].x.x, 
			   faces[i].x.y, 
			   faces[i].x.z) ;
    glVertex3f(faces[i].y.x, 
			   faces[i].y.y, 
			   faces[i].y.z) ;
    glVertex3f(faces[i].z.x, 
			   faces[i].z.y,
			   faces[i].z.z) ;

    glNormal3f(faces[i].x.x, 
			   faces[i].x.y,
			   faces[i].x.z);
    glNormal3f(faces[i].x.x, 
			   faces[i].x.y, 
			   faces[i].x.z);
    glNormal3f(faces[i].x.x, 
			   faces[i].x.y, 
			   faces[i].x.z);
    
    glEnd() ;
  }

}
  
  
  
}


void init(void)
{ // Called from main
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}


double updateRate = 20;

void timer(int)
{
  // Adjust rotation angles as needed here
   if(x_rotation)
    {
        x_angle += 0.5;
        if(x_angle > 360) 
		 x_angle -= 360;
    }
    if(y_rotation)
    {
        y_angle += 0.5;
        if(y_angle > 360) 
		y_angle -= 360;
    }
    if(z_rotation)
    {
        z_angle += 0.5;
        if(z_angle > 360)
		 z_angle -= 360;
    }
  // Then tell glut to redisplay
  glutPostRedisplay();
  // And reset tht timer
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}

void display(void)
{
  //glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 15.0, -25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // rotations and scaling here
   glScalef(1.2, 1.2, 1.2);
	//Put the light model here

    glPushMatrix(); 
    glRotatef(xangle, 1, 0, 0);        // X axis rotation
    glRotatef(yangle, 0, 1, 0);        // Y axis rotation
    glRotatef(zangle, 0, 0, 1);        // Z axis rotation
   // glScalef(scale, scale, scale);  // Set scaling factor
   
  if(wireframe==true)
  {
  	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
  	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  
    // Draw th emodel
    drawModel();
    // Swap the double buffers
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    height = h;
    width = w;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}



void keyboard (unsigned char key, int x, int y) 
{
  // Keystroke processing here
  switch(key)
	{
		case 27:
            exit(0);           
        	break;
		case 'q':
			exit(0);
			break;
		case 'w':
			wireframe = !wireframe;
			break;
		case 'x':
			x_rotation= !x_rotation;
			break;
		case 'y':
			y_rotation = !y_rotation;
			break;
		case 'z':
			z_rotation = !z_rotation;
			break;
		case 's':
            scale -= 0.1;    
        	break;
        case 'S':
            scale += 0.1;     
        	break;
        case 'r':               
            x_angle = 0;
            y_angle = 0;
            z_angle = 0;
        break;
	}
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jet Display");
    loadModel(); // Uncomment when the model reader is coded.
    loadMaterials();  // Uncomment when the materials reader is coded
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc(reshape);
    // Compute the update rate here...
    glutTimerFunc(1000.0 / updateRate, timer, 0);
    glutMainLoop();
    return 0;
}

