build:
	@mkdir -p bin
	@echo 'Compiling program'
	g++ -pthread -std=c++11 src/adder.cpp -o bin/tp2_adder

clean:
	@echo 'Cleaning project'
	@rm -r ./bin/*

run:
	# @make clean 
	@make build
	@echo $('\n\n')
	@bin/tp2_adder $(threads)
