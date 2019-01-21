#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
  #include <Scope/Parser/CommandParserDispatcher.h>
}

using namespace testing;
using namespace std;

TEST(CommandParserDispatcher, test_destroy)
{
  const size_t numberOfChannels = 3;
  ChannelHandle channels[numberOfChannels] = {NULL};

  ICommand* commandTrans = NULL;
  ICommand* commandClear = NULL;
  ICommand* commandAnnounce = NULL;

  CommandParserDispatcherHandle commandParserDispatcher = CommandParserDispatcher_create(NULL, channels, numberOfChannels, NULL, NULL);
  
  commandTrans = CommandParserDispatcher_run(commandParserDispatcher, (const char*) "ev_trans");
  ASSERT_THAT(commandTrans, NotNull());
  commandClear = CommandParserDispatcher_run(commandParserDispatcher, (const char*) "ev_clear");
  ASSERT_THAT(commandClear, NotNull());
  commandAnnounce = CommandParserDispatcher_run(commandParserDispatcher, (const char*) "ev_announce");
  ASSERT_THAT(commandAnnounce, NotNull());

  CommandParserDispatcher_destroy(commandParserDispatcher);
}
