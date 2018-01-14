#ifdef __WIN32__
#include <windows.h>
#pragma comment(lib,"lib/fmod.lib")// automatically link to fmod library
#else
#include "../include/wincompat.h"
#endif

#include <GL/glut.h>
#include "../include/fmod.h"
#include "../include/fmod_errors.h"
#include "../header/load_bmp.h"
#include "../header/load_3ds.h"
#include "../header/object.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <iostream>
#define AREA_PANTALLA 0 // Pantalla de Menu Principal
#define AREA_JUEGO  1 // Pantalla de Juego
#define OPCIONES 2 // Pantalla de Opciones
#define INSTRUCCIONES 3 // Pantalla de Instrucciones
#define DIFICULTAD 4 // Pantalla de Dificultad
#define SONIDO 5 // Pantalla de Sonidos
#define AREA_FIN_DE_JUEGO 6
#define NAVE 0
#define ROCA 1

#define PI 3.14159f
#define Frac_Circ 100
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

#define radioArea 15.0f
#define radioNaveEsfera 2.5f
#define radioPremioEsfera 1.0f

#define intervaloTiempoAumentoVelocidadPared 10 //cada 10seg aumenta la velocidad
#define MIN_VAL 0.5
#define result (tan((30*PI)/180))
#define resultCuadrado (tan((45*PI)/180))

#define alturaPared 5
#define inicioParedes 45

#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1

using namespace std;

typedef struct Punto3d
{
    float x;
    float y;
    float z;
} Punto3d;

typedef struct Circulo3d
{
    Punto3d centro;
    float radio;
} Circulo3d;

Circulo3d premio;
Circulo3d nave;



void mouseOperations();
void processMouseEntry(int state);
void processMouse(int button, int state, int x, int y);
int InsidePolygon(float matriz[][2],int N);
void drawString (void * font, char *s, float x, float y, float z);
void drawSun();
int inicializarSonidos();
float distanciaPuntoPlano(float x, float y, float z, float a, float b, float c, float d);
unsigned char *LoadBmp(char *fn, int *wi, int *hi);
void GenerateTextures(char *, int);
void InitialiseTextures(void);
float distanciaX(Punto3d *puntoA,Punto3d *puntoB);
float distanciaY(Punto3d *puntoA,Punto3d *puntoB);
float distancia(Punto3d *puntoA,Punto3d *puntoB);
float distanciaCuadrada(Punto3d *puntoA,Punto3d *puntoB);
bool evaluarContacto(Circulo3d *circ1, Circulo3d *circ2);
bool evaluarContactoCuad(Circulo3d *circ1, Circulo3d *circ2);
void drawString (void * font, char *s, float x, float y, float z);
void dibujarParedCuadrado();
void inicializarVariablesPared();
void dibujarParedCuadrado();
void dibujarParedHexagono();
void dibujarParedPentagono();
void (* vectorFuncion[3])();
void productoVectorial(int randomFuncion, float matriz[][2], int cant);
void (* vectorFuncionPremio[5])(Punto3d coordenada);
void contactoNavePremios(Punto3d coordenada, int premio, int tiempoJuego);
void manejoControlDibujadoParedes(int tiempoJuego);
void datosEnPantalla(int tiempoJuego);
void manejoControlDibujadoPremios(int tiempoJuego);



/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/
float LimiteCampo = inicioParedes;

int vida;
int score;
int banderaImpactoNavePared ;
//********************
GLfloat X;		// Translate screen to x direction (left or right)
GLfloat Y;		// Translate screen to y direction (up or down)
GLfloat Z;		// Translate screen to z direction (zoom in or out)
GLfloat rotX;	// Rotate screen on x axis
GLfloat rotY;	// Rotate screen on y axis
GLfloat rotZ;	// Rotate screen on z axis

GLfloat xRelativo;
GLfloat zRelativo;


// Puntero a los sonidos mp3 usados en el juego
FSOUND_SAMPLE* menuSound = NULL;
FSOUND_SAMPLE* aceleracionNaveSound = NULL;
FSOUND_SAMPLE* invencibilidadSound = NULL;
FSOUND_SAMPLE* juegoSound = NULL;
FSOUND_SAMPLE* vidaExtraSound = NULL;
FSOUND_SAMPLE* velocidadParedReducidaSound = NULL;
FSOUND_SAMPLE* puntoExtraSound = NULL;
FSOUND_SAMPLE* velocidadNaveReducidaSound = NULL;
FSOUND_SAMPLE* impactoParedSound = NULL;
FSOUND_SAMPLE* velocidadParedAumentaSound = NULL;
FSOUND_SAMPLE* finalJuegoSound = NULL;
FSOUND_SAMPLE* velocidadNaveAumentaSound = NULL;

