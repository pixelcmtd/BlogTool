all:
	@g++ blogtool.cpp -O3 -s -o blogtool
test:
	@g++ blogtool.cpp -Og -Wall -Wextra -o blogtool.dbg
	@./blogtool.dbg "Test Article" testarticle.raw \
		testarticle.txt testarticle.html
clean:
	@rm blogtool.dbg blogtool testarticle.txt testarticle.html
