build_server:
	@mkdir -p bin
	@echo 'Compiling program'
	g++ -std=c++11 src/server.cpp -o bin/tp1_server

build_client:
	@mkdir -p bin
	@echo 'Compiling program client'
	g++ -std=c++11 src/client.cpp -o bin/tp1_client

build:
	@make build_server
	@make build_client

clean:
	@echo 'Cleaning project'
	# @rm -r ./bin/*

run_server:
	# @make clean 
	@make build_server
	@echo $('\n\n')
	@bin/tp1_server

run_client: 
	# @make clean 
	@make build_client
	@echo $('\n\n')
	@bin/tp1_client $(N)