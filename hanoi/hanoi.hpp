
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

		State(){}

		State(const State &other){
			for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
				Piece* otherHead = other.towers[i].head;
				if (otherHead==NULL){
					continue;
				}
				Piece* head = new Piece();
				head->data=otherHead->data;
				towers[i].length=other.towers[i].length;
				towers[i].head=head;
				otherHead=otherHead->next;
				while (otherHead!=NULL){
					Piece* p= new Piece();
					p->data=otherHead->data;
					head->next=p;
					head=p;
					otherHead=otherHead->next;
				}
			}
		}

		bool eq(const Hanoi*, const State &o) const { //Check if states are equivilent
			for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
				Piece* current = towers[i].head;
				Piece* other = o.towers[i].head;
				/*if ((current && !other) || (other && !current)){
					return false;
				}*/
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

			void movedisk(Oper move, const Hanoi &domain)	{ //fix- should adjust the hueristic here

				 	Disk pickUp = domain.movelibrary[move].from;
					Disk putOn = domain.movelibrary[move].to;
					assert (towers[pickUp].head!=NULL);
					Piece* disk = towers[pickUp].head;
					Piece* previous = towers[pickUp].head->next;
					Piece* under = towers[putOn].head;
					int hadj=0;

					if (pickUp==2){
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								//fprintf(stdout, "one");
								hadj=-1; //was not in place
								break;
							}
							disk=disk->next;
						}
						//fprintf(stdout, "disk %d\n", disk->data);
							if (hadj==0 && disk->data==Ndisks-1){
								hadj=1; //was in place
								//fprintf(stdout, "two");
							}
							else {
								hadj=-1;  //was not in place
								//fprintf(stdout, "three");
							}
						disk=towers[pickUp].head;
					}

					towers[pickUp].head=towers[pickUp].head->next; //removes node from stack
					disk->next=NULL;
					towers[pickUp].length-=1; //decrease length of stack

					disk->next=towers[putOn].head; //add the node to new stack
					towers[putOn].head=disk;
					towers[putOn].length+=1;
					if (putOn==2){
						while(disk->next!=NULL){
							if (disk->next->data!=disk->data+1){
								hadj=1; //not in place
								//fprintf(stdout, "disk %d, next %d\n", disk->data, disk->next->data);
								break;
							}
						disk=disk->next;
						}
						if (hadj==0 && disk->data==Ndisks-1){
							hadj=-1; //in place
							//fprintf(stdout, "two\n");
						}
						else {
							hadj=1;  //not in place
							//fprintf(stdout, "three\n");
						}
					}
				h=h+hadj;

				assert (towers[putOn].head->next==under);
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
				//fprintf(stdout, "a: %d\n", a);
				for (int j=1;j<ARRAY_SIZE;j++){
					if (s.towers[j].head != NULL){
						b=s.towers[j].head->data;

					}
					//fprintf(stdout, "b: %d\n", b);
					if(a<b){                        //if move possible
						mvs[pos]=h.getmoveref(i,j);
						//fprintf(stdout, "i: %d, j: %d\n", i, j);
						//fprintf(stdout, "a<b pos: %d move: %d \n", pos, mvs[pos]);
						pos+=1;
					}
					else if (i!=j){                //else, inverse
						mvs[pos]=h.getmoveref(j,i);
						//fprintf(stdout, "j: %d, i: %d\n", j, i);
						//fprintf(stdout, "a>b pos: %d move: %d \n", pos, mvs[pos]);
						pos+=1;
					}
					b=Ndisks;
		}
		a=Ndisks;
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
			//domain.dumpstate(stdout, state);
				state.movedisk(revop, domain);
			//domain.dumpstate(stdout, state);
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
	// Print the state.
	void dumpstate(FILE *out, const State &s) const {
		for (int i=0; i < ARRAY_SIZE; i++){
			fprintf(out, "Stack %d: ", i);
			fflush(stdout);
			Piece* here = s.towers[i].head;
			for (int j=0; here!=NULL; j++){
				fprintf(out, "%u ", here->data);
				if (here->next==NULL){
					fprintf(out, "was NULL");
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
