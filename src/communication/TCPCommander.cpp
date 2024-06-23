#include "Commander.h"

TCPCommander::TCPCommander(WiFiServer *ser, char eol, bool echo){
    this->server = ser;
    Commander(eol, echo);
};

void TCPCommander::run(){
    if(clientvalid){
        if(client.connected()) run(client, eol);
        else {
            clientvalid=false;
            client.stop();
        }
    }else{
        client = server->available();
        if (client) clientvalid=true;    
    }

};

void TCPCommander::run(WiFiClient cl, char eol){
  WiFiClient tmp = client; // save the serial instance
  char eol_tmp = this->eol;
  this->eol = eol;
  client = cl;

  // a string to hold incoming data
  while (cl.available()) {
    // get the new byte:
    int ch = cl.read();
    received_chars[rec_cnt++] = (char)ch;
    // end of user input
    if(echo)
      print((char)ch);
    if (isSentinel(ch)) {
      // execute the user command
      Commander::run(received_chars);

      // reset the command buffer
      received_chars[0] = 0;
      rec_cnt=0;
    }
    if (rec_cnt>=MAX_COMMAND_LENGTH) { // prevent buffer overrun if message is too long
        received_chars[0] = 0;
        rec_cnt=0;
    }
  }

  client = tmp; // reset the instance to the internal value
  this->eol = eol_tmp;
}