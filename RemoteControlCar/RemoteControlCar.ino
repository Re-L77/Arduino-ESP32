/*
  ESP32
*/

int a = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println(a);
  delay(50);
  a++;
  if (a > 100)
  {
    a = 0;
  }
}
