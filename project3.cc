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

// 
char buffer[1000][20];
int sequenceNum = 0;
int alreadyAckd = 0;
int ackNumber = 1;

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
	for(int i = 0; i < sizeof(message.data); i++) {
		buffer[sequenceNum][i] = message.data[i];
	}
	
  std::cout << "A side has recieved a message from the application that should be sent to side B: " << message.data << std::endl;
  
  if (alreadyAckd <= sequenceNum) {
  	pkt packet;
  
  	packet.seqnum = sequenceNum;
  
  	std::cout << "SEQ NUM: " << packet.seqnum << std::endl;
  
  	for(int i = 0; i < sizeof(message.data); i++) {
  		packet.payload[i] = message.data[i];
  	}
  
  	simulation->tolayer3(A, packet);
  	
  	sequenceNum++;
  }
}

void B_output(struct msg message)  /* need be completed only for extra credit */
{
  std::cout << "B side has recieved a message from the application that should be sent to sideA: " << message.data << std::endl;

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  std::cout << "A side has recieved a packet sent over the network from side B:" << packet.payload << std::endl;
	std::cout << "ACK NUM: " << packet.acknum << std::endl;
	
	if (packet.acknum == sequenceNum + 1) {
		alreadyAckd = sequenceNum;	

  }
  else if(packet.acknum > sequenceNum + 1) {
  	alreadyAckd = sequenceNum;
  
  	sequenceNum = packet.acknum;
  }

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
    
    pkt ackPacket;
    
    if(ackNumber == packet.seqnum + 1) {
		 	ackPacket.acknum = packet.seqnum + 1;
		 	
		 	std:: cout << "SEQ NUM: " << packet.seqnum << std::endl;
		 	std:: cout << "ACK NUM: " << ackPacket.acknum << std::endl;
			
			
			simulation->tolayer3(B, ackPacket);
			ackNumber++;
		}
		// else if seqnum doens't match next ack, resend the last ack
		else {
			ackPacket.acknum = ackNumber;
			
			if(ackNumber < packet.seqnum + 1) {
				std:: cout << "MISSING A PACKET. RESENDING LAST ACK" << std::endl;
			}else {
				std:: cout << "PACKET ALREADY ACKD" << std::endl;
			}

			simulation->tolayer3(B, ackPacket);
			//don't increment ackNumber because B is either missing a packet or has already sent an ack for a packet
		}
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
