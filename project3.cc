#include "project3.h"

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

simulator *simulation;

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  std::cout << "A side has recieved a message from the application that should be sent to side B: " << message.data << std::endl;
  
  pkt packet;
  
  for(int i = 0; i < sizeof(message.data); i++) {
  	packet.payload[i] = message.data[i];
  }
  
  simulation->tolayer3(A, packet);

}

void B_output(struct msg message)  /* need be completed only for extra credit */
{
  std::cout << "B side has recieved a message from the application that should be sent to sideA: " << message.data << std::endl;

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  std::cout << "A side has recieved a packet sent over the network from side B:" << packet.payload << std::endl;

}


/* called when A's timer goes off */
void A_timerinterrupt()
{

  std::cout << "Side A's timer has gone off " << std::endl;
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
}


/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
    std::cout << "B side has recieved a packet sent over the network from side A:" << packet.payload << std::endl;
    
    //pkt packet;
	  
	//for(int i = 0; i < sizeof(message.data); i++) {
	//	packet.payload[i] = message.data[i];
	//}
	  
	simulation->tolayer3(B, packet);
}

/* called when B's timer goes off */
void B_timerinterrupt()
{
    std::cout << "Side B's timer has gone off " << std::endl;

}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
}

int main(int argc, char **argv) {

  simulation = new simulator;

  A_init();
  B_init();
  simulation->go();
}
