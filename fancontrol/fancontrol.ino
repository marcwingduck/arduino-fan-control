// 25kHz PWM Double Fan Control.

const int min_duty = 20;  // duty cycle range in percent
const int max_duty = 100; // duty cycle range in percent

int percent = 0;
int potval = 0;

// Configure Timer 1 for 25 kHz PWM.
// (from https://arduino.stackexchange.com/a/25623)
void init25k()
{

    TCCR1A = 0;            // undo the configuration done by...
    TCCR1B = 0;            // ...the Arduino core library
    TCNT1 = 0;             // reset timer
    TCCR1A = _BV(COM1A1)   // non-inverted PWM on channel A
             | _BV(COM1B1) // same on channel B
             | _BV(WGM11); // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)    // ditto
             | _BV(CS10);  // prescaler = 1
    ICR1 = 320;            // TOP = 320
}

// 25 kHz PWM only on pins 9 and 10.
// Value should be between 0 and 320, inclusive.
// (from https://arduino.stackexchange.com/a/25623)
void analogWrite25k(int pin, int value)
{
    switch (pin)
    {
    case 9:
        OCR1A = value;
        break;
    case 10:
        OCR1B = value;
        break;
    default:
        // no other pin will work
        break;
    }
}

void setup()
{
    init25k();
    pinMode(2, OUTPUT);  // pot
    pinMode(9, OUTPUT);  // fan 1
    pinMode(10, OUTPUT); // fan 2
}

void loop()
{
    // min..max duty cycle
    percent = constrain(map(analogRead(2), 0, 1023, 0, 100), min_duty, max_duty);
    // map to bottom..top
    potval = map(percent, 0, 100, 0, 320);

    analogWrite25k(9, potval);
    analogWrite25k(10, potval);

    delay(1000);
}