//Variables donde son almacenadas las texturas utilizadas en el juego.
GLuint sunTexture;//sol
GLuint earthTexture;//tierra
GLuint mercuryTexture;//tierra
GLuint moonTexture;//luna
GLuint jupiterTexture;//jupiter
GLuint starsTexture;//estrellas
GLuint starsTexture_I;
GLuint imagenMenu;//puntero al imagen que tendra el menu principal
GLuint menuPrincipal;
GLuint menuOpciones;
GLuint menuSonido;
GLuint menuInstrucciones;
GLuint menuDificultad;
GLuint asteroide;
GLuint menuPrincipal_jugar;
GLuint menuPrincipal_opciones;
GLuint menuPrincipal_salir;
GLuint menuOpciones_dificultad;
GLuint menuOpciones_instrucciones;
GLuint menuOpciones_sonido;
GLuint menuOpciones_volver;
GLuint menuInstrucciones_volver;
GLuint menuSonido_sinSonido;
GLuint menuSonido_sonido;
GLuint menuSonido_volver;
GLuint menuDificultad_dificil;
GLuint menuDificultad_medio;
GLuint menuDificultad_facil;
GLuint menuDificultad_volver;
GLuint menuFinJuego;
GLuint menuFinJuego_nuevaPartida;
GLuint menuFinJuego_salir;


//***************
int dificultadElegida = 1;//1 -> facil(por defecto), 2->normal, 3dificil->
int sonidoElegido = 1; //1-> con sonido(por defecto), 0->sin sonido
//***************

float radiosCuadrados;
static int slices = 10;
static int stacks = 10;
double a;
bool colision;
bool metodoCuadrados;

//**************************************
int banderaNaveDentroPared = 0;
bool keyStates[256]; // Crea un array de 256 booleanos. La misma se utiliza para guardar las teclas presionadas.
bool premioEnPantalla[5];//5 tipos de premios
int timerPremio[5];//5tipos de premios Tiempo en pantalla
int timerPremioActivado[5];//tiempo de efecto del premio
Punto3d coordPremio[5];//Este es 5 es por la cantidad de premios
Punto3d coordPremioRelativo[5];//Este es 5 es por la cantidad de premios
int porcentajeProbabilidad[5] = {10,10,15,15,50}; //Se cargan las probabilidades de cada esfera.
void keyOperations();
int randPremio;
//****************************************************
void esferaVidaExtra(Punto3d coordenada);
void esferaVelocidadReducida(Punto3d coordenada);
void esferaVelocidadNave(Punto3d coordenada);
void esferaPuntosExtras(Punto3d coordenada);
void esferaInvencibilidad(Punto3d coordenada);

Punto3d coordenadaObjeto;
char angle_char[15];
int pantalla = 0;
bool resultInsidePolygon  = false;
bool dentroAreaCircular(Punto3d p);
//***************

static float matrizHexagono[7][2]; //(x,z)
static float matrizCuadrado[5][2];
static float matrizPentagono[6][2];
//static float matrizTriangulo[4][2];
float vectoresPlano[2][3];
float vectoresNormalesPlanos[6][4];//ax+by+cz-d=0
int randomFuncion = 0;
int randomPared = 0;
int tiempoPerdidoAntesDeComenzar = 0;
int tiempoJuegoUnSegAtras = 0;
int tiempoImpactoNavePared = 0;
int banderaPantallaJuego = 0;
int impactoNavePared(int cantPared);
void dibujarCampoDeFuerza(int i);
int banderaEfectoChoqueNave = 1;
//***************
int banderaInvencibilidad = 0;
int banderaScore = 0;
int banderaVida = 0;
int banderaVelocidadNaveDoble = 0;
int banderaVelocidadParedReducida = 0;



// The width and height of your window, change them as you like
int screen_width=1000;
int screen_height=700;


// Flag for rendering as lines or filled polygons
int filling=1; //0=OFF 1=ON


const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// Lighting values
GLfloat  whiteLight[] = { 0.3f, 0.3f, 0.3f, 0.0f };
GLfloat  sourceLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
GLfloat	 lightPos[] = { 14.0f, 2.0f, 3.0f, 0.0f };

bool texturesCreated = false;//a flag that is used to ensure the textures are only loaded once
int tiempoTotalJuego;
int cantidadFigurasPared = 3;
int cantidadLadosPared = 0;
int x_click_mouse = 0;
int y_click_mouse = 0;


