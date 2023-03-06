import random
import argparse as ap

def build(pieces, problem):
    StackA=[]
    StackB=[]
    StackC=[]
    Shuffle=[StackA, StackB, StackC]
    for i in range(pieces, 0, -1):
        random.choice(Shuffle).append(i)
    #print(StackA)
    #print(StackB)
    #print(StackC)
    write(StackA,StackB,StackC,pieces,problem)

def write(A, B, C, pieces, fileName):
    writeFile = open(fileName,"w")
    writeFile.write(str(pieces)+"\n")
    writeFile.write("Stack A\n")
    while len(A):
        writeFile.write(str(A.pop())+"\n")
    writeFile.write("Stack B\n")
    while len(B):
        writeFile.write(str(B.pop())+"\n")
    writeFile.write("Stack C\n")
    while len(C):
        writeFile.write(str(C.pop())+"\n")
    writeFile.write("Goal:\n")
    writeFile.write("Stack A\n")
    writeFile.write("Stack B\n")
    writeFile.write("Stack C\n")
    for i in range(pieces):
        writeFile.write(str(i+1)+"\n")
    writeFile.close()

def standardBuild(pieces, problem):
    StackA=[]
    StackB=[]
    StackC=[]
    for i in range(pieces, 0, -1):
        StackA.append(i)
    write(StackA,StackB,StackC,pieces,problem)

if __name__=="__main__":
    argParser = ap.ArgumentParser(description = 'Create a Towers of Hanoi Problem use -n and -f')
    argParser.add_argument('-n', type = int, help = "Number of pieces in the puzzle")
    argParser.add_argument('-f', type = str, help = "Name of input file/name of new puzzle")
    argParser.add_argument('-s', action='store_true', help = "Ask for the 'standard' start state")
    arguments = argParser.parse_args()

    problem = "HanoiProblem.txt"
    pieces = 7

    if arguments.n!=None:
        pieces = int(arguments.n)
    if arguments.f != None:
        problem = arguments.f
    if arguments.s!=True:
        build(pieces, problem)
    else:
        standardBuild(pieces, problem)
