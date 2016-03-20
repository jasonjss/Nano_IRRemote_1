#include <IRremote.h>

#define repeatcode 0xFFFFFFFF
#define repeatstep 2

const int irReceiverPin = 8;

IRrecv irrecv(irReceiverPin);
decode_results results;
unsigned long last_value;
int last_bits;
decode_type_t last_decode_type;
int repeatcounter;

void setup()
{
  repeatcounter = 0;
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results)) {
    if ( results.decode_type != UNKNOWN) {
      if (results.value != repeatcode) {
        SendSerialData(results.value, results.bits, results.decode_type);
        last_value = results.value;
        last_bits = results.bits;
        last_decode_type = results.decode_type;
        repeatcounter = 0;
      } else {
        repeatcounter++;
        if (repeatcounter > repeatstep) {
          SendSerialData(last_value, last_bits, last_decode_type);
          //SendSerialData(results.value, results.bits, results.decode_type);
          repeatcounter = 0;
        }
      }
    }
    irrecv.resume();
  }
}

void SendSerialData(unsigned long value, int bits, decode_type_t decode_type)
{
  char sendbuffer[18];
  sprintf(sendbuffer, "%08lX,%02d,%02d\r\n", value, bits, decode_type);
  Serial.print(sendbuffer);
}

