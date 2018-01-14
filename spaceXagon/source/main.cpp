/*
 * To compile this project you must include the following libraries:
 * opengl32.lib,glu32.lib,glut32.lib
 * You need also the header file glut.h in your compiler directory.
 *
 */

/*
 * Spaceships credits:
 * fighter3.3ds - created by: Rene Reiter <renereiter@hotmail.com>
 */

#include "../header/main.h"

/**********************************************************
 *
 * SUBROUTINE dentroAreaCircular()
 *
 * Recibe un punto, calcula la distancia de dicho punto al origen (0,0,0)
 * y luego se verifica que sea menor o igual al radio del Area de juego.
 * Si la distancia es menor o igual al radio, significa que dicho punto
 * se encuentra dentro del area circular de juego.
 *********************************************************/
bool dentroAreaCircular(Punto3d p)
{
    int i;
    int j;
    Punto3d puntoOrigen;
    puntoOrigen.x = 0.0f;
    puntoOrigen.y = 0.0f;
    puntoOrigen.z = 0.0f;
    resultInsidePolygon = false;

    if(distancia(&p,&puntoOrigen) <= radioArea)
    {
        resultInsidePolygon = !resultInsidePolygon;
        return true;
    }
    return false;
}

/**********************************************************
 * SUBROUTINE drawString()
 * Permite imprimir en pantalla una cadena. Recibe el tipo de fuente,
 *una cadena y las coordenas donde se imprimiran dentro de la pantalla.
 *********************************************************/
void drawString (void * font, char *s, float x, float y, float z)
{
    unsigned int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen (s); i++)
        glutBitmapCharacter (font, s[i]);
}


/**********************************************************
 * SUBROUTINE toRadian()
 * Recibe un angulo, y realiza la respectiva conversion a radianes.
 *********************************************************/
float toRadian (GLfloat angle)
{
    float const PIE = atan(1.0)*4.0;

    return(angle * PIE / 180.0);
}


/**********************************************************
 * SUBROUTINE menu()
 * Dibuja el menu de juego en pantalla. Mediante un punto
 * a una imagen (GLuit imagenMenu), el glBindTexture permite
 * que se vayan cargando distintas imagenes según corresponda
 * el menú deseado, pero siempre en el mismo plano.
 *********************************************************/
