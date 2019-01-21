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

  ICommand* commandTrans = NULL;
  ICommand* commandClear = NULL;
  ICommand* commandAnnounce = NULL;

  CommandParserHandle commandParser = CommandParser_create(NULL, channels, numberOfChannels, NULL, NULL);
  
  commandTrans = CommandParser_run(commandParser, (const char*) "ev_trans");
  ASSERT_THAT(commandTrans, NotNull());
  commandClear = CommandParser_run(commandParser, (const char*) "ev_clear");
  ASSERT_THAT(commandClear, NotNull());
  commandAnnounce = CommandParser_run(commandParser, (const char*) "ev_announce");
  ASSERT_THAT(commandAnnounce, NotNull());

  CommandParser_destroy(commandParser);
}
