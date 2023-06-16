CLIENTPATH = client

#c++ because for some reason .h's only work with c++ and not c
TAGMAKE = ctags --fields=+l --c-kinds=+p --c++-kinds=+p
TAGINCLUDES = -R /mnt/data/code/c/libs/foamengine/inc -R .

all: client tags

client: linux win tags

nix: linux tags

tags:
	$(TAGMAKE) --exclude=client/build --exclude=.ycm_extra_conf.py --exclude=Makefile $(TAGINCLUDES)

linux:
	+$(MAKE) -C $(CLIENTPATH) -f Makefile

win:
	+$(MAKE) -C $(CLIENTPATH) -f Makefile.WIN32

clean: cleanobjs cleantags

cleanobjs:
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile
	+$(MAKE) clean -C $(CLIENTPATH) -f Makefile.WIN32

cleantags:
	@rm -rf tags


.PHONY: nix 

