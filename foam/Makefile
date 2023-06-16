COREPATH  = core
CLIENTPATH = client
TOOLSPATH = tools/exporter

#c++ because for some reason .h's only work with c++ and not c
TAGMAKE = ctags --fields=+l --c-kinds=+p --c++-kinds=+p -R

all: client tags

client: linux win emcc tags

nix: linux headers tags

headers:
	@rm -rf /mnt/data/code/c/libs/foamengine/inc/foam
	@mkdir -p /mnt/data/code/c/libs/foamengine/inc/foam
	@cp -R client/inc/foam/client /mnt/data/code/c/libs/foamengine/inc/foam/
	@cp -R core/inc/foam/core /mnt/data/code/c/libs/foamengine/inc/foam/
	@cp foam_core.h /mnt/data/code/c/libs/foamengine/inc/foam
	@cp foam_client.h /mnt/data/code/c/libs/foamengine/inc/foam

tags:
	@ctags --fields=+l --c-kinds=+p --c++-kinds=+p -R --exclude=core/build --exclude=foam/client/build --exclude=.ycm_extra_conf.py --exclude=Makefile

linux:
	+$(MAKE) -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(CLIENTPATH) -f Makefile

win:
	+$(MAKE) -C $(COREPATH) -f Makefile.WIN32
	+$(MAKE) -C $(CLIENTPATH) -f Makefile.WIN32

emcc:
	+$(MAKE) -C $(CLIENTPATH) -f Makefile.EMCC

core:
	+$(MAKE) -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(COREPATH) -f Makefile.WIN32

tools:
	+$(MAKE) -C $(COREPATH) -f Makefile
	+$(MAKE) -C $(TOOLSPATH) -f Makefile

clean: cleanobjs cleantags

cleanobjs:
	+$(MAKE) clean -C $(COREPATH) -f Makefile
	+$(MAKE) clean -C $(COREPATH) -f Makefile.WIN32
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile.WIN32
	+$(MAKE) clean -C $(TOOLSPATH) -f Makefile

cleantags:
	@rm -rf tags


.PHONY: nix core tools 
