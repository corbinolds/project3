#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <iostream>

#include "simulator.h"

// ***********************************************************
// ** If you want to define other fucntions, put them here.
// ***********************************************************
void A_init();
void B_init();

void A_output(struct msg message);
void B_output(struct msg message);  /* need be completed only for extra credit */

void A_input(struct pkt packet);
void B_input(struct pkt packet);

void A_timerinterrupt();
void B_timerinterrupt();

// ***********************************************************
// ** If you want to define other fucntions, put them here.
// ***********************************************************
