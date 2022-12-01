#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DEFINIÇÕES
#define endereco  0x27
#define colunas   16
#define linhas    2
// INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(endereco, colunas, linhas);
int seconds = 0;

#define VENT 10
#define FALHA 9
#define MOTOR_FUN 6
#define SENSOR_TEMP_MOTOR A1

char ciclos;
int motor, motor_comp;

//Função principal
void setup()
{ 

  //Definição de entradas e saídas
  pinMode(MOTOR_FUN, OUTPUT);     pinMode(FALHA, OUTPUT);
  pinMode(VENT, OUTPUT);   pinMode(SENSOR_TEMP_MOTOR, INPUT);  

  lcd.init();
  lcd.backlight();
  lcd.clear(); 
  lcd.print(" TRAB CIRCUITOS ");    lcd.setCursor(0,1);   lcd.print(" SISTEMA ARREF.");
  delay(3000);   
  lcd.clear();    lcd.home(); 
 
  //Configurações do Timer 1 | Prescaler 1:256 | Overflow em 500 ms
  TCCR1A = 0; TCCR1B|=(1<<CS12); TCCR1B&=(1<<CS11); TCCR1B&=(1<<CS10); OCR1A = 31250;  
  digitalWrite(MOTOR_FUN, HIGH);
  digitalWrite(FALHA, LOW);
  digitalWrite(VENT, LOW);
}

//Função de loop
void loop()
{
   motor = ((analogRead(SENSOR_TEMP_MOTOR)*(5.0/1023))/0.01);
  
  if ( motor!=motor_comp)
  {
    motor_comp = motor;
    lcd.clear();
    lcd.home();
     lcd.print("   ");lcd.print("TEMP. MOTOR ");
    lcd.setCursor(0,1); lcd.print("      "); lcd.print(motor,1); lcd.print(" C");
    delay(1000);
  }
  
  if(motor < 40)
  {
    TIMSK1 = (0<<OCIE1A);
    digitalWrite(MOTOR_FUN, HIGH); digitalWrite(FALHA, LOW); digitalWrite(VENT, LOW); 
  }
  if (motor>=40 && motor < 60)
  {
    TIMSK1 = (0<<OCIE1A);
    digitalWrite(MOTOR_FUN, HIGH); digitalWrite(FALHA, LOW);digitalWrite(VENT, HIGH);
  } 
  if (motor>=60)
  {
   TIMSK1 = (0<<OCIE1A);
    
    digitalWrite(MOTOR_FUN, HIGH); digitalWrite(FALHA,HIGH); digitalWrite(VENT, HIGH);
    lcd.clear();    lcd.home();
    lcd.print("ATENCAO FALHA");    lcd.setCursor(0,1); lcd.print("PARE O VEICULO");    
    
  } 

}   

//Função de interrupção por overflow do Timer 1
ISR(TIMER1_COMPA_vect)
{
    ciclos = ciclos + 1;
    TCNT1 = 0;  
}