#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
  #include <Scope/Command/CommandDispatcher.h>
}

using namespace testing;
using namespace std;

TEST(CommandDispatcher, test_destroy)
{
  const size_t numberOfChannels = 3;
  ChannelHandle channels[numberOfChannels] = {NULL};
  
  ICommand* commandRunning = NULL;
  ICommand* commandPoll = NULL;
  ICommand* commandAddr = NULL;
  ICommand* commandTInc = NULL;
  ICommand* commandTrans = NULL;
  ICommand* commandTrigger = NULL;
  
  CommandDispatcherHandle dispatcher = CommandDispatcher_create(NULL, channels, numberOfChannels, NULL, NULL);
  
  commandRunning = CommandDispatcher_getICommand(dispatcher, (const char*) "cf_running");
  ASSERT_THAT(commandRunning, NotNull());
  commandPoll = CommandDispatcher_getICommand(dispatcher, (const char*) "ev_poll");
  ASSERT_THAT(commandPoll, NotNull());  
  commandAddr = CommandDispatcher_getICommand(dispatcher, (const char*) "cf_addr");
  ASSERT_THAT(commandAddr, NotNull());
  commandTInc = CommandDispatcher_getICommand(dispatcher, (const char*) "cf_t_inc");
  ASSERT_THAT(commandTInc, NotNull());
  commandTrans = CommandDispatcher_getICommand(dispatcher, (const char*) "ev_trans");
  ASSERT_THAT(commandTrans, NotNull());
  commandTrigger = CommandDispatcher_getICommand(dispatcher, (const char*) "cf_tgr");
  ASSERT_THAT(commandTrigger, NotNull());

  CommandDispatcher_destroy(dispatcher);
}
