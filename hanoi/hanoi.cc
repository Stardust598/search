#include "hanoi.hpp"
#include "../utils/utils.hpp"
#include <cstdio>
#include <cerrno>
#include <iostream>

const Hanoi::Oper Hanoi::Nop;

Hanoi::Hanoi(FILE *in) {
	unsigned int Ndisks;

	if (fscanf(in, "%d\n", &Ndisks) != 1){
		fatalx(errno, "Failed to read the number of disks");
}
/*char str[100];
	int result = fscanf(in, "Initial:");
	fatal("%d", result);*/
    if (fscanf(in, "Initial:\n")!=0) {
	  fatal("Missing Initial header line in input file.\n");
	}
    for (unsigned int i=0; i<Ndisks; i++){
      if (fscanf(in, "%u\n", &init[i])!=1)
        fatal("failed to read a disk value\n");
    }

    if (fscanf(in, "Goal:\n")!=0) {
      fatal("Missing Stack Goal header line in input file.\n");
    }

for(Disk from = 0; from < ARRAY_SIZE; from++){ //creates a moves library
    for(Disk to = 0; to < ARRAY_SIZE; to++){
        if(from != to) movelibrary[(from)* ARRAY_SIZE + to] = Move(from, to);
        }
      }
    }


Hanoi::State Hanoi::initialstate() {
	State s;
  for (unsigned int i=0; i<Ndisks;i++){
    Piece* piece = new Piece();
    piece->data = i;
    if (s.towers[init[i]].head == NULL){ //if nothing, start list
      s.towers[init[i]].head = piece;
    }
    else {
      Piece* current = s.towers[init[i]].head;  //else go through to find end of list and add there
      while (current->next != NULL){
        current = current->next;
        }
      current->next = piece;
      }
    s.towers[init[i]].length+=1;
  }
      int peg3 = 0; //calculate initial huristic
      if (s.towers[2].length!=0){ //go through and calculate huristic
        Piece* here=s.towers[2].head;
        int tempsum=0;
        int sum2= 0;
        while (here->next != NULL){
					//fprintf(stdout, "here\n");
          tempsum+=1;
          if (here->next->data!=here->data+1){ //if next not in line, add double all
            sum2=sum2+tempsum*2;
            tempsum=0;
          }
					here=here->next;
        }
        if (here->data!=Ndisks-1){ //if end not bottom, add double all
          tempsum+=1;
          sum2=sum2+tempsum*2;
        }
				/*else if (tempsum==Ndisks){ //if end is bottom and all are correct
					sum2=0;
				}*/
        peg3=sum2;

      }
      s.h = s.towers[0].length+s.towers[1].length+peg3;
      s.d = s.h;
  	return s;
      }

Hanoi::Cost Hanoi::pathcost(const std::vector<State> &path, const std::vector<Oper> &ops) { //unmodified
	State state = initialstate();
	dumpstate(stdout, path[1]);
	Cost cost(0);
	for (int i = ops.size() - 1; i >= 0; i--) {
		State copy(state);
		Edge e(*this, copy, ops[i]);
		//fprintf(stdout, "%d\n", e.cost);
		//dumpstate(stdout, e.state);
		//dumpstate(stdout, path[i]);
		//assert (e.state.eq(this, path[i]));
		state = e.state;
		cost += e.cost;

	}
	assert (isgoal(state));
	return cost;
}
