#---------------------------------------------------------------------------------------------------------------------
# Butano custom IWRAM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_iwram.o: %.bn_iwram.cpp
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CXX) "$(CPPFLAGS) $(CXXFLAGS) -fno-lto -marm -mlong-calls -c $< -o $@" $<
endif
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_iwram.d $(CPPFLAGS) $(CXXFLAGS) -fno-lto -marm -mlong-calls -c $< -o $@ $(ERROR_FILTER)

%.bn_iwram.o: %.bn_iwram.c
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CC) "$(CPPFLAGS) $(CFLAGS) -fno-lto -marm -mlong-calls -c $< -o $@" $<
endif
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_iwram.d $(CPPFLAGS) $(CFLAGS) -fno-lto -marm -mlong-calls -c $< -o $@ $(ERROR_FILTER)
	
#---------------------------------------------------------------------------------------------------------------------
# Butano custom EWRAM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_ewram.o: %.bn_ewram.cpp
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CXX) "$(CPPFLAGS) $(CXXFLAGS) -fno-lto -c $< -o $@" $<
endif
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_ewram.d $(CPPFLAGS) $(CXXFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

%.bn_ewram.o: %.bn_ewram.c
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CC) "$(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@" $<
endif
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_ewram.d $(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------------------------------------------
# Butano custom ROM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_noflto.o: %.bn_noflto.cpp
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CXX) "$(CPPFLAGS) $(CXXFLAGS) -fno-lto -c $< -o $@" $<
endif
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_noflto.d $(CPPFLAGS) $(CXXFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

%.bn_noflto.o: %.bn_noflto.c
	$(SILENTMSG) $(notdir $<)
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) add $(CC) "$(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@" $<
endif
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_noflto.d $(CPPFLAGS) $(CFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)
