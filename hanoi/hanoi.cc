#include "hanoi.hpp"
#include "../utils/utils.hpp"
#include <cstdio>
#include <cerrno>
#include <iostream>

const Hanoi::Oper Hanoi::Nop;

Hanoi::Hanoi(FILE *in) {
	unsigned int Ndisks;

	if (fscanf(in, "%u\n", &Ndisks) != 1)
		fatalx(errno, "Failed to read the number of disks");

    if (!fscanf(in, "Stack A\n")) {
	  fatal("Missing Stack A header line in input file.\n");
	}
  //
  int i=0;
    while (!fscanf(in, "Stack B\n")) {
      fscanf(in, "%hhu\n", &atower[i]);
      i++;
    }

    while (!fscanf(in, "Stack C\n")) {
      fscanf(in, "%hhu\n", &btower[i]);
      i++;
    }

    while (!fscanf(in, "Goal:\n")) {
      fscanf(in, "%hhu\n", &ctower[i]);
      i++;
    }
//
    if (!fscanf(in, "Stack A\n")) {
        fatal("Missing Stack A goal header line in input file.\n");
        }
  i=0;
   while (!fscanf(in, "Stack B\n")) {
      fscanf(in, "%hhu\n", &agoal[i]);
      i++;
    }

    while (!fscanf(in, "Stack C\n")) {
      fscanf(in, "%hhu\n", &bgoal[i]);
      i++;
    }

    while (!fscanf(in, "\n")) {
      fscanf(in, "%hhu\n", &cgoal[i]);
      i++;
    }

for(Disk from = 0; from < Ndisks; from++){
    for(Disk to = 0; to < Ndisks; to++){
        if(from != to) movelibrary[(from)* Ndisks + to] = Move(from, to);
        }
      }
    }

// potentially create a move library here
Hanoi::State Hanoi::initialstate() {
	State s;
  int* list[3]={atower, btower, ctower};
  for (int j=0; j<3; j++){
  	for (int i=0; i<len(list[j]);i++){
      Node* node = new Node;
      node->data = i;
      node->next = NULL;
      if (s.towers[j].head == NULL){ //if nothing, start list
        s.towers[j].head == node;
      }
      else {
        Node* current = s.towers[0].head;  //else go through to find end of list and add there
        while (current->next != NULL){
          current = current->next;
        }
        current->next = node;
        s.towers[j].lenth+=1; //add 1 to length
      }
    }
  }
      int peg3 = 0;
      if (s.towers[0].length==0){  //calculate initial huristic value
      }
      else{
        Node* here=s.towers[2].head;
        int tempsum=0;
        int sum2= 0;
        while (here->next != NULL){
          tempsum+=1;
          if (here.next.data!=here.data+1){
            sum2=sum2+tempsum*2;
            tempsum=0;
          }
        }
        if (here.data!=Ndisks){
          tempsum+=1;
          sum2=sum2+tempsum*2;
        }
        peg3=sum2;

      }
      s.h = s.towers[0].length+s.towers[1].length+peg3;
      s.d = s.h;
  	return s;
      }

Hanoi::Cost Hanoi::pathcost(const std::vector<State> &path, const std::vector<Oper> &ops) { //unmodified
	State state = initialstate();
	Cost cost(0);
	for (int i = ops.size() - 1; i >= 0; i--) {
		State copy(state);
		Edge e(*this, copy, ops[i]);
		assert (e.state.eq(this, path[i]));
		state = e.state;
		cost += e.cost;
	}
	assert (isgoal(state));
	return cost;
}
