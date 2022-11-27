void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  Serial1.begin(9600);
  

}

void loop() {
  sendchar("hello");

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