void pantallaMenu()//Principal()
{
    GLfloat val = 95.0f;
    glColor3f(255,255,255);
    drawString(GLUT_BITMAP_HELVETICA_18,"Informatica 3 - 2014", -2, 4.0, 12.9);//hasta 12.9 nomas muestra
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
        glTranslatef(0.0f,13.0f,12.89f);
        glRotatef(-45,1.0,0.0,0.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, imagenMenu);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(-val, -val, -val);
            glTexCoord2f(1, 0);
            glVertex3f(val, -val, -val);
            glTexCoord2f(1, 1);
            glVertex3f(val, val, -val);
            glTexCoord2f(0, 1);
            glVertex3f(-val, val, -val);
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

/**********************************************************
 * SUBROUTINE movimientoRelativo()
*Rotación de ejes
* Cambio de la orientación de los ejes de referencia mientras se conserva el origen.
* La principal razón para rotar los ejes es que una ecuación dada es mucho más simple
* en el nuevo sistema de coordenadas que en el sistema original.
* Si los ejes originales x y y rotan un ángulo (a), para
* cualquier punto P(x, y), las coordenadas originales (x, y) se convierten en las nuevas
* coordenadas (x ´, y ´), que son:
* x ´ = x cos(a) - y sen(a)
* y´ =  x sen(a) + y cos(a)
* Tanto X como Y, en todo momento se hacen relativo al angulo de rotacion rotX, y de esa forma
* crear la impresion del movimiento adecuado de la nave, siguiendole la camara por su espalda
* en todo momento (en 3ra persona).
 *********************************************************/
void movimientoRelativo()
{
    zRelativo = X * sin(toRadian(rotX)) + Z * cos(toRadian(rotX));
    xRelativo = X * cos(toRadian(rotX)) - Z * sin(toRadian(rotX));

}

/**********************************************************
 * SUBROUTINE movimientoRelativoEsfera()
 * Fuente wikipedia - Movimiento Relativo entre particulas
 * Los vectores de posición (relativa) de la partícula B con respecto a la A
 * y de la A con respecto a la B están definidos por
 * Rba = rb - ra; Rab = ra - rb
 *
 * La función calcula la posición de las esferas respecto al área
 * de juego, el cual es el que se traslada. Mediante esto, las esferas
 * Se ubican "fijamente" sobre el area de juego.
 *********************************************************/
void movimientoRelativoEsfera()
{

    int i;
    for(i=0; i<5; i++)
    {
        coordPremioRelativo[i].z =  zRelativo - coordPremio[i].z;
        coordPremioRelativo[i].x =   xRelativo - coordPremio[i].x;
    }

}

/**********************************************************
 * SUBROUTINE distanciaX(Punto3d *puntoA,Punto3d *puntoB)
 * Calcula la distancia en el eje x entre dos puntos.
 *********************************************************/
float distanciaX(Punto3d *puntoA,Punto3d *puntoB)
{
    float distancia_x = puntoB->x - puntoA->x;
    if(distancia_x < 0)
    {
        return -distancia_x;
    }

    return distancia_x;
}


/**********************************************************
 * SUBROUTINE distanciaY(Punto3d *puntoA,Punto3d *puntoB)
 * Calcula la distancia en el eje y entre dos puntos
 *********************************************************/
float distanciaY(Punto3d *puntoA,Punto3d *puntoB)
{
    float distancia_y = puntoB->y - puntoA->y;
    if(distancia_y < 0)
    {
        return -distancia_y;
    }
    return distancia_y;
}


/**********************************************************
 * SUBROUTINE distanciaZ(Punto3d *puntoA,Punto3d *puntoB)
 * Calcula la distancia en el eje z entre dos puntos
 *********************************************************/
float distanciaZ(Punto3d *puntoA,Punto3d *puntoB)
{
    float distancia_z = puntoB->z - puntoA->z;
    if(distancia_z < 0)
    {
        return -distancia_z;
    }
    return distancia_z;
}

/**********************************************************
 * SUBROUTINE distancia(Punto3d *puntoA,Punto3d *puntoB)
 * Calcula la distancia entre dos puntos
 * Utiliza la formula de distancia entre dos puntos:
 * D = sqrt(((X1-X2)^2)+((y1-y2)^2)+((z1-z2)^2))
 *********************************************************/
float distancia(Punto3d *puntoA,Punto3d *puntoB)
{
    float distancia = sqrt( pow(distanciaX(puntoA,puntoB),2) + pow(distanciaY(puntoA,puntoB),2)+pow(distanciaZ(puntoA,puntoB),2));
    return distancia;
}


/**********************************************************
 * SUBROUTINE distanciaCuadrada(Punto3d *puntoA,Punto3d *puntoB)
 * Calcula la distancia al cuadrado entre dos puntos
 *********************************************************/
float distanciaCuadrada(Punto3d *puntoA,Punto3d *puntoB)
{
    float distanciaC =  pow(distanciaX(puntoA,puntoB),2) + pow(distanciaY(puntoA,puntoB),2) +pow(distanciaZ(puntoA,puntoB),2);
    return distanciaC;
}

/**********************************************************
 * SUBROUTINE evaluarContacto(Circulo3d *circ1, Circulo3d *circ2)
 * Evalua el contacto entre dos circulos usando la distancia regular
 * La distancia entre los centros de ambos circulos, deben ser menor o igual
 * que la suma de la distancia entre sus radios. De esta manera se verifica
 * si hay choque.
 *********************************************************/
bool evaluarContacto(Circulo3d *circ1, Circulo3d *circ2)
{
    if(distancia( &((*circ1).centro),&((*circ2).centro)) <= ((*circ1).radio + (*circ2).radio + 0.005))  //EL 0.005 es temporal, hasta ver
    {
        return true;
    }
    return false;
}

/**********************************************************
 * SUBROUTINE evaluarContactoCuad(Circulo3d *circ1, Circulo3d *circ2)
 * Evalua el contacto entre dos circulos usando la distancia cuadrada
 *********************************************************/
bool evaluarContactoCuad(Circulo3d *circ1, Circulo3d *circ2)
{
    if(distanciaCuadrada( &((*circ1).centro),&((*circ2).centro)) <= radiosCuadrados)
    {
        return true;
    }
    return false;
}

/**********************************************************
 *
 * SUBROUTINE inicializarVariables(void)
 *
 *********************************************************/

void inicializarVariables()
{
    int i;
    //inicializamos las variables globales (en caso que se juegue una nueva partida)
    vida = 3; //variable global, encargado de contabilizar la vida. Se establece un valor inicial.
    score = 0; //puntaje obtenido durante la partida
    LimiteCampo = inicioParedes; //establece la posicion inicial de las paredes. A medida que transcurre el tiempo
                                //esta variable va descontandose, produciendo asi el movimiento de las paredes.
    banderaImpactoNavePared = 0; //esta bandera indica si se impacto o no con la pared. La misma es utilizada para
                                //que tras detectar la colision con la pared, te descuente una sola vez la vida.
                                //Puesto que la colision se hace distancia entre un punto y un plano (pared),
                                //la colision es detectada hasta que las distancias ya no cumplan con la condicion.
    X = 0.0f;		// Traslada la pantalla sobre el eje X(left or right)
    Y = 0.0f;		// Traslada la pantalla sobre el eje Y (up or down)
    Z = 0.0f;		// Traslada la pantalla sobre el eje Z (zoom in or out)
    rotX = 0.0f;	// Rota la pantalla sobre el eje X
    rotY = 0.0f;	// Rota la pantalla sobre el eje Y
    rotZ = 0.0f;	// Rota la pantalla sobre el eje Z

    xRelativo = 0.0f;//esta variable se utiliza para obtener la coordenada relativa en X dependiendo de la rotacion rotX
    zRelativo = 0.0f;//esta variable se utiliza para obtener la coordenada relativa en Z dependiendo de la rotacion rotX
    banderaNaveDentroPared = 0; //esta variable es utilizada para detectar si la nave esta dentro o fuera de las paredes,
                                //para que de esa forma la distancia entre el punto (centro de la nave) y los planos(paredes)
                                //sean calculadas solo si la nave se encuentra entre las paredes.
                                //El error que producia sin esta variable es que cuando la nave salia de entre las paredes,
                                //podia llegar a un punto donde satisfacia la ecuacion del plano, por lo que detectaba como
                                //colision, cuando en realidad la nave estaba fuera del alcance de las paredes.

    for(i=0;i<5;i++){ //Este es 5 es por la cantidad de premios
        timerPremio[i] = 0;  //en este array se almacena el tiempo en el que un premio es sorteado y dibujado en pantalla
                            //mediante esto, se puede dar un lapso de tiempo de permanencia en el area de juego, tras lo cual
                            //desaparece si no es agarrado
        timerPremioActivado[i] = 0;//en este array se almacena el tiempo en el que un premio es agarrado por la nave.Es
                                    //utilizado para hacer un control del tiempo o duracion que deberia tener el efecto de dicho
                                    //premio.
        premioEnPantalla[i] = false;//indica si un premio esta o no activado (dibujado en pantalla)
    }
    randomFuncion = 0; //numero aleatorio generado, representante de una funcion de dibujado de pared.
    randomPared = 0;    //numero aleatorio generado, representante de una pared de la figura elegida aleatoriamente anteriormente,
                        //indicando asi la pared que no será dibujada.
    tiempoPerdidoAntesDeComenzar = 0;//se guarda el tiempo transcurrido desde el momento en que se ejecuta el juego
                                    //hasta empezar el juego. Mediante esto, al iniciar el juego, el tiempo aparece en cero,
                                    //pues de lo contrario, el tiempo ya tendria un valor porque depende del clock del sistema.
    tiempoJuegoUnSegAtras = 0;//de manera a aumentar el score cierta cantidad por segundo, fue necesario detectar el transcurso
                            //de un segundo. En esta variable se guarda el segundo anterior.
    tiempoImpactoNavePared = 0;//almacena el momento en el que se produce el impacto entre la nave y la pared. De esta forma
                            //se puede establecer un lapso de tiempo, durante el cual se le da un efecto de color a la nave
                            //representando un impacto.
    banderaPantallaJuego = 0;//se activa al entrar al juego, de esta forma se hace el calculo entre el tiempo perdido y el tiempo
                            //de inicio en cero.

    banderaEfectoChoqueNave = 1;//Se utiliza en la funcion de dibujado de la nave, es para darle el efecto de impacto.
    banderaInvencibilidad = 0;//bandera que indica si el modo invicibilidad fue activado. La misma es usada para aplicar el efecto
                            //de invulnerabilidad antes los choques con las paredes
    banderaScore = 0;//bandera utilizada para que el score se contabilice una sola vez luego de comer la esfera respectiva
    banderaVida = 0;//bandera utilizada para que la vida se contabilice una sola vez luego de comer la esfera respectiva
    banderaVelocidadNaveDoble = 0;//bandera utilizada para aplicar el efecto de aumentar la velocidad de la nave.
    banderaVelocidadParedReducida = 0;//bandera utilizada para aplicar el efecto de reducir la velocidad de las paredes.

    cantidadFigurasPared = 3; //cantidad de figuras de paredes a dibujar. En este caso hay tres: paredes cuadradas, hexagonales
                             // y pentagonales
    cantidadLadosPared = 0; //dependiendo de la figura de la pared, la cantidad de lados varia para dicha figura

//Inicializamos el punto y el circulo
    premio.centro.x = 0.0f; //Ubica las esferas en el centro del plano
    premio.centro.y = 0.0f;
    premio.centro.z = 0.0f;
    premio.radio = radioPremioEsfera;

    nave.centro.x = 0.0f; //Ubica la nave en el centro del plano
    nave.centro.y = 0.0f;
    nave.centro.z = 0.0f;
    nave.radio = radioNaveEsfera;

    //Se inicializa la variable de colision
    colision = false;
    //Se inicializa la optimizacion de radios cuadrados
    metodoCuadrados = false;
    radiosCuadrados = pow((premio.radio + nave.radio),2);

    vectorFuncion[2] = &dibujarParedHexagono;
    vectorFuncion[1] = &dibujarParedPentagono;
    vectorFuncion[0] = &dibujarParedCuadrado;
    vectorFuncionPremio[0] = &esferaVidaExtra; //VidaExtra
    vectorFuncionPremio[1] = &esferaInvencibilidad; //Invencibilidad
    vectorFuncionPremio[2] = &esferaVelocidadNave;//VelocidadNAve
    vectorFuncionPremio[3] = &esferaVelocidadReducida;//VelocidadParedReducida
    vectorFuncionPremio[4] = &esferaPuntosExtras;//PuntosExtras
}

/**********************************************************
 *
 * SUBROUTINE init(void)
 *
 * Used to initialize OpenGL and to setup our world
 *
 *********************************************************/

void init(void)
{
    MatrGenerateLookupTab();

    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black
    glShadeModel (GL_SMOOTH);						// Set the shading model to GL_FLAT
    glEnable (GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //***se ubica la camara, en la posicion  angulo deseado
    glFrustum(-1, 1, -1.0, 1.0, 1.0, 100.0);
    gluLookAt(0,15,13,0,2,0,0,1,0);
    glViewport(0,0,screen_width,screen_height); //tamaño de la ventana

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
    glLoadIdentity(); // We initialize the projection matrix as identity
    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

    // Enable lighting

    glEnable (GL_LIGHTING);

    //Lights initialization and activation
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

    //Materials initialization and activation
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, high_shininess);

    //Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)
    //Objects loading

    ObjLoad ("objetos3d/fighter3.3ds","textura/spaceshiptexture.bmp", 0.0, 0.0, -2.0, 90,0,0);//esta función permite cargar
                                                                                            //el objeto en la nave.

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

// Black blue background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

}



/**********************************************************
 *
 * SUBROUTINE resize(int p_width, int p_height)
 *
 * This routine must be called everytime we resize our window.
 *
 * Input parameters: p_width = width in pixels of our viewport
 *					 p_height = height in pixels of our viewport
 *
 *********************************************************/

void resize (int p_width, int p_height)
{
    if (screen_width==0 && screen_height==0) exit(0);
    screen_width=p_width; // We obtain the new screen width values and store it
    screen_height=p_height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}


/**********************************************************
 *
 * SUBROUTINE drawSpaceship(void)
 * Dibuja el objeto nave en pantalla
 *********************************************************/
void drawSpaceship()
{
    int i,j;
    //i = 0; //se carga el objeto NAVE en [0]
    //
    if(banderaEfectoChoqueNave) glColor3ub(150, 190, 150); //da color a la nave,
    else glColor3d(1,0,0);                                //desactivamos esto por 2seg
                                                          //cuando ocurre un choque para darle efecto de choque.

    glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST);
    glPushMatrix(); // We save the current matrix
    //****Se encarga de darle un efecto de giro a la nave ya sea al avanzar o rotar
    if(keyStates['j']||keyStates['J']) glRotatef(5,0,0,1);
    if(keyStates['l']||keyStates['L']) glRotatef(-5,0,0,1);
    if(keyStates['a']||keyStates['A']) glRotatef(5,0,1,0);
    if(keyStates['d']||keyStates['D']) glRotatef(-5,0,1,0);
    if(keyStates['w']||keyStates['W']) glRotatef(5,1,0,0);
    if(keyStates['s']||keyStates['S']) glRotatef(-5,1,0,0);
    glScalef(0.5f,0.5f, 0.5f);
    glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the identity-first matrix
    if (object[NAVE].id_texture!=-1)
    {
        glBindTexture(GL_TEXTURE_2D, object[NAVE].id_texture); // We set the active texture
        glEnable(GL_TEXTURE_2D); // Texture mapping ON
    }
    else
        glDisable(GL_TEXTURE_2D); // Texture mapping OFF

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (j=0; j<object[NAVE].polygons_qty; j++)
    {
        //----------------- FIRST VERTEX -----------------
        //Normal coordinates of the first vertex
        glNormal3f( object[NAVE].normal[ object[NAVE].polygon[j].a ].x,
                    object[NAVE].normal[ object[NAVE].polygon[j].a ].y,
                    object[NAVE].normal[ object[NAVE].polygon[j].a ].z);
        // Texture coordinates of the first vertex
        glTexCoord2f( object[NAVE].mapcoord[ object[NAVE].polygon[j].a ].u,
                      object[NAVE].mapcoord[ object[NAVE].polygon[j].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object[NAVE].vertex[ object[NAVE].polygon[j].a ].x,
                    object[NAVE].vertex[ object[NAVE].polygon[j].a ].y,
                    object[NAVE].vertex[ object[NAVE].polygon[j].a ].z);

        //----------------- SECOND VERTEX -----------------
        //Normal coordinates of the second vertex
        glNormal3f( object[NAVE].normal[ object[NAVE].polygon[j].b ].x,
                    object[NAVE].normal[ object[NAVE].polygon[j].b ].y,
                    object[NAVE].normal[ object[NAVE].polygon[j].b ].z);
        // Texture coordinates of the second vertex
        glTexCoord2f( object[NAVE].mapcoord[ object[NAVE].polygon[j].b ].u,
                      object[NAVE].mapcoord[ object[NAVE].polygon[j].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object[NAVE].vertex[ object[NAVE].polygon[j].b ].x,
                    object[NAVE].vertex[ object[NAVE].polygon[j].b ].y,
                    object[NAVE].vertex[ object[NAVE].polygon[j].b ].z);

        //----------------- THIRD VERTEX -----------------
        //Normal coordinates of the third vertex
        glNormal3f( object[NAVE].normal[ object[NAVE].polygon[j].c ].x,
                    object[NAVE].normal[ object[NAVE].polygon[j].c ].y,
                    object[NAVE].normal[ object[NAVE].polygon[j].c ].z);
        // Texture coordinates of the third vertex
        glTexCoord2f( object[NAVE].mapcoord[ object[NAVE].polygon[j].c ].u,
                      object[NAVE].mapcoord[ object[NAVE].polygon[j].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object[NAVE].vertex[ object[NAVE].polygon[j].c ].x,
                    object[NAVE].vertex[ object[NAVE].polygon[j].c ].y,
                    object[NAVE].vertex[ object[NAVE].polygon[j].c ].z);

    }
    glEnd();
    glDisable(GL_CULL_FACE); // Enable the back face culling
    glDisable(GL_DEPTH_TEST);
    glPopMatrix(); // Restore the previous matrix
    glColor3ub(150, 190, 150);//permite que las paredes mantengan su color, por mas que el color de la nave cambie.

}

/**********************************************************
 *
 * SUBROUTINE drawCirculo(void)
 * Dibuja un circulo en pantalla, representando el area donde la
 * nave se puede mover.
 *********************************************************/
void drawCirculo()
{
    int i;
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glColor3d(255,0,0);
    glRotatef(rotX,0.0,1.0,0.0);// Rotate on x
    glTranslatef(xRelativo, 0, zRelativo);
    glPointSize(3.0); //punto de tamanho 3px
    glBegin(GL_POINTS);//el circulo se transforma en un perimetro de puntos

    for (i = 0; i < Frac_Circ + 1; i++)    // +1 para cerrar
    {
        glVertex3f( 0 + (radioArea+0.5f) * cos(2.0 * PI * i / Frac_Circ),0,
                    0 + (radioArea+0.5f) * sin(2.0 * PI * i / Frac_Circ) );
    }

    glEnd();
    glPopMatrix();
    glDisable(GL_LIGHTING);
}

/**********************************************************
 *
 * SUBROUTINE drawEsfera(void)
 * Se dibuja una esfera en pantalla, se lo ubica en el origen.
 * Mediante esta esfera, se observa el radio que tendria la nave
 * al momento de evaluar las colisiones tanto con las paredes como
 * con los premios.
 *********************************************************/
void drawEsfera()
{
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glRotatef(90.0,0.0,1.0,0.0);
    glutSolidSphere(radioNaveEsfera,20,20);
    glPopMatrix();
    glDisable(GL_LIGHTING);
}


/**********************************************************
 * SUBROUTINE esferaVelocidadReducida(void)
 * Dibuja una esfera, representando el premio cuyo efecto
 * ralentiza la velocidad de las paredes.
 *********************************************************/
void esferaVelocidadReducida(Punto3d coordenada)
{
// Esfera con grillas
   glColor3d(0,0,205); //AZUL
    glPushMatrix();
        glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(radioPremioEsfera,slices,stacks);
    glPopMatrix();

    // toro solido
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.9,10,16);//radio 0.1
    glPopMatrix();

    // Toro Superior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2+0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Toro inferior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2-0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Esfera solida
    glColor3d(1,1,1); //BLANCO
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere((radioPremioEsfera-0.6),slices,stacks);//radio 0.4
    glPopMatrix();


}

/**********************************************************
 * SUBROUTINE esferaInvencibilidad(void)
 * Dibuja una esfera, representando el premio cuyo efecto
 * es invencibilidad, permitiendo choques contra la pared sin
 * efectos colaterales.
 *********************************************************/
void esferaInvencibilidad(Punto3d coordenada)
{
// Esfera con grillas
   glColor3d(1,1,1); //BLANCO
    glPushMatrix();
         glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(radioPremioEsfera,slices,stacks);
    glPopMatrix();

    // toro solido
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera - 0.9),0.9,10,16);//radio 0.1
    glPopMatrix();

    // Toro Superior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2+0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera - 0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Toro inferior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2-0.4,coordenada.z);
        //glTranslated(-2.4,1.6,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera - 0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Esfera solida
    glColor3d(1,0,1);
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        //glTranslated(-2.4,1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere((radioPremioEsfera - 0.6),slices,stacks);//radio 0.4
    glPopMatrix();

}

