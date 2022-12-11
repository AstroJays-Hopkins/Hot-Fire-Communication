void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Serial1.begin(9600);
  Serial.begin(9600);

  digitalWrite(3, HIGH); // transmit on 
  digitalWrite(2, LOW); // receive off

}
int i = 0;
void loop() {
  Serial1.write(0x53);
  //Serial1.print(i);
  //i++;
  //i = i % 99999;
  Serial1.print("hello");
  delay(50);

}

void sendchar(char c[]) { 
  digitalWrite(3, HIGH); // transmit on 
  digitalWrite(2, LOW); // receive off
  Serial1.println(c);
}

void readrequest() {
  
}

void writerequest(char data[]) {

}