build_signal_handler:
	@mkdir -p bin
	@echo 'Compiling program'
	g++ -std=c++11 src/signal_handler.cpp -o bin/tp1_signal_handler

build_signal_sender:
	@mkdir -p bin
	@echo 'Compiling program signal_sender'
	g++ -std=c++11 src/signal_sender.cpp -o bin/tp1_signal_sender

build:
	@make build_signal_handler
	@make build_signal_sender

clean:
	@echo 'Cleaning project'
	# @rm -r ./bin/*

run_signal_handler:
	# @make clean 
	@make build_signal_handler
	@echo $('\n\n')
	@bin/tp1_signal_handler $(wait_type)

run_signal_sender: 
	# @make clean 
	@make build_signal_sender
	@echo $('\n\n')
	@bin/tp1_signal_sender $(pid) $(signal)