/**********************************************************
 * SUBROUTINE esferaPuntosExtras(void)
 * Dibuja una esfera, representando el premio cuyo efecto
 * es obtener puntos extras.
 *********************************************************/
void esferaPuntosExtras(Punto3d coordenada)
{
// Esfera con grillas
   //glColor3d(5,1,0);//amarillo
   glColor3d(255,0,255); //ROSA
    glPushMatrix();
        glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(radioPremioEsfera,slices,stacks);
    glPopMatrix();

    // toro solido
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.9,10,16);//radio 0.1
    glPopMatrix();

    // Toro Superior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2+0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Toro inferior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2-0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Esfera solida
    glColor3d(1,1,1);//BLANCO
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere((radioPremioEsfera-0.6),slices,stacks);//radio 0.4
    glPopMatrix();
}

/**********************************************************
 * SUBROUTINE esferaVidaExtra(void)
 * Dibuja una esfera, representando el premio cuyo efecto
 * es obtener vida extra.
 *********************************************************/
void esferaVidaExtra(Punto3d coordenada)
{
// Esfera con grillas
   glColor3d(1,0,0); //ROJO
    glPushMatrix();
        glRotatef(rotX,0.0,1.0,0.0);
        glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(radioPremioEsfera,slices,stacks);
    glPopMatrix();

    // toro solido
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.9,10,16);//radio 0.1
    glPopMatrix();

    // Toro Superior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2+0.4,coordenada.z);
        //glTranslated(-2.4,0.8,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Toro inferior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2-0.4,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Esfera solida
    glColor3d(1,1,1);
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere((radioPremioEsfera-0.6),slices,stacks);//radio 0.4
    glPopMatrix();
}

/**********************************************************
 * SUBROUTINE esferaVelocidadNave(void)
 * Dibuja una esfera, representando el premio cuyo efecto
 * es velocidad aumentada X2 de la nave.
 *********************************************************/
void esferaVelocidadNave(Punto3d coordenada)
{
// Esfera con grillas
   glColor3d(0,1,0); //VERDE
    glPushMatrix();
         glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(radioPremioEsfera,slices,stacks);
    glPopMatrix();

    // toro solido
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.9,10,16);//radio 0.1
    glPopMatrix();

    // Toro Superior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2+0.4,coordenada.z);
        //glTranslated(-2.4,0.8,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Toro inferior
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2-0.4,coordenada.z);
        //glTranslated(-2.4,1.6,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus((radioPremioEsfera-0.9),0.6,10,16);//radio 0.1
    glPopMatrix();

    // Esfera solida
    glColor3d(1,1,1);
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
         glScalef(2.0f,2.0f, 2.0f);
        glTranslated(coordenada.x,-1.2,coordenada.z);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere((radioPremioEsfera-0.6),slices,stacks);//radio 0.4
    glPopMatrix();


}


/**********************************************************
 *
 * SUBROUTINE drawEarth(void)
 * Dibujan dos esferas, a las cuales se le aplican texturas
 * representando la tierra y la luna. La tierra gira alrededor
 * de su eje; pero, la luna gira alrededor de su eje y a la vez
 * alrededor de la tierra.
 *********************************************************/
void drawEarth()
{
    GLUquadricObj* pObj;
    static float fEarthRot = 0.0f;
    static float fMoonRot = 0.0f;

    //MOON
    glPushMatrix();
        glRotatef(rotX,0.0,1.0,0.0);
        pObj = gluNewQuadric();
        gluQuadricTexture(pObj,GL_TRUE);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, moonTexture); //sets the texture to the moon
        glTranslatef((xRelativo/4) + 20.0f,-5.0f,(zRelativo/4) - 0.0f);//moves away and out from the earth
        glRotatef(fMoonRot,0.0f, 1.0f, 0.0f); //rotates the moon around the earth
        glTranslatef(30.0f, 0.0f, 0.0f); //moves away and out from the earth
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f); //Rotates the moon on its spot, fixing the texture display
        glRotatef(fMoonRot * 3, 0.0f, 0.0f, 1.0f); //Rotates the moon on its spot, simulating the planet spinning
        gluSphere(pObj, 4.0f, 50, 50); //draws the sphere for the moon
        glDisable(GL_TEXTURE_2D);
        //gets the new rotation for the moon
        fMoonRot+= 0.05f;
        //resets the rotation
        if(fMoonRot >= 360.0f)
            fMoonRot = 0.0f;

        gluDeleteQuadric(pObj);
        // Restore the matrix state
    glPopMatrix();	// Modelview matrix

    //Earth
    glPushMatrix();
        glRotatef(rotX,0.0,1.0,0.0);
        pObj = gluNewQuadric();
        gluQuadricTexture(pObj,GL_TRUE);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, earthTexture);

        // Draw the Earth
        glTranslatef(xRelativo-(-35.0f),-10.0f,zRelativo-(-20.0f));//moves the position out from the sun
        glRotatef(160.0f, 1.0f, 0.0f, 1.0f); //Rotates the Earth on its spot, fixing the texture display
        glRotatef(90.0f, 1.0f, 0.0f, 1.0f);
        glRotatef(fEarthRot * 3, 0.0f, 0.0f, 1.0f); //Rotates the Earth on its spot, simulating the planet spinning
        gluSphere(pObj, 16.0f, 50, 50); //draws the sphere for the earth
        glDisable(GL_TEXTURE_2D);

        gluDeleteQuadric(pObj); //frees pObj
    glPopMatrix();	// Modelview matrix

    // Step earth orbit 5 degrees
    fEarthRot += 0.004;
    //resets the rotation value
    if(fEarthRot >= 360.0f)
        fEarthRot = 0.0f;


}


/**********************************************************
 *
 * SUBROUTINE skybox(void)
 *
 *********************************************************/
