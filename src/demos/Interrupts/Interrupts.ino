void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  {
    Serial.print("sizeof()=");
    Serial.println(sizeof(SREG));
  }

  {
    Serial.print("default=");
    Serial.println((int)SREG);
  }

  {
    noInterrupts();
    uint8_t disabled = SREG;
    interrupts();
    Serial.print("noInterrupts()=");
    Serial.println((int)disabled);
  }

  {
    interrupts();
    uint8_t enabled = SREG;
    Serial.print("interrupts()=");
    Serial.println((int)enabled);
  }

  {
    cli();
    uint8_t disabled = SREG;
    interrupts();
    Serial.print("cli()=");
    Serial.println((int)disabled);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
