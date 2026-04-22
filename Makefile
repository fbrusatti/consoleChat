all:
	$(CC) -Wall -o server src/server.c src/server/client.c -O2 -lpthread

tests:
	$(CC) -Wall -o tests_run tests/main.c tests/client_tests.c tests/unity.c src/server/client.c -O0
	./tests_run

integration:
	python3 tests/integration_test.py

clean:
	$(RM) -f server tests_run

.PHONY: all tests integration clean