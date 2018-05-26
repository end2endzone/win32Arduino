void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("ready");

  #define SHOW_CONSTANT(constant) Serial.print(#constant); \
                                  Serial.print(" = "); \
                                  Serial.print((int)constant); \
                                  Serial.print(", "); \
                                  Serial.print(sizeof(constant)); \
                                  Serial.println(" bytes");

  SHOW_CONSTANT(HIGH);
  SHOW_CONSTANT(LOW);
  SHOW_CONSTANT(INPUT);
  SHOW_CONSTANT(OUTPUT);
  SHOW_CONSTANT(INPUT_PULLUP);
  //SHOW_CONSTANT(SERIAL);      //TODO: Implement this constant
  //SHOW_CONSTANT(DISPLAY);     //TODO: Implement this constant
  SHOW_CONSTANT(LSBFIRST);
  SHOW_CONSTANT(MSBFIRST);
  SHOW_CONSTANT(CHANGE);
  SHOW_CONSTANT(FALLING);
  SHOW_CONSTANT(RISING);
  //SHOW_CONSTANT(DEFAULT);     //TODO: Implement this constant
  //SHOW_CONSTANT(EXTERNAL);    //TODO: Implement this constant
  //SHOW_CONSTANT(INTERNAL);    //TODO: Implement this constant

}

void loop() {
  // put your main code here, to run repeatedly:

}
