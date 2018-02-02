void setup() {
  // put your setup code here, to run once:

  uint32_t now = 16;
  uint32_t startTime = 0xffffff08;
  uint32_t diff = now - startTime;

  Serial.begin(115200);
  Serial.println("ready");
  Serial.println(diff);
}

void loop() {
  // put your main code here, to run repeatedly:

}
