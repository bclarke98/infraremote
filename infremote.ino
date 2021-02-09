//------------------------------------------------------------------------------
// Include the IRremote library header
//
#include <IRremote.h>

//------------------------------------------------------------------------------
// Tell IRremote which Arduino pin is connected to the IR Receiver (TSOP4838)
//
int recvPin = 11;
IRrecv irrecv(recvPin);
IRsend irsend;

const int S_ULONG = 4;

unsigned long readULong() {
  union u_tag {
    byte b[S_ULONG];
    unsigned long ulval;
  } u;
  for (int i = 0; i < S_ULONG; u.b[i++] = Serial.read());
  return u.ulval;
}

//+=============================================================================
// Configure the Arduino
//
void setup(){
  Serial.begin(9600);   // Status message will be sent to PC at 9600 baud
  irrecv.enableIRIn();  // Start the receiver
}

//+=============================================================================
// Display IR code
//
void ircode (decode_results *results){
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void encoding(int type){
  switch (type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

void sendir(int type, unsigned long data){
  switch (type) {
      case NEC:
          irsend.sendNEC(data, 32);
          break ;
      case SONY:
          irsend.sendSony(data, 32);
          break ;
      case RC5:
          irsend.sendRC5(data, 32);
          break ;
      case RC6:
          irsend.sendRC6(data, 32);
          break ;
      case DISH:
          irsend.sendDISH(data, 32);
          break ;
      case SHARP:
          irsend.sendSharpRaw(data, 32);
          break ;
      case JVC:
          irsend.sendJVC(data, 32, false);
          break ;
      case SAMSUNG:
          irsend.sendSAMSUNG(data, 32);
          break ;
      case LG:
          irsend.sendLG(data, 32);
          break ;
      default:
          break;
  }
  irrecv.enableIRIn();
}

void dumpInfo(decode_results *results){
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }
  encoding(results->decode_type);
  Serial.print(":");
  ircode(results);
  Serial.println("");
}

void replay(decode_results *r){
  sendir(r->decode_type, r->value);
}

void checkSerial(){
  if(Serial.available() >= 6){
    int type = (int)(Serial.read());
    char colon = Serial.read();
    unsigned long code = readULong();
    sendir(type, code);
  }
}

void ir_in(){
  decode_results results;           // Somewhere to store the results
  if (irrecv.decode(&results)) {    // Grab an IR code
    dumpInfo(&results);             // Output the results
    replay(&results);               // Replay received data
    irrecv.resume();                // Prepare for the next value
  }
}

void loop(){
  ir_in();
  checkSerial();
}
