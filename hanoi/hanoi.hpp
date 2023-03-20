// Here is the class template for creating a new domain.

#include "../utils/utils.hpp"
#include <cstdio>

#define MAXDISKS 250
#if NDISKS > MAXDISKS
#error Too many disks
#endif
const int ARRAY_SIZE = 3; //eventually updated to variable amount of Pillars

class Hanoi {
public:

	typedef int Cost; //The type for how to represent move costs
	typedef int Oper;  //the type for how to represent operations
	typedef unsigned int Disk; //the type for disks

	enum{Ndisks = NDISKS}; //how many disks was it made with

	struct Move{     //struct for from and to moves
			Disk from;
			Disk to;

			Move() : from(0), to(0){}
			Move(Disk p1, Disk p2){
					from = p1;
					to = p2;
			}

			bool operator==(const Move &o) const {
		return from == o.from && to == o.to;
			}
	};

	static const int Nop =-1; //Set so oper -1 is a do nothing action

	Hanoi(FILE*);

	struct Piece {
		int data;
		Piece* next=NULL;
	};

	struct State {

		unsigned long hash (const Hanoi*) const{
			return towers[0].length+2*towers[1].length+Ndisks*towers[2].length;
		}


		bool eq(const Hanoi*, const State &o) const { //Check if states are equivilent
			//for (unsigned int i = 0; i < Ndisks; i++) {
				if (towers != o.towers){
					return false;
				}
				return true;
			}

			void movedisk(Oper move, const Hanoi &domain)	{ //fix- should adjust the hueristic here

				 	Disk pickUp = domain.movelibrary[move].from;
					Disk putOn = domain.movelibrary[move].to;
					Piece* disk = towers[pickUp].head;
					int hadj;

					if (pickUp==2){
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								hadj=-1; //not in place
								break;
							}
							disk=disk->next;
						}
							if (disk->data==Ndisks){
								hadj=1; //in place
							}
							else {
								hadj=1;  //not in place
							}
						disk=towers[pickUp].head;
					}

					if (towers[pickUp].head->next){  //remove the node from stack
						towers[pickUp].head=towers[pickUp].head->next;
					}
					else{
						towers[pickUp].head=NULL; //if only thing in stack, stack empty
					}
					towers[pickUp].length-=1; //decrease length of stack

					disk->next=towers[putOn].head; //add the node to new stack
					towers[putOn].head=disk;
					towers[putOn].length+=1;
					if (putOn==2){
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								hadj=1; //not in place
								break;
							}
						disk=disk->next;
						}
						if (disk->data==Ndisks){
							hadj=-1; //in place
						}
						else {
							hadj=1;  //not in place
						}
					}
				h=h+hadj;
			}

private:
	friend class Hanoi;

	struct Pillar{
		Piece* head=NULL;
		int length=0;
	};

	Pillar towers[ARRAY_SIZE]; //towers array

	Cost h;
	Cost d;



};

	typedef State PackedState; //Packed State is the same as State

	/*struct PackedState {


		// Functions for putting a packed state
		// into a hash table.
		bool operator==(const PackedState &) const {
			return false;
		}
	};

	unsigned long hash(const PackedState&) const {
		return -1;
	}*/



	// Get the initial state.
	State initialstate();

	// Get the heuristic.
	Cost h(const State &s) const {
		return s.h;
	}

	// Get a distance estimate.
	Cost d(const State &s) const {
		return s.d;
	}

	// Is the given state a goal state?
	bool isgoal(const State &s) const {
		return s.h==0;
	}

	// Operators implements an vector of the applicable
	// operators for a given state.
	struct Operators {
		Operators(const Hanoi& h, const State& s){
			int n=3;
			if (s.towers[0].length or s.towers[1].length or s.towers[2].length==Ndisks){
				n=Ndisks-1; //min # of moves
			}
			else{
				n=3; //true for Ndisks=3, does not scale up
			}
			int pos=0;
			Oper * temp = new Oper[n];
			mvs = temp;
			int a=0;
			int b=0;

			for (int i=0;i<Ndisks-1; i++){    //Go through all possible moves
				if (s.towers[i].head == NULL){ //check if head exsists
					a=0;
				}
				else{
					a=s.towers[i].head->data;
				}
				for (int j=0;Ndisks-1;j++){
					if (s.towers[j].head == NULL){
						b=0;
					}
					else{
						b=s.towers[j].head->data;
					}
					if(a<b){                        //if move possible
						mvs[pos]=h.getmoveref(i,j);
						pos+=1;
					}
					else if (i!=j){                //else, inverse
						mvs[pos]=h.getmoveref(j,i);
						pos+=1;
					}
		}
	}
}
		// size returns the number of applicable operatiors.
		unsigned int size() const {
			return n; //eventually variable corrisponding to number of pegs
		}

		// operator[] returns a specific operator.
		Oper operator[] (unsigned int i) const {
			return mvs[i]; //Not sure what this does
		}
	private:
		Oper *mvs;
		int n;
	};

	struct Edge {
		Cost cost;
		Oper revop;
		Cost revcost;

		State &state;
			Hanoi &domain;


		// Applys the operator to thet state.  Some domains
		// may modify the input state in this constructor.
		// Because of this, a search algorithm may not
		// use the state passed to this constructor until
		// after the Edge's destructor has been called!
		Edge(Hanoi& d, State &s, Oper move):
				cost(1), revop(d.getmoveref(d.movelibrary[move].to, d.movelibrary[move].from)),
				revcost(1), state(s), domain(d){
					state.movedisk(move, domain);
				}

		// The destructor is expected to undo any changes
		// that it may have made to the input state in
		// the constructor.  If a domain uses out-of-place
		// modification then the destructor may not be
		// required.
		~Edge(void) {
				state.movedisk(revop, domain);
		}
	};

	Oper getmoveref(Disk from, Disk to) const {
		return (from)*(Ndisks) + to;
	}
//
	void pack(PackedState &dst, State &s) const {
		dst = s; //copies State to Packedstate
	}

	State &unpack(State &buf, PackedState &pkd) const {
		return pkd; //returns Packedstate
	}
//
	// Print the state.
	void dumpstate(FILE *out, const State &s) const {
		for (int i=0; i < ARRAY_SIZE; i++){
			fprintf(out, "Stack: %d", i);
			Piece* here = s.towers[i].head;
			for (int j=0; j<s.towers[i].length; j++){
				fprintf(out, "%u", here->data);
				here = here->next;
			}
		}
		fprintf(out, "huristic value: %d", s.h);
	}

	Cost pathcost(const std::vector<State>&, const std::vector<Oper>&);
private:
	int init[Ndisks];
	Move movelibrary[Ndisks*Ndisks]={Move()};
};
