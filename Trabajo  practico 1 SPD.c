/*
 *	Trabajo Práctico 1: Cronómetro Binario
 *
 *	Al iniciar la simulacion, todos los led deben comenzar apagados.
 *	Cada 1 segundo que transcurra, se deben encender los led
 *	de acuerdo al numero que representan.
 *
 *	Ejemplo:
 *   
 *	El numero 15 (0000001111) se representaria encendiendo los 
 *	leds AMARILLO y NARANJA.
 *	Al pasar al numero 16 (0000010000), se deben apagar todos 
 *	los led anteriores y encenderse uno de los leds VERDES. 
 *	Notese, en el ejemplo los 0 representan los led apagados 
 *	y los 1 los led encendidos). 
 *   
 *	-------------------------------------------------------
 *   
 *	Al presionarse el boton START, debe iniciar el cronometro.
 *	Volver a presionarlo hace que la secuencia se detenga. 
 *  (Como si se hubiese presionado pausa).
 *	Al presionarse el boton RESET, el cronometro
 *	debe reiniciarse y arrancar de 0.
 *
 *	Tip: Modularizar la función que controla el encendido de los 
 *	LEDS y de ser posible, todo el código para evitar repetir lineas lo mas posible.
 *  Usar millis para controlar el tiempo del contador para que el cambio de los 
 *	leds encendidos sea perceptible para el ojo humano y 
 *	documentar cada función creada en el código. 
 *	Un breve comentario que diga que es lo que hace esa función
 *  y de corresponder, que retorna).
*/

//--- Defines ---//
#define BOTON_INICIO 2
#define BOTON_REINICIO 3
#define PRIMER_LED 4         
#define ULTIMO_LED 13            
#define BASE_MILISEG 1000 
#define SEGUNDOS_MAX 1023       
//--- End Defines ---//


      
void setup()
{
  Serial.begin(9600);  
  set_pins();
}

int contador = 1;
int minutos = 0;
int segundos = 0;
int resto;

unsigned long millis_before = 0;
unsigned long millis_now;

short botonInicioAntes = 0;
short botonInicioAhora;
short botonReinicioAntes = 0;
short botonReinicioAhora;

short pausa = 0;
short flagTabla = 1;

//Inicializa los pines de entrada (botones) y salida (leds).
void set_pins()
{
  for(int i=PRIMER_LED;i<=ULTIMO_LED;i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(BOTON_REINICIO, INPUT);
  pinMode(BOTON_INICIO, INPUT);
}

//Imprime los segundos transcurridos desde el comienzo del contador, en forma binaria.
void cronometroBinario()
{
  for (int i = 9; i >=0; i--)
  {
  	Serial.print(bitRead(contador, i));
  }
}

//Imprime los segundos y minutos transcurridos desde el comienzo del contador, en forma decimal.
void cronometroDecimal()
{
  Serial.print("\n");
  segundos++;
  
  if(segundos==60)
  {
     minutos++;
    segundos=0;
  }
 
  if(minutos<10)
  {
    Serial.print("0");
  }
  Serial.print(minutos);
  Serial.print(" min, ");

  if(segundos<10)
  {
    Serial.print("0");
  }
  Serial.print(segundos);
  Serial.print(" seg         ||       ");
 
}

//Prende los Leds, de forma que se representa el tiempo de ejecucion actual, en forma binaria.
void mostrarLeds()
{
  resto = contador;
  for(int i=PRIMER_LED;i<=ULTIMO_LED;i++)
  {
    digitalWrite(i,resto%2);
    resto/=2; 
  }
}

//Imprime el formato de tabla de valores. Se imprime una unica vez por cada ciclo del contador.
void tabla()
{
  Serial.print("Tiempo transcurrido:   ||   Expresion en binario:");
}

//Ejecuta la secuencia principal del programa. Se leen los valores de los pulsadores, y se llaman a todas las demas funciones. Ademas, tiene implementado un retardo de 1 seg entre cada loop.
void secuencia()
{
  botonInicioAhora=digitalRead(2);
  botonReinicioAhora=digitalRead(3);
  
  if(contador == SEGUNDOS_MAX || (botonReinicioAhora==1 && botonReinicioAntes==0))
  {
    reiniciar();
  }
  if(botonInicioAhora==1 && botonInicioAntes==0)
  {
  	pausa=!pausa;
  }
  
  if(pausa)
  {
    if(flagTabla)
    {
      tabla();
      flagTabla=0;
    }
    
    millis_now = millis(); 
    if (millis_now - millis_before >= BASE_MILISEG)
    { 
      cronometroDecimal();
      cronometroBinario();
      mostrarLeds();  

      millis_before = millis_now;
      contador++;
    }
  }
  
  botonInicioAntes=botonInicioAhora;
  botonReinicioAntes=botonReinicioAhora;
}

//Reinicia todos los valores del contador, y le notifica al usuario por pantalla.
void reiniciar()
{
  Serial.print("\n\nReiniciando...\n\n");
  
  flagTabla=1;
  contador=1;
  segundos=0;
  minutos=0;
}


void loop()
{
  secuencia();
}