COREPATH  = core
CLIENTPATH = client
SERVERPATH = server

#c++ because for some reason .h's only work with c++ and not c
TAGMAKE = ctags --fields=+l --c-kinds=+p --c++-kinds=+p
TAGINCLUDES = -R /mnt/data/code/c/libs/foamengine/inc -R .

all: client server tags

client: linux win emcc tags

nix: linux server tags

tags:
	$(TAGMAKE) --exclude=core/build --exclude=client/build --exclude=server/build --exclude=.ycm_extra_conf.py --exclude=Makefile $(TAGINCLUDES)

linux:
	+$(MAKE) -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(CLIENTPATH) -f Makefile

win:
	+$(MAKE) -C $(COREPATH) -f Makefile.WIN32
	+$(MAKE) -C $(CLIENTPATH) -f Makefile.WIN32

emcc:
	+$(MAKE) -C $(CLIENTPATH) -f Makefile.EMCC

server:
	+$(MAKE) clean -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(COREPATH) -f Makefile SERVER=1
	+$(MAKE) -C $(SERVERPATH) -f Makefile

core:
	+$(MAKE) -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(COREPATH) -f Makefile.WIN32


clean: cleanobjs cleantags

cleanobjs:
	+$(MAKE) clean -C $(COREPATH) -f Makefile
	+$(MAKE) clean -C $(COREPATH) -f Makefile.WIN32
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile.WIN32
	+$(MAKE) clean -C $(SERVERPATH) -f Makefile

cleantags:
	@rm -rf tags


.PHONY: nix core server tools 

