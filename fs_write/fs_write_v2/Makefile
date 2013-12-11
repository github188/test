MAKE = make
all:exec
exec:
	cd ./src && $(MAKE)
	@mv ./src/fs_write ./
	@cd ..
	@echo "make success"
clean:
	@rm ./fs_write
	@cd ./src && $(MAKE) clean
	@echo "done!"
