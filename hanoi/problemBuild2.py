import random
import argparse as ap

def build(pieces, problem):
    order=[]
    Shuffle=[0,1,2] #More when more pillars
    for i in range(pieces, 0, -1):
        order.append(random.choice(Shuffle))
    write(order, pieces, problem)

def write(order, pieces, fileName):
    writeFile = open(fileName,"w")
    writeFile.write(str(pieces)+"\n")
    writeFile.write("Initial:\n")
    for i in order:
        writeFile.write(str(i)+"\n")
    writeFile.write("Goal:\n")
    for i in range(pieces):
        writeFile.write(str(2)+"\n")
    writeFile.close()

def standardBuild(pieces, problem):
    order=[]
    for i in range(pieces, 0, -1):
        order.append(0) #will be diffrent when miore pillars
    write(order,pieces,problem)

if __name__=="__main__":
    argParser = ap.ArgumentParser(description = 'Create a Towers of Hanoi Problem use -n and -f')
    argParser.add_argument('-n', type = int, help = "Number of pieces in the puzzle")
    argParser.add_argument('-f', type = str, help = "Name of input file/name of new puzzle")
    argParser.add_argument('-s', action='store_true', help = "Ask for the 'standard' start state")
    arguments = argParser.parse_args()

    problem = "HanoiProblem2.txt"
    pieces = 7

    if arguments.n!=None:
        pieces = int(arguments.n)
    if arguments.f != None:
        problem = arguments.f
    if arguments.s!=True:
        build(pieces, problem)
    else:
        standardBuild(pieces, problem)
