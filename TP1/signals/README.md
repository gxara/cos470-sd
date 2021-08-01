# Signals

These programs were developed in order to practice the content studied about signals on the distributed systems class

## Executing

### Signal Handler

The signal handler is a program that creates custom handlers for signals sent by the operating system.

The program can be ran with two different types of waiting:

- busy wait -> The program will verify if any signal has been received in an infinite loop. This way the proccess will consume 100% of the time given by the CPU

  `make run_signal_handler wait_type=busy`

- blocking wait -> The program will check if any signal has been received. And then will sleep for a certain amount of time

  `make run_signal_handler wait_type=block`

### Signal Sender

The signal sender is a program that sends a desired signal to a proccess specified by the user.

The program must be run with two command line arguments: pid (the ID of the proccess that will receive the signal) and signal (the ID of the signal that must be sent)

Example:
`make run_signal_sender pid=123 signal=1`

## Available Signals

#### 1 SIGABRT

Abnormal termination of the program, such as a call to abort.

#### 2 SIGFPE

An erroneous arithmetic operation, such as a divide by zero or an operation resulting in overflow.

#### 3 SIGILL

Detection of an illegal instruction.

#### 4 SIGINT

Receipt of an interactive attention signal.

#### 5 SIGSEGV

An invalid access to storage.

#### 6 SIGTERM

A termination request sent to the program.
