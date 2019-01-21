#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
  #include <Scope/Command/CommandParser.h>
}

using namespace testing;
using namespace std;

TEST(CommandParser, test_destroy)
{
  const size_t numberOfChannels = 3;
  ChannelHandle channels[numberOfChannels] = {NULL};
  
  ICommand* commandRunning = NULL;
  ICommand* commandPoll = NULL;
  ICommand* commandAddr = NULL;
  ICommand* commandTInc = NULL;
  ICommand* commandTrans = NULL;
  ICommand* commandTrigger = NULL;
  
  CommandParserHandle dispatcher = CommandParser_create(NULL, channels, numberOfChannels, NULL, NULL);
  
  commandRunning = CommandParser_getICommand(dispatcher, (const char*) "cf_running");
  ASSERT_THAT(commandRunning, NotNull());
  commandPoll = CommandParser_getICommand(dispatcher, (const char*) "ev_poll");
  ASSERT_THAT(commandPoll, NotNull());  
  commandAddr = CommandParser_getICommand(dispatcher, (const char*) "cf_addr");
  ASSERT_THAT(commandAddr, NotNull());
  commandTInc = CommandParser_getICommand(dispatcher, (const char*) "cf_t_inc");
  ASSERT_THAT(commandTInc, NotNull());
  commandTrans = CommandParser_getICommand(dispatcher, (const char*) "ev_trans");
  ASSERT_THAT(commandTrans, NotNull());
  commandTrigger = CommandParser_getICommand(dispatcher, (const char*) "cf_tgr");
  ASSERT_THAT(commandTrigger, NotNull());

  CommandParser_destroy(dispatcher);
}
