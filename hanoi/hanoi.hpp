
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

	struct Piece { //the nodes in the linked lists
		int data;
		Piece* next=NULL;
	};

	struct State {

		unsigned long hash (const Hanoi*) const{ //simple hash function using tower lengths
			return towers[0].length+2*towers[1].length+Ndisks*towers[2].length;
		}

/* This code is trying to force the packstate to do a deep copy.
It doesn't work propperly. To fix this domain, either fix this
or change state representation to be array based rather than linked lists.
		State& operator=(const State &other){
			for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
				fprintf(stdout, "Stack %u:", i);
				Piece* otherHead = other.towers[i].head;
				if (otherHead==NULL){
					towers[i].head=NULL;
					towers[i].length=0;
					continue;
				}
				Piece* head = new Piece();
				head->data=otherHead->data;
				fprintf(stdout, "%u\n", head->data);
				towers[i].length=other.towers[i].length;
				towers[i].head=head;
				head->next=NULL;
				otherHead=otherHead->next;
				while (otherHead!=NULL){
					Piece* p= new Piece();
					p->next=NULL;
					p->data=otherHead->data;
					fprintf(stdout, "%u\n", p->data);
					head->next=p;
					head=p;
					otherHead=otherHead->next;
				}
			}
			fprintf(stdout, "towers length: %u %u %u\n", towers[0].length, towers[1].length, towers[2].length);
			return *this;
		}
		*/

//This equvilancy function fails because the states copy incorrectly
		bool eq(const Hanoi*, const State &o) const { //Check if states are equivilent
			for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
				Piece* current = towers[i].head;
				Piece* other = o.towers[i].head;
				if (towers[i].length != o.towers[i].length){
					return false;
				}
				for (int j=0; j < towers[i].length; j++){
					if (current->data != other->data){
					return false;
					}
				current=current->next;
				other=other->next;
				}
			}
				return true;
 		}

			void movedisk(Oper move, const Hanoi &domain)	{ //function that actually moves the disks

				 	Disk pickUp = domain.movelibrary[move].from;
					Disk putOn = domain.movelibrary[move].to;
					assert (towers[pickUp].head!=NULL);
					Piece* disk = towers[pickUp].head;
					Piece* previous = towers[pickUp].head->next; //used to assert that move was done correctly
					Piece* under = towers[putOn].head; //used to assert that move was done correctly
					int hadj=0;  //heuristic adjustment

					if (pickUp==2){ //if removing disk from stack 3
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								hadj=-1; //was not in place
								break;
							}
							disk=disk->next;
						}
							if (hadj==0 && disk->data==Ndisks-1){
								hadj=1; //was in place
							}
							else {
								hadj=-1;  //was not in place
							}
						disk=towers[pickUp].head; //reset disk back to head
					}

					towers[pickUp].head=towers[pickUp].head->next; //removes node from stack
					disk->next=NULL;
					towers[pickUp].length-=1; //decrease length of old stack

					disk->next=towers[putOn].head; //add the node to new stack
					towers[putOn].head=disk;
					towers[putOn].length+=1; //increases length of new stack

					if (putOn==2){     //if added piece to 3rd stack
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								hadj=1; //not in place
								break;
							}
						disk=disk->next;
						}
						if (hadj==0 && disk->data==Ndisks-1){
							hadj=-1; //in place
						}
						else {
							hadj=1;  //not in place
						}
					}
				h=h+hadj; //adjust hueristic

				assert (towers[putOn].head->next==under); //check that move done correctly
				assert (towers[pickUp].head==previous);
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
			n=3;
			if (s.towers[0].length==Ndisks || s.towers[1].length==Ndisks || s.towers[2].length==Ndisks){
				n=ARRAY_SIZE-1; //min # of moves
			}
			else{
				n=3; //true for Ndisks=3, does not scale up
			}
			int pos=0;
			Oper * temp = new Oper[n];
			mvs = temp;
			int a=Ndisks;
			int b=Ndisks;

			for (int i=0;i<ARRAY_SIZE-1; i++){    //Go through all possible moves
				if (s.towers[i].head != NULL){ //check if head exsists
					a=s.towers[i].head->data;

				}
				for (int j=1;j<ARRAY_SIZE;j++){
					if (s.towers[j].head != NULL){
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
					b=Ndisks; //reset b
		}
		a=Ndisks; //reset a
	}
}
		// size returns the number of applicable operatiors.
		unsigned int size() const {
			return n; //eventually variable corrisponding to number of pegs
		}

		// operator[] returns a specific operator.
		Oper operator[] (unsigned int i) const {
			return mvs[i];
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
		return (from)*(ARRAY_SIZE) + to;
	}
//
	void pack(PackedState &dst, State &s) const {
		dst = s; //copies State to Packedstate
	}

	State &unpack(State &buf, PackedState &pkd) const {
		return pkd; //returns Packedstate
	}
//
	// Prints the state.
	void dumpstate(FILE *out, const State &s) const {
		for (int i=0; i < ARRAY_SIZE; i++){
			fprintf(out, "Stack %d: ", i);
			fflush(stdout);
			Piece* here = s.towers[i].head;
			for (int j=0; here!=NULL; j++){
				fprintf(out, "%u ", here->data); //will seg fault here due to incorrect copying
				if (here->next==NULL){
					fprintf(out, "end of stack");
				}
				fflush(stdout);
				here = here->next;
			}
			fprintf(out, "\n");
			fflush(stdout);
		}
		fprintf(out, "heuristic value: %d\n", s.h);
		fflush(stdout);
	}

	Cost pathcost(const std::vector<State>&, const std::vector<Oper>&);
private:
	int init[Ndisks];
	Move movelibrary[ARRAY_SIZE*ARRAY_SIZE]={Move()};
};
