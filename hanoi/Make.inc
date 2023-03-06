hanoi:\
	hanoi/hanoi_solver

CLEAN+=\
	hanoi/hanoi_solver\

EVERYTHING+=hanoi

hanoiSRC=\
	hanoi/hanoi.cc\

CLEAN+=$(hanoiSRC:.cc=.o)


hanoi/hanoi_solver:\
	$(hanoiSRC)\
	hanoi/main.cc\
	utils/utils.a\
	structs/structs.a\
	search/search.a
	@echo $@
	@$(CXX) $(hanoiSRC)\
		hanoi/main.cc\
		utils/utils.a\
		structs/structs.a\
		search/search.a\
		$(LDFLAGS) -DNDISKS=$*  -o $@

include hanoi/main.d
