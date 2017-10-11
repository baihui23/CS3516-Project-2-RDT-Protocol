#include <stdio.h>
#include <stdlib.h>
#include "project2.h"
#include <string.h>

/* ***************************************************************************
ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

This code should be used for unidirectional or bidirectional
data transfer protocols from A to B and B to A.
Network properties:
- one way network delay averages five time units (longer if there
are other messages in the channel for GBN), but can be larger
- packets can be corrupted (either the header or the data portion)
or lost, according to user-defined probabilities
- packets may be delivered out of order.

Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/

int lastPacket;
struct msg message;
struct pkt ACKPacket;

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/*
* The routines you will write are detailed below. As noted above,
* such procedures in real-life would be part of the operating system,
* and would be called by other procedures in the operating system.
* All these routines are in layer 4.
*/


/*
* Just like A_output, but residing on the B side.  USED only when the
* implementation is bi-directional.
*/
void B_output(struct msg message)  {

}


/*
* Note that with simplex transfer from A-to-B, there is no routine  B_output()
*/

/*
* B_input(packet),where packet is a structure of type pkt. This routine
* will be called whenever a packet sent from the A-side (i.e., as a result
* of a tolayer3() being done by a A-side procedure) arrives at the B-side.
* packet is the (possibly corrupted) packet sent from the A-side.
*/

/* STEP 1: Update sequence number and send ACK
* STEP 2: Process the message and send to layer 5
*/

void B_input(struct pkt packet) {


  /* checksum is correct */
  if(calculateChecksum(packet) != packet.checksum ||
    ( packet.seqnum != 0 && packet.seqnum != 1 ) ||
    ( packet.acknum != 0 && packet.acknum != 1 ) ) {

      /* DEBUG */
    if( DEBUGLEVEL > 1 ){
       printf("B->A: sn# %d, msg: %.20s, isCorrupt\n", packet.seqnum, packet.payload);
    }

    /* send packet */
    sendMessageB(ACKPacket);

  } else {

    /* sequence number is same as previous packet */
    if( packet.seqnum == lastPacket ) {

        /* DEBUG */
      if( DEBUGLEVEL > 1 ){
        printf("B->A: sn# %d, msg: %.20s, waiting...: %d, poor sequence number\n", packet.seqnum, packet.payload, ((lastPacket + 1) % 2));
      }

      /* send packet */
      sendMessageB(ACKPacket);

    } else {

        /* DEBUG */
      if( DEBUGLEVEL > 1 ) {
        printf("B->A: sn# %d, msg %.20s, good\n", packet.seqnum, packet.payload);
        printf("Sequence Number (old): %d, Sequence Number (new): %d; in the new Packet\n", lastPacket, packet.seqnum);
      }
        /* new sequence number becomes old */
      lastPacket = packet.seqnum;
        /* send OK Packet */
      sendMessageB(ACKPacket);
      /* copy message */
      strncpy(message.data, packet.payload, 20);
      /* send to layer 5 */
      tolayer5(BEntity, message);
    }
  }

}

/*
* B_timerinterrupt()  This routine will be called when B's timer expires
* (thus generating a timer interrupt). You'll probably want to use this
* routine to control the retransmission of packets. See starttimer()
* and stoptimer() in the writeup for how the timer is started and stopped.
*/
void  B_timerinterrupt() {
}

/*
* The following routine will be called once (only) before any other
* entity B routines are called. You can use it to do any initialization
*/
void B_init() {
  /* DEBUG */
  if( DEBUGLEVEL > 1 ){
      printf("SequenceNumber(old): %d, SequenceNumber(new): %d; initial\n", lastPacket, 0);
  }
  /* initial */
  lastPacket = 0;

}
