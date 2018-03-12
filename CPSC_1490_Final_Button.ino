int button = 0;
bool lastButtonState = 1;
bool onOff = 0;

void setup()
{
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

bool checkButton() {
  bool buttonState = digitalRead(12); // read from button
  if (!lastButtonState && buttonState) { // if state changed from 0 to 1, set lastButtonState to buttonState and return 1
    lastButtonState = buttonState;
    return 1;
  }
  else { // else, set lastButtonState to buttonState and return zero
    lastButtonState = buttonState;
    return 0;
  }
}

void loop() {
  bool button = checkButton();
  if (button) {
    if (onOff)
      onOff = 0;
    else
      onOff = 1;
  }
  if (onOff)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}
