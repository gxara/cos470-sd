build:
	@mkdir -p bin
	@echo 'Compiling program'
	g++ -std=c++11 src/pipes.cpp -o bin/tp1_pipes

clean:
	@echo 'Cleaning project'
	# @rm -r ./bin/*

run:
	# @make clean 
	@make build
	@echo $('\n\n')
	@bin/tp1_pipes $(messages)
