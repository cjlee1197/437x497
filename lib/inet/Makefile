CONFIGFILES=kconfig.mk
include $(CONFIGFILES)
DEP=$(addprefix $(DEPDIR),$(LIBS:.so=.d)) 
all:$(LIBS)
	@for i in $(SUBDIRS); do make -C $$i $@; done
$(DEPDIR)%.d: %.cpp
	@install -d $(DEPDIR)
	$(CXX) -MM $< | sed 's,\($*\)\.o[:]*, \1.o $@:,g' > $@
%.so:%.o
	$(CXX) $(LIBFLAGS) -o $@ $< 
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< 
sinclude $(DEP)

#########################################################################
## Machine Architecture configure
#########################################################################

255_config:
	@echo "configure mach 255..."
	@$(SED) -i 's/MACH=.*/MACH=255/' $(CONFIGFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done
270_config:
	@echo "configure mach 270..."
	@$(SED) -i 's/MACH=.*/MACH=270/' $(CONFIGFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done
x86_config:
	@echo "configure mach x86..."
	@$(SED) -i 's/MACH=.*/MACH=x86/' $(CONFIGFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done
cur_config:
	@echo machine: `grep -ir MACH= $(CONFIGFILES) | sed 's/MACH=//'`
	@for i in $(SUBDIRS); do make -C $$i $@; done
3354A_config: 
	@echo "configure mach 3354A..."
	@$(SED) -i 's/MACH=.*/MACH=3354A/' $(CONFIGFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done
3354L_config: 
	@echo "configure mach 3354L..."
	@$(SED) -i 's/MACH=.*/MACH=3354L/' $(CONFIGFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done

#########################################################################
## Create Package with DEBUG symbols
#########################################################################

debug:CXXFLAGS+=$(DEBUGFLAGS)
debug: $(LIBS)
	@for i in $(SUBDIRS); do make -C $$i $@; done

#########################################################################
## Create normal distribute Package or .gz type one
#########################################################################

release: 
ifeq ($(CURDIR),$(INSTALLDIR))
	$(error Please run this command in the top directory)
endif
	@$(INSTALL) -d $(INSTALLDIR)
ifneq (, $(EXTRAFILES))
	@$(INSTALL) $(EXTRAFILES) $(INSTALLDIR) 
endif
ifneq (,$(EXTRADIRS))
	@$(COPY) $(EXTRADIRS) $(INSTALLDIR)  && \
	rm -rf `find $(INSTALLDIR)$($EXTRADIR) | grep /.svn` 
endif
	@for i in $(SUBDIRS); do make -C $$i $@; done

#########################################################################
## Clear the packages, objects, even dependences to get clean source tree
#########################################################################

dist-clean: clean release-clean
	rm  -rf $(DEPDIR)
	@for i in $(SUBDIRS); do make -C $$i $@; done
clean: 
	rm -rf $(DELFILES)
	@for i in $(SUBDIRS); do make -C $$i $@; done
.PHONY: all clean release dist-clean release-clean 255_config\
	270_config i86_config cur_config
