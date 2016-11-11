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
char buffer[1000][20]; 		//message data buffer
int sequenceNum = 0; 		//current sequenceNum for A
int catchUpSequenceNum = 0; 	//used to resend all packets up to the one that it was last trying to before failing/timer interrupting
int sequenceNumTimerTrack = 0; 	//keeps track of which sequence number was being sent when the timer is started.
bool aTimerRunning = false;	//used to keep track of whether or not the timer is running.
int alreadyAckd = 0;		//furthest packet ackd so far
int ackNumber = 1;		//keeps track of current ackNum for B

// cast the payload into integers and add that with the sequence number for A to B transfer.
int calcCheckSum (pkt packet) {
	int checkSum = 0;
	for (int i = 0; i < sizeof(packet.payload); i++) {
		checkSum += (int)packet.payload[i];
	}
	
	checkSum = checkSum + packet.seqnum + packet.acknum;
	
	return checkSum;
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
	for(int i = 0; i < sizeof(message.data); i++) {
		buffer[sequenceNum][i] = message.data[i];
	}
	
 	std::cout << "A side has recieved a message from the application that should be sent to side B: " << message.data << std::endl;

	pkt packet;
	  
	packet.seqnum = sequenceNum;
	
	//ack num doesn't matter for this so set it to 0
	packet.acknum = 0;
		  
	std::cout << "SEQ NUM: " << packet.seqnum << std::endl;
		  
	for(int i = 0; i < sizeof(buffer[sequenceNum]); i++) {
		packet.payload[i] = buffer[sequenceNum][i];
	}
	
	int checkSum = calcCheckSum(packet);
	packet.checksum = checkSum;
		  
	if(!aTimerRunning) {
		sequenceNumTimerTrack = sequenceNum;
		simulation->starttimer(A, 150.00);
		aTimerRunning = true;
	}
	simulation->tolayer3(A, packet);
		  	
	catchUpSequenceNum = sequenceNum;	  	
	sequenceNum++;
}

// Used to retry sending all packets up to the packet it most recently sent before failure/timer interrupt
void Reoutput_A(struct msg message)
{
	std::cout << "A side is retrying to send to side B: " << message.data << std::endl;

	while(sequenceNum < catchUpSequenceNum) {
		pkt packet;
	  
		packet.seqnum = sequenceNum;
	
		//ack num doesn't matter for this so set it to 0
		packet.acknum = 0;
		  
		std::cout << "SEQ NUM: " << packet.seqnum << std::endl;
		  
		for(int i = 0; i < sizeof(buffer[sequenceNum]); i++) {
			packet.payload[i] = buffer[sequenceNum][i];
		}
	
		int checkSum = calcCheckSum(packet);
	
		packet.checksum = checkSum;
		  
		if(!aTimerRunning) {
			sequenceNumTimerTrack = sequenceNum;
			simulation->starttimer(A, 150.00);
			aTimerRunning = true;
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
	
	int checkSumFromB = calcCheckSum(packet);
	
	// if B to A was corrupt, resend the packet after the one that was last ackd
	if(checkSumFromB != packet.checksum) {
		catchUpSequenceNum = sequenceNum;
		sequenceNum = alreadyAckd;
		msg message;
		
		std:: cout << "DATA CORRUPT. RESENDING LAST PACKET" << std::endl;		
		
		for(int i = 0; i < sizeof(buffer[sequenceNum]); i++) {
			message.data[i] = buffer[sequenceNum][i];
		}
		
		Reoutput_A(message);
	}
	// if not corrupt, proceed as normal
	else {	
		if (sequenceNumTimerTrack <= packet.acknum - 1) {
			simulation->stoptimer(A);
			aTimerRunning = false;
		}
		
		if (sequenceNum < packet.acknum) {
			sequenceNum = packet.acknum;
			catchUpSequenceNum = sequenceNum;
		}
	
		//keep track of packets already ackd
		if (packet.acknum > alreadyAckd) {
			alreadyAckd = packet.acknum;	
		}
	  	
	 	else if(packet.acknum <= alreadyAckd) {
	  		msg message;
		
			std:: cout << "ALREADY ACKED PACKET, RESENDING LAST PACKET" << std::endl;
			
			catchUpSequenceNum = sequenceNum;
			sequenceNum = packet.acknum;		
		
			for(int i = 0; i < sizeof(buffer[sequenceNum]); i++) {
				message.data[i] = buffer[sequenceNum][i];
			}
		
			Reoutput_A(message);	
	  		
	  	}
	}
}


/* called when A's timer goes off */
void A_timerinterrupt()
{	
	aTimerRunning = false;
	msg message;
	catchUpSequenceNum = sequenceNum;
	sequenceNum = sequenceNumTimerTrack;
	
	for(int i = 0; i < sizeof(buffer[sequenceNum]); i++) {
		message.data[i] = buffer[sequenceNum][i];
	}
	
	std::cout << "Side A's timer has gone off " << std::endl;
  
  	Reoutput_A(message);
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
	
	int checkSumFromA = calcCheckSum(packet);
	
	pkt ackPacket;
	
	//seqnum doesn't matter for b to a
	ackPacket.seqnum = 0;
	
	//if packet received from A is not corrupt
	if(checkSumFromA == packet.checksum) {
	    
		if(ackNumber == packet.seqnum + 1) {
		 	ackPacket.acknum = ackNumber;
		 	
		 	int checkSumToA = calcCheckSum(ackPacket);
			
			ackPacket.checksum = checkSumToA;
		 	
		 	std:: cout << "SEQ NUM: " << packet.seqnum << std::endl;
		 	std:: cout << "ACK NUM: " << ackPacket.acknum << std::endl;
		
			simulation->tolayer5(B, packet.payload);
			simulation->tolayer3(B, ackPacket);
			ackNumber++;
		}
		// else if seqnum doens't match next ack, resend the last ack
		else {
			ackPacket.acknum = ackNumber - 1;
			
			int checkSumToA = calcCheckSum(ackPacket);
			
			ackPacket.checksum = checkSumToA;
			
			if(ackNumber < packet.seqnum + 1) {
				std:: cout << "MISSING A PACKET. RESENDING LAST ACK" << std::endl;
			}else {
				std:: cout << "PACKET ALREADY ACKD. RESENDING LAST ACK" << std::endl;
			}
		
			std:: cout << "SEQ NUM: " << packet.seqnum << std::endl;
		 	std:: cout << "ACK NUM: " << ackPacket.acknum << std::endl;

			simulation->tolayer3(B, ackPacket);
			//don't increment ackNumber because B is either missing a packet or has already sent an ack for a packet
		}
	}
	//if corrupt, send last ack
	else {
		ackPacket.acknum = ackNumber - 1;
		
		int checkSumToA = calcCheckSum(ackPacket);
			
		ackPacket.checksum = checkSumToA;
		
		std:: cout << "DATA CORRUPT. RESENDING LAST ACK" << std::endl;
		
		std:: cout << "(corrupt) SEQ NUM: " << packet.seqnum << std::endl;
		std:: cout << "(corrupt) DATA: " << packet.payload << std::endl;
		std:: cout << "ACK NUM: " << ackPacket.acknum << std::endl;
		
		simulation->tolayer3(B, ackPacket);
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
