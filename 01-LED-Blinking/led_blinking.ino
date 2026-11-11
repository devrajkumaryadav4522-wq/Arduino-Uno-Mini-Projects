//Led-Blinking-code
//cpp
void setup ( ) 
{
  pinMode (7, HIGH);
}
void loop ( )
{
  digitalWrite (7, HIGH);
delay (1000); //wait for 1000 milisecond 
gigitalWrite (7, LOW);
delay (1000); //wait for 1000 milisecond 
}