void skybox()
{
    // Store the current matrix
    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
    glTranslatef(0.0f,-10.0f,0.0f);
    glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);

    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
    GLfloat val = 40.0f; //tamaño de los lados del cubo

    // Render the techo quad
    glBindTexture(GL_TEXTURE_2D, starsTexture); //aplica textura
    glBegin(GL_QUADS); //dibuja los planos
        glTexCoord2f(0, 0);
        glVertex3f(val, -val, val);
        glTexCoord2f(1, 0);
        glVertex3f(-val, -val, val);
        glTexCoord2f(1, 1);
        glVertex3f(-val, val, val);
        glTexCoord2f(0, 1);
        glVertex3f(val, val, val);
    glEnd();

    // Render the derecha quad
    glBindTexture(GL_TEXTURE_2D, starsTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(val, -val, -val);
        glTexCoord2f(1, 0);
        glVertex3f(val, -val, val);
        glTexCoord2f(1, 1);
        glVertex3f(val, val, val);
        glTexCoord2f(0, 1);
        glVertex3f(val, val, -val);
    glEnd();

    // Render the piso quad
    glBindTexture(GL_TEXTURE_2D, starsTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-val, -val, -val);
        glTexCoord2f(1, 0);
        glVertex3f(val, -val, -val);
        glTexCoord2f(1, 1);
        glVertex3f(val, val, -val);
        glTexCoord2f(0, 1);
        glVertex3f(-val, val, -val);
    glEnd();

    // Render the piso quad
    glBindTexture(GL_TEXTURE_2D, starsTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-val, val, -val);
        glTexCoord2f(1, 0);
        glVertex3f(-val, val, val);
        glTexCoord2f(1, 1);
        glVertex3f(-val, -val, val);
        glTexCoord2f(0, 1);
        glVertex3f(-val, -val, -val);
    glEnd();

    // Render the frente quad
    glBindTexture(GL_TEXTURE_2D, starsTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(val, val, -val);
        glTexCoord2f(1, 0);
        glVertex3f(val, val, val);
        glTexCoord2f(1, 1);
        glVertex3f(-val, val, val);
        glTexCoord2f(0, 1);
        glVertex3f(-val, val, -val);
    glEnd();

    // Render the atras quad
    glBindTexture(GL_TEXTURE_2D, starsTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-val, -val, -val);
        glTexCoord2f(1, 0);
        glVertex3f(-val, -val, val);
        glTexCoord2f(1, 1);
        glVertex3f(val, -val, val);
        glTexCoord2f(0, 1);
        glVertex3f(val, -val, -val);
    glEnd();

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}

/**********************************************************
 *
 * SUBROUTINE drawSun(void)
 * Dibuja una esferita a la cual se le aplica una textura
 * aparentando ser el sol.
 *********************************************************/
void drawSun()
{
    GLUquadricObj* pObj;
    // Sun

    glPushMatrix();
    glRotatef(rotX,0.0,1.0,0.0);
    // Translate the whole scene out and into view	based on the Sun's position
    glTranslatef(0.0f, -20.0f, 80.0f);
    glDisable(GL_LIGHTING);
    pObj = gluNewQuadric(); //creates new quadric for pObj
    gluQuadricTexture(pObj,GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sunTexture); //sets texture to the sun texture

    //Rotate the Sun on its centre
    glRotatef(0, 1.0f, 0.0f, 0.0f); //rotates Sun along X axis
    glRotatef(0, 0.0f, 1.0f, 0.0f); //rotates Sun along Y axis
    glRotatef(0, 0.0f, 0.0f, 1.0f); //rotates Sun along Z axis
    gluSphere(pObj, 5.0f, 50, 50); //draw sphere for the sun
    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(pObj); //frees pObj
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

/**********************************************************
 *
 * SUBROUTINE dibujarParedHexagono(void)
 *
 *********************************************************/
void dibujarParedHexagono()
{
    int i = 0;
    double tiempoAnguloGiroPared = -(a/3);
    for (i = 0; i < 6; i++)
    {
        if( i != randomPared)
        {
            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            //glRotatef(tiempoAnguloGiroPared,0.0,1.0,0.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, asteroide);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex3f(matrizHexagono[i][0],0,matrizHexagono[i][1]);
            glTexCoord2f(2, 0);glVertex3f(matrizHexagono[i][0],alturaPared,matrizHexagono[i][1]);
            glTexCoord2f(2, 2);glVertex3f(matrizHexagono[i+1][0],alturaPared,matrizHexagono[i+1][1]);
            glTexCoord2f(0, 2);glVertex3f(matrizHexagono[i+1][0],0,matrizHexagono[i+1][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();

            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, asteroide);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex3f(matrizHexagono[i+1][0],0,matrizHexagono[i+1][1]);
            glTexCoord2f(1, 0);glVertex3f(matrizHexagono[i+1][0],alturaPared,matrizHexagono[i+1][1]);
            glTexCoord2f(1, 1);glVertex3f(matrizHexagono[i][0],alturaPared,matrizHexagono[i][1]);
            glTexCoord2f(0, 1);glVertex3f(matrizHexagono[i][0],0,matrizHexagono[i][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();
        }
    }
}

/**********************************************************
 *
 * SUBROUTINE dibujarParedCuadrado(void)
 *
 *********************************************************/
void dibujarParedCuadrado()
{
    int i =0;
    double tiempoAnguloGiroPared = a/5;

    for (i = 0; i < 4; i++)
    {
        if( i != randomPared)
        {
            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            //glRotatef(tiempoAnguloGiroPared,0.0,1.0,0.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, asteroide);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(matrizCuadrado[i][0],0,matrizCuadrado[i][1]);
            glTexCoord2f(1, 0);
            glVertex3f(matrizCuadrado[i][0],alturaPared,matrizCuadrado[i][1]);
            glTexCoord2f(1, 1);
            glVertex3f(matrizCuadrado[i+1][0],alturaPared,matrizCuadrado[i+1][1]);
            glTexCoord2f(0, 1);
            glVertex3f(matrizCuadrado[i+1][0],0,matrizCuadrado[i+1][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();

            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            //(tiempoAnguloGiroPared,0.0,1.0,0.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, asteroide);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(matrizCuadrado[i+1][0],0,matrizCuadrado[i+1][1]);
            glTexCoord2f(1, 0);
            glVertex3f(matrizCuadrado[i+1][0],alturaPared,matrizCuadrado[i+1][1]);
            glTexCoord2f(1, 1);
            glVertex3f(matrizCuadrado[i][0],alturaPared,matrizCuadrado[i][1]);
            glTexCoord2f(0, 1);
            glVertex3f(matrizCuadrado[i][0],0,matrizCuadrado[i][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();
        }
    }
}
/**********************************************************
 *
 * SUBROUTINE dibujarParedPentagono(void)
 *
 *********************************************************/
void dibujarParedPentagono()
{
    int i = 0;
    double tiempoAnguloGiroPared = a/4;

    for (i = 0; i < 5; i++)
    {
        if( i != randomPared)
        {
            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            //glRotatef(tiempoAnguloGiroPared,0.0,1.0,0.0);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, asteroide);

            glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex3f(matrizPentagono[i][0],0,matrizPentagono[i][1]);
            glTexCoord2f(1, 0);glVertex3f(matrizPentagono[i][0],alturaPared,matrizPentagono[i][1]);
            glTexCoord2f(1, 1);glVertex3f(matrizPentagono[i+1][0],alturaPared,matrizPentagono[i+1][1]);
            glTexCoord2f(0, 1);glVertex3f(matrizPentagono[i+1][0],0,matrizPentagono[i+1][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();

            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
            glRotatef(rotX,0.0,1.0,0.0);
            glTranslatef(xRelativo, 0, zRelativo);
            //glRotatef(tiempoAnguloGiroPared,0.0,1.0,0.0);
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, asteroide);
            glBegin(GL_QUADS);

            glTexCoord2f(0, 0);glVertex3f(matrizPentagono[i+1][0],0,matrizPentagono[i+1][1]);
            glTexCoord2f(1, 0);glVertex3f(matrizPentagono[i+1][0],alturaPared,matrizPentagono[i+1][1]);
            glTexCoord2f(1, 1);glVertex3f(matrizPentagono[i][0],alturaPared,matrizPentagono[i][1]);
            glTexCoord2f(0, 1);glVertex3f(matrizPentagono[i][0],0,matrizPentagono[i][1]);
            glEnd();
            glPopAttrib();
            glPopMatrix();
        }
    }
}

/**********************************************************
 *
 * SUBROUTINE inicializarVariablesPared(void)
 *Asignamos estaticamente las coordenadas de las distintas paredes
 *********************************************************/
void inicializarVariablesPared()
{
    int i,j = 0;

    //HEXAGONO
    matrizHexagono[0][0] = result*LimiteCampo; //1
    matrizHexagono[0][1] = -LimiteCampo;
    matrizHexagono[1][0] = LimiteCampo; //2
    matrizHexagono[1][1] = 0.0f;
    matrizHexagono[2][0] = result*LimiteCampo;
    matrizHexagono[2][1] = LimiteCampo;
    matrizHexagono[3][0] = -result*LimiteCampo;
    matrizHexagono[3][1] = LimiteCampo;
    matrizHexagono[4][0] = -LimiteCampo;
    matrizHexagono[4][1] = 0.0f;
    matrizHexagono[5][0] = -result*LimiteCampo;
    matrizHexagono[5][1] = -LimiteCampo;
    matrizHexagono[6][0] = result*LimiteCampo; //1
    matrizHexagono[6][1] = -LimiteCampo;

    //CUADRADO
    matrizCuadrado[0][0] = -resultCuadrado*LimiteCampo;
    matrizCuadrado[0][1] = -LimiteCampo;
    matrizCuadrado[1][0] = -resultCuadrado*LimiteCampo;
    matrizCuadrado[1][1] = LimiteCampo;
    matrizCuadrado[2][0] = resultCuadrado*LimiteCampo;
    matrizCuadrado[2][1] = LimiteCampo;
    matrizCuadrado[3][0] = resultCuadrado*LimiteCampo;
    matrizCuadrado[3][1] = -LimiteCampo;
    matrizCuadrado[4][0] = -resultCuadrado*LimiteCampo;
    matrizCuadrado[4][1] = -LimiteCampo;

    //PENTAGONO
    matrizPentagono[0][0] = -result*LimiteCampo;
    matrizPentagono[0][1] = LimiteCampo;
    matrizPentagono[1][0] = -LimiteCampo;
    matrizPentagono[1][1] = 0.0f;
    matrizPentagono[2][0] = 0.0f;
    matrizPentagono[2][1] = -LimiteCampo;
    matrizPentagono[3][0] = LimiteCampo;
    matrizPentagono[3][1] = 0.0f;
    matrizPentagono[4][0] = result*LimiteCampo;
    matrizPentagono[4][1] = LimiteCampo;
    matrizPentagono[5][0] = -result*LimiteCampo;
    matrizPentagono[5][1] = LimiteCampo;

    /*matrizTriangulo[0][0] = -result*LimiteCampo;
    matrizTriangulo[0][1] = LimiteCampo;
    matrizTriangulo[1][0] = 0.0f;
    matrizTriangulo[1][1] = -LimiteCampo;
    matrizTriangulo[2][0] = result*LimiteCampo;
    matrizTriangulo[2][1] = LimiteCampo;
    matrizTriangulo[3][0] = -result*LimiteCampo;
    matrizTriangulo[3][1] = LimiteCampo;*/
}

/**********************************************************
 *
 * SUBROUTINE randomUbicacion(void)
 *  Aleatoriamente elige un punto dentro de los parámetros del area
 *********************************************************/
Punto3d randomUbicacion()  //con rand se elige una x y una z dentro del espacio limitado.
{

    int x,z,signo_x, signo_z;
    //srand(time(NULL));
    Punto3d punto;
    punto.x = rand()%((int)radioArea);
    punto.y = 0;
    punto.z = rand()%((int)radioArea);

    signo_x = rand()%(2);
    signo_z = rand()%(2);

    if (!signo_x) punto.x = -punto.x;
    if (!signo_z) punto.z = -punto.z;
    return punto;
}

/**********************************************************
 *
 * SUBROUTINE display(void)
 *
 *********************************************************/
void display(void)
{
    int i,j;
    char angle_char[15];
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    a = t*90.0;

    char buffer[33];//tiempo de Juego en string para imprimir en pantalla
    itoa(t,buffer,10);
    int tiempoJuego = atoi(buffer) - tiempoPerdidoAntesDeComenzar;//tiempo de juego en int
    srand(time(NULL));
    mouseOperations();
    keyOperations();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(pantalla == AREA_JUEGO )
    {
        if(!sonidoElegido)FSOUND_StopSound(0); //si se eligio la opcion sin sonido, se detiene todo sonido, incluyendo el del menu
        if(!banderaPantallaJuego){ //El tiempoJuego empieza desde cero. Solo se ejecuta una vez, estando en esta area
            tiempoPerdidoAntesDeComenzar = tiempoJuego - 0;
            banderaPantallaJuego = 1;
        }
        if(!tiempoJuegoUnSegAtras){ //calculamos o computamos un seg, para que de esa forma el score aumente cada seg
            tiempoJuegoUnSegAtras = tiempoJuego;
        }

        movimientoRelativo();
        skybox();
        drawEarth();
        drawSun();
        drawCirculo();
        drawEsfera(); //<---Importante, permite visualizar, el radio de la Nave.

        if((tiempoJuego - tiempoJuegoUnSegAtras) == 1){ //Calculamos un seg, para aumentar el score cada seg en 10
                tiempoJuegoUnSegAtras = 0;
                score += 10;
        }
        movimientoRelativoEsfera();
        manejoControlDibujadoPremios(tiempoJuego);
        drawSpaceship();
        manejoControlDibujadoParedes(tiempoJuego);
        datosEnPantalla(tiempoJuego);
        if(vida <= 0){ //controlamos que si la vida es cero, termine el juego
            pantalla = AREA_FIN_DE_JUEGO;
            tiempoTotalJuego = tiempoJuego;
            if(sonidoElegido){
                    FSOUND_StopSound(3);//detenemos la musica del juego
                    FSOUND_PlaySound(10,finalJuegoSound);//hacemos sonar una nueva musica
                    FSOUND_SetLoopMode(10,FSOUND_LOOP_NORMAL);

            }
            imagenMenu = menuFinJuego;

        }


    }else{
        banderaPantallaJuego = 0;//habilitamos de nuevo la bandera para que el tiempo, al entrar al area de juego, empiece em cerp
    }
    if((pantalla == AREA_PANTALLA)||(pantalla == OPCIONES)||(pantalla == SONIDO)||(pantalla == INSTRUCCIONES)||(pantalla == DIFICULTAD)){
            pantallaMenu();
    }
    if(pantalla == AREA_FIN_DE_JUEGO){
            pantallaMenu();
            //Tiramos los puntajes obtenidos y el tiempo de juego
            glColor3f(255,255,255);
            sprintf(angle_char, "%d", score);
            drawString(GLUT_BITMAP_HELVETICA_18,angle_char,  12,-7.5,0);
            glColor3f(255,255,255);
            sprintf(angle_char, "%d seg.", tiempoTotalJuego);
            glColor3f(255,255,255);
            drawString(GLUT_BITMAP_HELVETICA_18,angle_char,  9,1,0);
    }

    glFlush(); // This force the execution of OpenGL commands
    glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer
}

/**********************************************************
 *
 * SUBROUTINE datosEnPantalla(void)
 * Tira el score y la vida en pantalla, durante el juego.
 *********************************************************/
void datosEnPantalla(int tiempoJuego){
        glColor3f(255,255,255);
        sprintf(angle_char, "Score: %d", score);
        drawString(GLUT_BITMAP_HELVETICA_18,angle_char,  -8,14,0);
        glColor3f(255,255,255);
        sprintf(angle_char, "Vida: %d", vida);
        drawString(GLUT_BITMAP_HELVETICA_18,angle_char,  0,14,0);
        sprintf(angle_char, "Tiempo: %d", tiempoJuego);
        glColor3f(255,255,255);
        drawString(GLUT_BITMAP_HELVETICA_18,angle_char,  7,14,0);

}

/**********************************************************
 *
 * SUBROUTINE manejoControlDibujadoPremios(void)
 *********************************************************/
void manejoControlDibujadoPremios(int tiempoJuego){
    Punto3d puntoRandom;
    puntoRandom.x = 0;
    puntoRandom.z = 0;
    puntoRandom.y = 0;
    int i;

        /*for(i=0;i<5;i++){
            if(premioEnPantalla[i])printf("premioEnPantalla[%d] = 1\n",i);
            else printf("premioEnPantalla[%d] = 0\n",i);
            printf("timerPremioActivado[%d] = %d\n",i,timerPremioActivado[i]);
            printf("Tiempo de Juego = %d\n",tiempoJuego);
            printf("timerPremio[%d] = %d\n",i,timerPremio[i]);

        }*/

        for(i=0; i<5; i++) //Loop por los 5 tipos de Premios
        {
            if(premioEnPantalla[i] && ((tiempoJuego - timerPremio[i]) <= 7))  //se verifica si el premio debe ser dibujado (true) y que su lapso dentro del campo no supere los 7seg de dibujado
            {
                vectorFuncionPremio[i](coordPremioRelativo[i]); //se dibuja el premio en pantalla
                contactoNavePremios(coordPremio[i],i,tiempoJuego); //se verifica la colision entre los premios y la nave
            }
            else
            {
                if(premioEnPantalla[i] && ((tiempoJuego - timerPremio[i]) > 7))//si el premio está en pantalla, pero ya expiro su lapso de permanencia en el area,
                {                                                               //se ceran sus valores, tiempos
                    premioEnPantalla[i] = false;
                    timerPremioActivado[i] = 0;
                    timerPremio[i] = 0;
                }

            }
            if(!premioEnPantalla[i] && (timerPremioActivado[i]!= 0)) // si el premio no esta activado (false) pero ya se activo el timerPremioActivado,
            {                                                           //luego de ser agarrado por la nave, se procede a activar su efecto (el producido por el premio)
                //activar el premio
                switch(i)
                {
                case 0: //vida extra Aumentar variable
                    if(banderaVida){//bandera para que se coma una sola vez la vida
                            vida++;
                            banderaVida = 0;
                    }
                    if((tiempoJuego - timerPremioActivado[i]) > 1 ){
                            timerPremioActivado[i] = 0; //el efecto del premio, luego de la colision expiro. En pantalla se dibuja un "+1"
                    }
                    glColor3f(255,255,255);
                    drawString(GLUT_BITMAP_HELVETICA_18,"+1",1,13,0);
                    break;
                case 1: //invencibilidad
                    glColor3d(1,1,1);//blanco
                    dibujarCampoDeFuerza(i);
                    if((tiempoJuego - timerPremioActivado[i]) > 5 ){ //5seg de duracion del efecto
                            if(sonidoElegido)FSOUND_StopSound(2);//parar la musica de invencibilidad
                            timerPremioActivado[i] = 0; //el efecto del premio, luego de la colision expiro tras los 5 seg.
                            banderaInvencibilidad = 0; //se desactiva el efecto de invencibilidad
                    }
                    break;
                case 2: //veloc movil x2
                    glColor3d(0,1,0);//verde
                    dibujarCampoDeFuerza(i);
                    if((tiempoJuego - timerPremioActivado[i]) > 5 ) {//5seg de duracion del efecto
                            if(sonidoElegido)FSOUND_StopSound(11);
                            timerPremioActivado[i] = 0; //el efecto del premio, luego de la colision expiro tras los 5 seg.
                            banderaVelocidadNaveDoble = 0;//se desactiva la bandera de la veloc X2
                    }
                    break;
                case 3: //veloc pared reducida x2
                    glColor3d(0,0,205);//azul
                    dibujarCampoDeFuerza(i);
                    if((tiempoJuego - timerPremioActivado[i]) > 5 ) {//5seg de duracion del efecto
                            if(sonidoElegido)FSOUND_StopSound(5);
                            timerPremioActivado[i] = 0; //el efecto del premio, luego de la colision expiro tras los 5 seg.
                            banderaVelocidadParedReducida = 0;
                    }
                    break;
                case 4: //1000puntos
                    if(banderaScore){
                            score+=1000;
                            banderaScore = 0;
                    }
                    if((tiempoJuego - timerPremioActivado[i]) > 1 )timerPremioActivado[i] = 0; //el efecto del premio, luego de la colision expiro tras los 5 seg.
                    glColor3f(255,255,255);
                    drawString(GLUT_BITMAP_HELVETICA_18,"+1000",  -8,13,0);
                    break;
                }
            }
            else
            {
                if(!premioEnPantalla[i] && (timerPremioActivado[i]== 0) && (tiempoJuego%5 == 0))  // si el premio no esta activado y su tiempo de efecto es cero.
                {                                                                                  //Esperamos 5 seg antes de generar los premios
                    //srand(time(NULL));
                    randPremio = 1 + (rand()%((int)100)); // Probabilidad
                    if(randPremio <= porcentajeProbabilidad[i])  //vidaExtra
                    {
                        premioEnPantalla[i] = true;
                        timerPremio[i] = tiempoJuego; //se activa el tiempo de aparicion en el campo por 7 seg
                        puntoRandom = randomUbicacion(); //genera un punto aleatorio dentro del area de mov. de la nave

                        if(dentroAreaCircular(puntoRandom))
                        {
                            coordPremio[i].z =  puntoRandom.z;
                            coordPremio[i].x =   puntoRandom.x;
                        }
                    }

                }
            }

        }//endfor
}

/**********************************************************
 *
 * SUBROUTINE dibujarCampoDeFuerza(void)
 *********************************************************/
void dibujarCampoDeFuerza(int i){
// Toro Superior

    glPushMatrix();
        glRotated(a,0,0,1);
        glRotated(60,1,0,0);
        glutSolidTorus(0.05,2.0f+(0.5*i),4,30);
    glPopMatrix();
    // Toro Superior
    glPushMatrix();
        glRotated(a,1,1,0);
        glRotated(60,1,0,0);
        glutSolidTorus(0.05,2.0f+(0.5*i),4,60);
    glPopMatrix();

}

/**********************************************************
 *
 * SUBROUTINE manejoControlDibujadoParedes(void)
 *********************************************************/
void manejoControlDibujadoParedes(int tiempoJuego){

    int impacto = 0;
    int intervalo = 0;
    //srand(time(NULL));

    float velocPared = 0.06 + (0.01*(tiempoJuego/intervaloTiempoAumentoVelocidadPared)*dificultadElegida);//medida arbitraria de velocidad, dependiente de la dificultad y el intervalo de tiempo de aumento.
        LimiteCampo = LimiteCampo - velocPared/(banderaVelocidadParedReducida+1); //como la bandera tiene solo dos valores, cuando es cero se le suma 1, entonces queda el valor sin modificar al hacer la division; en cambio, si la bandera es 1, la suma da 2, entonces se divide el valor entre 2, reduciendo la velocidad de la pared

        printf("Veloc Pared = %f\n",velocPared);
        printf("Limite Campo = %f\n",LimiteCampo);
        if (LimiteCampo < MIN_VAL)
        {
            LimiteCampo = inicioParedes;
            banderaImpactoNavePared = 0;

            randomFuncion = rand()%(cantidadFigurasPared); //el +1 es para que no dibuje la pared triangular
            randomPared = rand()%((int)(randomFuncion + 4));
        }
        inicializarVariablesPared();
        vectorFuncion[randomFuncion](); //dibuja la pared
        //*******************COLISION***********************
        cantidadLadosPared = (int)(randomFuncion + 4);//cantidad de lados que tiene la pared elegida
        switch(randomFuncion)
        {
        case 0://Cuadrado
            productoVectorial(randomFuncion,matrizCuadrado,cantidadLadosPared);
            banderaNaveDentroPared = InsidePolygon(matrizCuadrado,cantidadLadosPared);
            break;
        case 1://Pentagono
            productoVectorial(randomFuncion,matrizPentagono,cantidadLadosPared);
            banderaNaveDentroPared = InsidePolygon(matrizPentagono,cantidadLadosPared);
            break;
        case 2://Hexagono
            productoVectorial(randomFuncion,matrizHexagono,cantidadLadosPared);
            banderaNaveDentroPared = InsidePolygon(matrizHexagono,cantidadLadosPared);
            break;
        }


        impacto = impactoNavePared(cantidadLadosPared);
        printf("Impacto = %d\n",impacto);
        if(banderaInvencibilidad)impacto = 0; //Si el modo es invencible, impacto = 0, no hubo colision.
        if(impacto && (banderaImpactoNavePared == 0)){
                tiempoImpactoNavePared = tiempoJuego;
                if(sonidoElegido)FSOUND_PlaySound (8,impactoParedSound);
                banderaImpactoNavePared = 1;
                vida--;
                if((score - 10) >= 0)score-=10;
        }
        if(((tiempoJuego - tiempoImpactoNavePared) <= 2) && impacto)banderaEfectoChoqueNave = 0; //si hubo impacto, y el tiempo transcurrido desde el impacto no supera los 2 seg, ponemos en rojo el color de la nave
        else banderaEfectoChoqueNave = 1; //activamos de nuevo el color real de la nave
}
/**********************************************************
 *
 * SUBROUTINE contactoNavePremios(void)
 *
 *********************************************************/
void contactoNavePremios(Punto3d coordenada, int numeroPremio, int tiempoJuego)
{
    (&premio)->centro.x = coordenada.x; //Ubica las esferas en el plano
    (&premio)->centro.y = 0.0f;
    (&premio)->centro.z = coordenada.z;//zRelativoEsfera;
    (&premio)->radio = radioPremioEsfera;
    (&nave)->centro.x = xRelativo; //Coordenada de la esfera inscripta en la nave
    (&nave)->centro.y = 0.0f;
    (&nave)->centro.z = zRelativo;
    (&nave)->radio = radioNaveEsfera;

    if(metodoCuadrados)
    {
        printf("Radios Cuadrados\n");
        colision = evaluarContactoCuad(&premio,&nave);

        if(colision)
        {
            printf("Colision = 1\n");
            premioEnPantalla[numeroPremio] = false;
            timerPremioActivado[numeroPremio] = tiempoJuego;
            timerPremio[numeroPremio] = 0;
            if(numeroPremio == 0) {
                    if(sonidoElegido)FSOUND_PlaySound (4,vidaExtraSound);
                    banderaVida = 1; //activamos la bandera de vida para comer una sola vida
            }
            if(numeroPremio == 1) {
                    if(sonidoElegido)FSOUND_PlaySound (2,invencibilidadSound);
                    banderaInvencibilidad = 1;//activamos la bandera de invencibilidad
            }
            if(numeroPremio == 2){
                    if(sonidoElegido)FSOUND_PlaySound (11,velocidadNaveAumentaSound);
                    banderaVelocidadNaveDoble = 1;
            }
            if(numeroPremio == 3){
                    if(sonidoElegido)FSOUND_PlaySound (5,velocidadParedReducidaSound);
                    banderaVelocidadParedReducida = 1;
            }
            if(numeroPremio == 4){
                    if(sonidoElegido)FSOUND_PlaySound (6,puntoExtraSound);
                    banderaScore = 1; //activamos la bandera de score para asignar puntos una sola vez
            }

        }
        else
        {
            printf("Colision = 0\n");
        }
    }
    else
    {
        printf("Suma de radios\n");
        colision = evaluarContacto(&premio,&nave);
        if(colision)
        {
            printf("Colision = 1\n");
            premioEnPantalla[numeroPremio] = false; //ya no se dibuja en pantalla
            timerPremioActivado[numeroPremio] = tiempoJuego; //empieza a correr el tiempo del efecto del poder
            timerPremio[numeroPremio] = 0;
            if(numeroPremio == 0) {
                    if(sonidoElegido)FSOUND_PlaySound (4,vidaExtraSound);
                    banderaVida = 1; //activamos la bandera de vida para comer una sola vida
            }
            if(numeroPremio == 1) {
                    if(sonidoElegido)FSOUND_PlaySound (2,invencibilidadSound);
                    banderaInvencibilidad = 1;//activamos la bandera de invencibilidad
            }
            if(numeroPremio == 2){
                    if(sonidoElegido)FSOUND_PlaySound (11,velocidadNaveAumentaSound);
                    banderaVelocidadNaveDoble = 1;
            }
            if(numeroPremio == 3){
                    if(sonidoElegido)FSOUND_PlaySound (5,velocidadParedReducidaSound);
                    banderaVelocidadParedReducida = 1;
            }
            if(numeroPremio == 4){
                    if(sonidoElegido)FSOUND_PlaySound (6,puntoExtraSound);
                    banderaScore = 1; //activamos la bandera de score para asignar puntos una sola vez
            }

        }
        else
        {
            printf("Colision = 0\n");
        }
    }
}

/**********************************************************
 *
 * SUBROUTINE keyOperations()
 * Realiza las operaciones de acuerdo a las teclas presionadas.
 *
 *********************************************************/

void keyOperations (void)
{

    float velocidadGiroMundo = (0.5f*(banderaVelocidadNaveDoble + 1));//0.5f original
    float velocidadAvanceMundo = (0.1f*(banderaVelocidadNaveDoble + 1));//0.1f original
    float velocidadAvanceObjetoNave = 0.008*(banderaVelocidadNaveDoble + 1);

    if (keyStates['l'] || keyStates['L']) //Giro a la derecha
    {
        if (rotX >= 360) rotX = (int)rotX % 360;
        coordenadaObjeto.x = X * cos(toRadian(rotX + velocidadGiroMundo)) - Z * sin(toRadian(rotX + velocidadGiroMundo));
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = X * sin(toRadian(rotX + velocidadGiroMundo)) + Z * cos(toRadian(rotX + velocidadGiroMundo));
        if (dentroAreaCircular(coordenadaObjeto))
        {
            rotX += velocidadGiroMundo;

        }

    }

    if(keyStates['n']) //cambiar metodo de deteccion de la colision. Cambia de formula, mismo funcionamiento.
    {
        metodoCuadrados = !metodoCuadrados;

    }

    if(keyStates['e'] || keyStates['E'])//Rota la nave. Sin funcion aparente.
    {
        ObjRotate(&object[NAVE],0,20,0);

    }
    if (keyStates['j'] || keyStates['J'])//Giro a la izquierda
    {
        if (abs(rotX) >= 360) rotX = (int)rotX % 360;
        coordenadaObjeto.x = X * cos(toRadian(rotX + velocidadGiroMundo)) - Z * sin(toRadian(rotX + velocidadGiroMundo));
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = X * sin(toRadian(rotX + velocidadGiroMundo)) + Z * cos(toRadian(rotX + velocidadGiroMundo));
        if (dentroAreaCircular(coordenadaObjeto))
        {
            rotX -= velocidadGiroMundo;
        }

    }
    if (keyStates['a'] || keyStates['A'])//Avance hacia la izquierda
    {
        coordenadaObjeto.x = X + velocidadAvanceMundo;
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = Z;
        if (dentroAreaCircular(coordenadaObjeto))
        {
            X += velocidadAvanceMundo;
            ObjTranslate(&object[NAVE],-velocidadAvanceObjetoNave,0,0);

        }
    }
    if (keyStates['d'] || keyStates['D'])//Avance hacia la derecha
    {
        coordenadaObjeto.x = X - velocidadAvanceMundo;
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = Z;
        if (dentroAreaCircular(coordenadaObjeto))
        {
            X -=  velocidadAvanceMundo;
            ObjTranslate(&object[NAVE],velocidadAvanceObjetoNave,0,0);
        }
    }
    if (keyStates['w'] || keyStates['W'])//Avance  hacia adelante
    {
        coordenadaObjeto.x = X;
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = Z + velocidadAvanceMundo;
        if(dentroAreaCircular(coordenadaObjeto))
        {
            Z += velocidadAvanceMundo;
            ObjTranslate(&object[NAVE],0,0,-velocidadAvanceObjetoNave);
        }
    }
    if (keyStates['s'] || keyStates['S'])//Retroceso
    {
        coordenadaObjeto.x = X;
        coordenadaObjeto.y = 0;
        coordenadaObjeto.z = Z - velocidadAvanceMundo;
        if(dentroAreaCircular(coordenadaObjeto))
        {
            Z -= velocidadAvanceMundo;
            ObjTranslate(&object[NAVE],0,0,velocidadAvanceObjetoNave);
        }
    }

    if (keyStates['r'] || keyStates['R'])//Cambiar el pilling. Dibuja con grillas. Sin funcion aparente.
    {
        if (filling==0)
        {
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
            filling=1;
        }
        else
        {
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
            filling=0;
        }
    }

    if (keyStates[27])  //tecla ESC
    {
        exit(0);
    }

}

/**********************************************************
 *
 * SUBROUTINE keyPressed(void)
 * Detecta todas las teclas presionadas, y las va guardando
 * como true.
 *********************************************************/
void keyPressed (unsigned char key, int x, int y)
{
// Set the state of the current key to pressed
    keyStates[key] = true;
}

/**********************************************************
 *
 * SUBROUTINE keyUp(void)
 * Pone en false las teclas que se han soltado. Actualizan
 * el vector
 *********************************************************/
void keyUp (unsigned char key, int x, int y)
{
// Set the state of the current key to not pressed
    keyStates[key] = false;

}

/**********************************************************
 *
 * SUBROUTINE locatedMouse(void)
 * Utilizado en el menu Principal, permite detectar la ubicacion
 * del puntero del mouse, de esta forma nos fue posible
 * dar el efecto de seleccion de las opciones del menu.
 *********************************************************/
void locatedMouse(int x, int y){
    if(pantalla == AREA_PANTALLA )
    {
        if(x >= 215 && x <= 508 && y >= 273 && y <= 333)
        {
            imagenMenu = menuPrincipal_jugar;
        }else if(x >= 215 && x <= 508 && y >= 343 && y <= 404)
        {
            imagenMenu = menuPrincipal_opciones;
        }else if(x >= 215 && x <= 508 && y >= 413 && y <= 474)
        {
            imagenMenu = menuPrincipal_salir;
        }else{
            imagenMenu = menuPrincipal;
        }
    }
    if(pantalla == OPCIONES )
    {

        if(x >= 336 && x <= 596 && y >= 196 && y <= 255)
        {
            imagenMenu = menuOpciones_instrucciones;
        }else if(x >= 336 && x <= 596 && y >= 260 && y <= 325)
        {
            imagenMenu = menuOpciones_dificultad;
        }else if(x >= 336 && x <= 596 && y >= 330 && y <= 398)
        {
            imagenMenu = menuOpciones_sonido;
        }else if(x >= 710 && x <= 934 && y >= 562 && y <= 628)
        {
            imagenMenu = menuOpciones_volver;
        }else{
            imagenMenu = menuOpciones;
        }

    }

    if(pantalla == INSTRUCCIONES )
    {

        if(x >= 694 && x <= 897 && y >= 584 && y <= 655)
        {
            imagenMenu = menuInstrucciones_volver;

        }else{
            imagenMenu = menuInstrucciones;
        }


    }

    if(pantalla == SONIDO )
    {
        if(x >= 333 && x <= 531 && y >= 209 && y <= 274)
        {
             imagenMenu = menuSonido_sinSonido;
        }else if(x >= 333 && x <= 531 && y >= 279 && y <= 344)
        {
             imagenMenu = menuSonido_sonido;
        }else if(x >= 758 && x <= 911 && y >= 560 && y <= 630)
        {
            imagenMenu = menuSonido_volver;
        }else
        {
            imagenMenu = menuSonido;
        }


    }
     if(pantalla == DIFICULTAD )
    {
        if(x >= 357 && x <= 550 && y >= 233 && y <= 294)
        {
             imagenMenu = menuDificultad_facil;
        }else if(x >= 357 && x <= 550 && y >= 300 && y <= 360){
            imagenMenu = menuDificultad_medio;
        }else if(x >= 357 && x <= 550 && y >= 365 && y <= 432){
            imagenMenu = menuDificultad_dificil;
        }else if(x >= 736 && x <= 905 && y >= 555 && y <= 626){
            imagenMenu = menuDificultad_volver;
        }else{
            imagenMenu = menuDificultad;
        }

    }

    if(pantalla == AREA_FIN_DE_JUEGO){
        if(x >= 147 && x <= 437 && y >= 611 && y <= 652)//nueva partida posicionado puntero mouse
        {
            imagenMenu = menuFinJuego_nuevaPartida;
        }else if(x >= 711 && x <= 837 && y >= 611 && y <= 652){//salir posicionado puntero mouse
            imagenMenu = menuFinJuego_salir;
        }else{
            imagenMenu = menuFinJuego;
        }

    }

}
/**********************************************************
 *
 * The main routine
 *
 *********************************************************/

int main(int argc, char **argv)
{
    // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
    //*****************
    srand(time(NULL));
    randomFuncion = rand()%(cantidadFigurasPared); //para elegir una pared inicial
    //**************
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(screen_width,screen_height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("SpaceXagon");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc (resize);
    glutEntryFunc(processMouseEntry);
    glutMouseFunc(processMouse);
    glutPassiveMotionFunc(locatedMouse);
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp);
    init();


    //a flag check to make sur ethe textures are only loaded in once
    if (texturesCreated == false)
    {
        InitialiseTextures();
        texturesCreated = true; //sets the flag to true so this if statement doesn't execute again
    }
    imagenMenu = menuPrincipal;//inicializamos la pantalla del menu principal
    inicializarVariables();
    inicializarSonidos();

    //********************
    inicializarVariablesPared();
    //vectorFuncion[0] = &dibujarParedTriangulo;



    //********************
    glutMainLoop();

    return(0);
}


/**********************************************************
 *
 * SUBROUTINE LoadBmp(void)
 *
 *********************************************************/
unsigned char *LoadBmp(char *fn, int *wi, int *hi)
{
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    WORD bits;
    FILE *t24;
    unsigned char *lpBitmapBits;
    long imagesize,nc;

    // read the bitmap
    t24=fopen((char *)fn,"rb");
    if(t24 == NULL)
    {
        printf("Could not open input file\n");
        exit(0);
    }
    fread((char *)&bmfh,sizeof(BITMAPFILEHEADER),1,t24);
    fread((char *)&bmih,sizeof(BITMAPINFOHEADER),1,t24);
    if(bmih.biClrUsed != 0)nc=bmih.biClrUsed;
    else
    {
        bits = bmih.biBitCount;
        switch (bits)
        {
        case 1:
            nc=2;
            break;
        case 4:
            nc=16;
            break;
        case 8:
            nc=256;
            break;
        default:
            nc=0;
            break;
        }
    }
    if(nc > 0)
    {
        printf("Cannot handle paletted image\n");
        exit(0);
    }
    imagesize=bmfh.bfSize-bmfh.bfOffBits;
    if((lpBitmapBits=(unsigned char *)malloc(imagesize)) == NULL)
    {
        fclose(t24);
        exit (0);
    }
    fread((char *)lpBitmapBits,imagesize,1,t24);
    fclose(t24);
    *wi=bmih.biWidth;
    *hi=bmih.biHeight;
    return lpBitmapBits;
}


/**********************************************************
 *
 * SUBROUTINE InitialiseTextures(void)
 *
 *********************************************************/
void InitialiseTextures()
{
    //calls the procedure to load and store every texture
    GenerateTextures("textura/earthmap1k.bmp", 1);
    GenerateTextures("textura/sunmap.bmp", 2);
    GenerateTextures("textura/mercurymap.bmp", 3);
    GenerateTextures("textura/moon.bmp", 4);
    GenerateTextures("textura/jupitermap.bmp", 5);
    GenerateTextures("textura/stars.bmp", 6);
    GenerateTextures("textura/stars_I.bmp", 7);
    GenerateTextures("textura/asteroide_II.bmp", 9);
    GenerateTextures("textura/menu/pantalla.bmp", 8);
    GenerateTextures("textura/menu/pantalla_jugar.bmp", 14);
    GenerateTextures("textura/menu/pantalla_opciones.bmp", 15);
    GenerateTextures("textura/menu/pantalla_salir.bmp", 16);
    GenerateTextures("textura/menu/opciones.bmp", 10);
    GenerateTextures("textura/menu/opciones_dificultad.bmp", 17);
    GenerateTextures("textura/menu/opciones_instrucciones.bmp", 18);
    GenerateTextures("textura/menu/opciones_sonidos.bmp", 19);
    GenerateTextures("textura/menu/opciones_volver.bmp", 20);
    GenerateTextures("textura/menu/instrucciones.bmp", 12);
    GenerateTextures("textura/menu/instrucciones_volver.bmp", 21);
    GenerateTextures("textura/menu/sonido.bmp", 11);
    GenerateTextures("textura/menu/sonido_sinSonido.bmp", 22);
    GenerateTextures("textura/menu/sonido_sonido.bmp", 23);
    GenerateTextures("textura/menu/sonido_volver.bmp", 24);
    GenerateTextures("textura/menu/dificultad.bmp", 13);
    GenerateTextures("textura/menu/dificultad_dificil.bmp", 25);
    GenerateTextures("textura/menu/dificultad_medio.bmp", 26);
    GenerateTextures("textura/menu/dificultad_facil.bmp", 27);
    GenerateTextures("textura/menu/dificultad_volver.bmp", 28);
    GenerateTextures("textura/menu/finJuego.bmp", 29);
    GenerateTextures("textura/menu/finJuego_nueva_partida.bmp", 30);
    GenerateTextures("textura/menu/finJuego_salir.bmp", 31);



}


/**********************************************************
 *
 * SUBROUTINE GenerateTextures(void)
 * El char* es usado para almacenar el nombre de la textura a cargar,
 * el numero usado para determinar en que variable es almacenado.
 *********************************************************/
void GenerateTextures(char *name, int i)
{
    unsigned char *pix;
    int w,h;
    glEnable(GL_TEXTURE_2D);
    //assigns pix to a texture
    pix=LoadBmp(name,&w,&h);
    if (i == 1) //textura Tierra
    {
        glGenTextures(1, &earthTexture);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
    }
    if (i == 2) //textura Sol
    {
        glGenTextures(1, &sunTexture);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
    }
    if (i == 3) //textura de Mercurio (prueba)
    {
        glGenTextures(1, &mercuryTexture);
        glBindTexture(GL_TEXTURE_2D, mercuryTexture);
    }

    if (i == 4) //textura de la Luna
    {
        glGenTextures(1, &moonTexture);
        glBindTexture(GL_TEXTURE_2D, moonTexture);
    }

    if (i == 5) //textura de Jupiter (prueba)
    {
        glGenTextures(1, &jupiterTexture);
        glBindTexture(GL_TEXTURE_2D, jupiterTexture);
    }


    if (i == 6) //textura de las estrellas
    {
        glGenTextures(1, &starsTexture);
        glBindTexture(GL_TEXTURE_2D, starsTexture);
    }
    if (i == 7) //textura de las estrellas (prueba)
    {
        glGenTextures(1, &starsTexture_I);
        glBindTexture(GL_TEXTURE_2D, starsTexture_I);
    }


    if (i == 8) //textura del menu Principal
    {
        glGenTextures(1, &menuPrincipal);
        glBindTexture(GL_TEXTURE_2D, menuPrincipal);
    }

    if (i == 9) //textura asteroide
    {
        glGenTextures(1, &asteroide);
        glBindTexture(GL_TEXTURE_2D, asteroide);
    }
    if (i == 10) //textura del menu opciones
    {
    glGenTextures(1, &menuOpciones);
    glBindTexture(GL_TEXTURE_2D, menuOpciones);
    }
    if (i == 11) //textura del menu sonido
    {
    glGenTextures(1, &menuSonido);
    glBindTexture(GL_TEXTURE_2D, menuSonido);
    }
    if (i == 12) //textura del menu instrucciones
    {
    glGenTextures(1, &menuInstrucciones);
    glBindTexture(GL_TEXTURE_2D, menuInstrucciones);
    }
    if (i == 13) //textura del menu instrucciones
    {
    glGenTextures(1, &menuDificultad);
    glBindTexture(GL_TEXTURE_2D, menuDificultad);
    }
    if (i == 14) //textura del menu principal, posicionado sobre jugar
    {
    glGenTextures(1, &menuPrincipal_jugar);
    glBindTexture(GL_TEXTURE_2D,menuPrincipal_jugar);
    }
    if (i == 15) //textura del menu principal, posicionado sobre opciones
    {
    glGenTextures(1, &menuPrincipal_opciones);
    glBindTexture(GL_TEXTURE_2D,menuPrincipal_opciones);
    }
    if (i == 16) //textura del menu principal, posicionado sobre salir
    {
    glGenTextures(1, &menuPrincipal_salir);
    glBindTexture(GL_TEXTURE_2D,menuPrincipal_salir);
    }
    if (i == 17) //textura del menu opciones, posicionado sobre dificultad
    {
    glGenTextures(1, &menuOpciones_dificultad);
    glBindTexture(GL_TEXTURE_2D,menuOpciones_dificultad);
    }
    if (i == 18) //textura del menu opciones, posicionado sobre instrucciones
    {
    glGenTextures(1, &menuOpciones_instrucciones);
    glBindTexture(GL_TEXTURE_2D,menuOpciones_instrucciones);
    }
    if (i == 19) //textura del menu opciones, posicionado sobre sonido
    {
    glGenTextures(1, &menuOpciones_sonido);
    glBindTexture(GL_TEXTURE_2D,menuOpciones_sonido);
    }
    if (i == 20) //textura del menu opciones, posicionado sobre volver
    {
    glGenTextures(1, &menuOpciones_volver);
    glBindTexture(GL_TEXTURE_2D,menuOpciones_volver);
    }
    if (i == 21) //textura del menu instrucciones, posicionado sobre volver
    {
    glGenTextures(1, &menuInstrucciones_volver);
    glBindTexture(GL_TEXTURE_2D,menuInstrucciones_volver);
    }
    if (i == 22) //textura del menu sonido, posicionado sobre sin Sonido
    {
    glGenTextures(1, &menuSonido_sinSonido);
    glBindTexture(GL_TEXTURE_2D,menuSonido_sinSonido);
    }
    if (i == 23) //textura del menu sonido, posicionado sobre sonido
    {
    glGenTextures(1, &menuSonido_sonido);
    glBindTexture(GL_TEXTURE_2D,menuSonido_sonido);
    }
    if (i == 24) //textura del menu sonido, posicionado sobre volver
    {
    glGenTextures(1, &menuSonido_volver);
    glBindTexture(GL_TEXTURE_2D,menuSonido_volver);
    }
    if (i == 25) //textura del menu dificultad, posicionado sobre dificil
    {
    glGenTextures(1, &menuDificultad_dificil);
    glBindTexture(GL_TEXTURE_2D,menuDificultad_dificil);
    }
    if (i == 26) //textura del menu dificultad, posicionado sobre medio
    {
    glGenTextures(1, &menuDificultad_medio);
    glBindTexture(GL_TEXTURE_2D,menuDificultad_medio);
    }
    if (i == 27) //textura del menu dificultad, posicionado sobre facil
    {
    glGenTextures(1, &menuDificultad_facil);
    glBindTexture(GL_TEXTURE_2D,menuDificultad_facil);
    }
    if (i == 28) //textura del menu dificultad, posicionado sobre volver
    {
    glGenTextures(1, &menuDificultad_volver);
    glBindTexture(GL_TEXTURE_2D,menuDificultad_volver);
    }
    if (i == 29) //textura del menu fin de juego
    {
    glGenTextures(1, &menuFinJuego);
    glBindTexture(GL_TEXTURE_2D,menuFinJuego);
    }
    if (i == 30) //textura del menu fin de juego, posicionado sobre nueva partida
    {
    glGenTextures(1, &menuFinJuego_nuevaPartida);
    glBindTexture(GL_TEXTURE_2D,menuFinJuego_nuevaPartida);
    }
    if (i == 31) //textura del menu fin de juego, posicionado sobre salir
    {
    glGenTextures(1, &menuFinJuego_salir);
    glBindTexture(GL_TEXTURE_2D,menuFinJuego_salir);
    }

    glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pix);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    free(pix); //deletes the data held by pix
    glDisable(GL_TEXTURE_2D);
}


/**********************************************************
 *
 * SUBROUTINE productoVectorial(void)
 * Para hallar las normales de los planos, se utilizan dos puntos extremos
 * cruzados del plano, formando asi dos vectores, para luego realizar el
 * producto vectorial entre ellos, resultando en la normal.
 *********************************************************/
void productoVectorial(int randomFuncion, float matriz[][2], int cant)
{
    int i,j;
    Punto3d p1,p2,p3,p4;

    for (i=0; i<cant; i++)
    {
        p1.x = matriz[i][0];
        p1.y = 0;
        p1.z = matriz[i][1];
        p2.x = matriz[i][0];
        p2.y = alturaPared;
        p2.z = matriz[i][1];
        p3.x = matriz[i+1][0];
        p3.y = alturaPared;
        p3.z = matriz[i+1][1];
        p4.x = matriz[i+1][0];
        p4.y = 0;
        p4.z = matriz[i+1][1];

        //Halla los vectores P1P3 y P4P2
        vectoresPlano[0][0] = p3.x - p1.x;
        vectoresPlano[0][1] = p3.y - p1.y;
        vectoresPlano[0][2] = p3.z - p1.z;
        vectoresPlano[1][0] = p2.x - p4.x;
        vectoresPlano[1][1] = p2.y - p4.y;
        vectoresPlano[1][2] = p2.z - p4.z;
        //Se realiza el producto vectorial por medio de determinantes. Fuente http://es.wikipedia.org/wiki/Producto_vectorial
        /*a*/    vectoresNormalesPlanos[i][0] = vectoresPlano[0][1]*vectoresPlano[1][2] - vectoresPlano[0][2]*vectoresPlano[1][1];
        /*b*/    vectoresNormalesPlanos[i][1] = -(vectoresPlano[0][0]*vectoresPlano[1][2] - vectoresPlano[0][2]*vectoresPlano[1][0]);
        /*c*/    vectoresNormalesPlanos[i][2] = vectoresPlano[0][0]*vectoresPlano[1][1] - vectoresPlano[1][0]*vectoresPlano[0][1];
        /*ax+by+cz = d*/ vectoresNormalesPlanos[i][3] = vectoresNormalesPlanos[i][0]* p1.x + vectoresNormalesPlanos[i][1]*p1.y + vectoresNormalesPlanos[i][2]*p1.z;

    }
}


/**********************************************************
 *
 * SUBROUTINE distanciaPuntoPlano(void)
 *
 *********************************************************/
float distanciaPuntoPlano(float x, float y, float z, float a, float b, float c, float d)
{
    //x,y,z Coordenas centro esfera
    //a,b,c,d Parametros del plano ax+by+cz-d = 0
    float valorDistanciaPuntoPlano = 0.0f;
    float distancia = abs(a*x + b*y + c*z + d);
    valorDistanciaPuntoPlano = distancia/sqrt(pow(a,2)+pow(b,2)+pow(c,2));//Formula de distancia del punto al plano
    return valorDistanciaPuntoPlano;
}


/**********************************************************
 *
 * SUBROUTINE impactoNavePared(void)
 *
 *********************************************************/
int impactoNavePared(int cantPared)
{
    int i,j;
    int banderaImpacto = 0;

    for(i=0; i<cantPared; i++)
    {
       /* printf("Random Pared = %d\n",randomPared);
        printf("Distancia esfera nave = %f\n",distanciaPuntoPlano(xRelativo,0.0f,zRelativo,vectoresNormalesPlanos[i][0],vectoresNormalesPlanos[i][1],vectoresNormalesPlanos[i][2],vectoresNormalesPlanos[i][3]));
        printf("Pared = %d\n",i);*/

        //printf("Radio Nave Esfera = %f\n",radioNaveEsfera);
        if((i != randomPared) && (banderaNaveDentroPared == 0)) //se verifica con todas las paredes, menos con la pared vacia (random Pared)
        {
            if (distanciaPuntoPlano(xRelativo,0.0f,zRelativo,vectoresNormalesPlanos[i][0],vectoresNormalesPlanos[i][1],vectoresNormalesPlanos[i][2],vectoresNormalesPlanos[i][3]) <= radioNaveEsfera)//se pasa las variables obtenidas a la funcion distancia, se verifica que sea menor que el radio de la Nave o sino es choque
            {
                banderaImpacto++;
            }
        }
    }
    //printf("banderaImpacto = %d\n", banderaImpacto);
    if(banderaImpacto == 0) return 0;
    else return 1;
}


/**********************************************************
 *
 * SUBROUTINE InsidePolygon(void)
 * Verifica si un punto esta dentro de las paredes. Se hace este control
 * para evitar hallar la distancia con la pared estando ya afuera.
 * La colision se detecta
 *********************************************************/
int InsidePolygon(float matriz[][2],int N)
{
    int counter = 0;
    int i;
    double xinters;
    Punto3d p1,p2,p;

    p1.x = matriz[0][0];
    p1.z = matriz[0][1];
    p.x = xRelativo;
    p.z = zRelativo;
    for (i=1; i<=N; i++)
    {
        p2.x = matriz[i % N][0];
        p2.z = matriz[i % N][1];
        if (p.z > MIN(p1.z,p2.z))
        {
            if (p.z <= MAX(p1.z,p2.z))
            {
                if (p.x <= MAX(p1.x,p2.x))
                {
                    if (p1.z != p2.z)
                    {
                        xinters = (p.z-p1.z)*(p2.x-p1.x)/(p2.z-p1.z)+p1.x;
                        if (p1.x == p2.x || p.x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }

    if (counter % 2 == 0)
        return(1);
    else
        return(0);
}


/**********************************************************
 *
 * SUBROUTINE inicializarSonidos(void)
 *
 *********************************************************/
int inicializarSonidos()
{
    // initialise fmod, 44000 Hz, 64 channels
    if( FSOUND_Init(44000,64,0) == FALSE )
    {
        std::cerr << "[ERROR] Could not initialise fmod\n";
        return 0;
    }

    // attempt to open the mp3 file as a stream
    menuSound = FSOUND_Sample_Load (0,"sounds/Mission.mp3",0,0,0);
    aceleracionNaveSound = FSOUND_Sample_Load (1,"sounds/Warp.mp3",0,0,0);
    invencibilidadSound = FSOUND_Sample_Load (2,"sounds/Dirty_wave.mp3",0,0,0);
    juegoSound = FSOUND_Sample_Load (3,"sounds/Diamonds.mp3",0,0,0);
    vidaExtraSound = FSOUND_Sample_Load (4,"sounds/Laser_power_up.mp3",0,0,0);
    velocidadParedReducidaSound = FSOUND_Sample_Load (5,"sounds/Power_down.mp3",0,0,0);
    puntoExtraSound = FSOUND_Sample_Load (6,"sounds/Hard_impact_alarm.mp3",0,0,0);
    velocidadNaveReducidaSound =  (7,"sounds/Dull_drum_hit.mp3",0,0,0);
    impactoParedSound = FSOUND_Sample_Load (8,"sounds/Boiling_crash.mp3",0,0,0);
    velocidadParedAumentaSound = FSOUND_Sample_Load (9,"sounds/Face_off.mp3",0,0,0);
    finalJuegoSound = FSOUND_Sample_Load (10,"sounds/Night_of_the_dead.mp3",0,0,0);
    velocidadNaveAumentaSound =  FSOUND_Sample_Load(11,"sounds/Phaze.mp3",0,0,0);



    // make sure mp3 opened OK
    if(!menuSound)
    {
        std::cerr << "[ERROR] could not open file\n";
        return 0;
    }


    FSOUND_PlaySound (0,menuSound);
    FSOUND_SetLoopMode(0,FSOUND_LOOP_NORMAL);


    return 0;

}


void processMouseEntry(int state){
     if (state == GLUT_ENTERED){ // si el puntero se encuentra dentro de la ventana
        printf("estais adentro\n");

     }else{ // si el puntero se encuentra fuera de la ventana
            printf("estais afuera\n");
     }
}

/**********************************************************
 *
 * SUBROUTINE processMouse(void)
 * Detecta los clicks
 *********************************************************/
void processMouse(int button, int state, int x, int y){
                 if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
                           x_click_mouse = x;
                           y_click_mouse = y;
                 }
                 //if(state == GLUT_UP)printf("X = %d Y=%d\n",x,y);


}

/**********************************************************
 *
 * SUBROUTINE mouseOperations(void)
 * De acuerdo a los clicks, se realizan distintas operaciones.
 *********************************************************/
void mouseOperations()
{

    if(pantalla == AREA_PANTALLA )
    {

        if(x_click_mouse >= 215 && x_click_mouse <= 508 && y_click_mouse >= 273 && y_click_mouse <= 333)//jugar
        {
            if(sonidoElegido){
                FSOUND_StopSound(0);//se detiene la musica del menu
                FSOUND_PlaySound(3,juegoSound);//suena la musica del juego
                FSOUND_SetLoopMode(3,FSOUND_LOOP_NORMAL);//se pone en Loop para que repita la musica
            }
            pantalla = AREA_JUEGO;
            x_click_mouse = 0;
            y_click_mouse = 0;

        }
        if(x_click_mouse >= 215 && x_click_mouse <= 508 && y_click_mouse >= 343 && y_click_mouse <= 404)//opciones
        {
            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;

        }
        if(x_click_mouse >= 215 && x_click_mouse <= 508 && y_click_mouse >= 413 && y_click_mouse <= 474)//salir
        {
            exit(0);
        }

    }

    if(pantalla == OPCIONES )
    {

        if(x_click_mouse >= 336 && x_click_mouse <= 596 && y_click_mouse >= 196 && y_click_mouse <= 255)//instrucciones
        {
            imagenMenu = menuInstrucciones;
            pantalla = INSTRUCCIONES;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 336 && x_click_mouse <= 596 && y_click_mouse >= 260 && y_click_mouse <= 325)//dificultad
        {
            imagenMenu = menuDificultad;
            pantalla = DIFICULTAD;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 336 && x_click_mouse <= 596 && y_click_mouse >= 330 && y_click_mouse <= 398)//sonido
        {
            imagenMenu = menuSonido;
            pantalla = SONIDO;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 710 && x_click_mouse <= 934 && y_click_mouse >= 562 && y_click_mouse <= 628)//volver
        {
            imagenMenu = menuPrincipal;
            pantalla = AREA_PANTALLA;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }

    }

    if(pantalla == INSTRUCCIONES )
    {

        if(x_click_mouse >= 694 && x_click_mouse <= 897 && y_click_mouse >= 584 && y_click_mouse <= 655)//volver
        {

            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;

        }


    }

    if(pantalla == SONIDO )
    {

        if(x_click_mouse >= 333 && x_click_mouse <= 531 && y_click_mouse >= 209 && y_click_mouse <= 274)//sin sonido
        {
            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
            sonidoElegido = 0;//quitamos el sonido
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 333 && x_click_mouse <= 531 && y_click_mouse >= 279 && y_click_mouse <= 344)//con sonido
        {
            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
            sonidoElegido = 1;//ponemos sonido
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
         if(x_click_mouse >= 758 && x_click_mouse <= 911 && y_click_mouse >= 560 && y_click_mouse <= 630)//volver
        {
            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }


    }
     if(pantalla == DIFICULTAD )
    {
        if(x_click_mouse >= 357 && x_click_mouse <= 550 && y_click_mouse >= 233 && y_click_mouse <= 294)//facil
        {
             imagenMenu = menuOpciones;
             dificultadElegida = 1;//se pone en facil
            pantalla = OPCIONES;
              x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 357 && x_click_mouse <= 550 && y_click_mouse >= 300 && y_click_mouse <= 360){//medio
            imagenMenu = menuOpciones;
            dificultadElegida = 2; //se pone en normal
            pantalla = OPCIONES;
             x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 357 && x_click_mouse <= 550 && y_click_mouse >= 365 && y_click_mouse <= 432){//dificil
            imagenMenu = menuOpciones;
            dificultadElegida = 3;//se pone en dificil
            pantalla = OPCIONES;
             x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 736 && x_click_mouse <= 905 && y_click_mouse >= 555 && y_click_mouse <= 626){//volver
            imagenMenu = menuOpciones;
            pantalla = OPCIONES;
             x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }

    }

    if(pantalla == AREA_FIN_DE_JUEGO){
        if(x_click_mouse >= 147 && x_click_mouse <= 437 && y_click_mouse >= 611 && y_click_mouse <= 652)//nueva partida posicionado puntero mouse
        {
            imagenMenu = menuPrincipal;
            inicializarVariables();
            pantalla = AREA_PANTALLA;
            FSOUND_StopSound(10);
            FSOUND_PlaySound (0,menuSound);
            FSOUND_SetLoopMode(0,FSOUND_LOOP_NORMAL);
            x_click_mouse = 0;//ceramos las coordenadas, para que no detecte inmediatamente el mismo click en la sgte pantalla antes de entrar.
            y_click_mouse = 0;
        }
        if(x_click_mouse >= 711 && x_click_mouse <= 837 && y_click_mouse >= 611 && y_click_mouse <= 652){//click salir
            exit(0);
        }

    }




}
