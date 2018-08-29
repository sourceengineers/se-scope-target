#include <gtest/gtest.h>

/*

{
	"transport": "...",
	"payload": {
		"sc_cmd": {
			"cf_running": {
				"1": false,
				"2": true,
				"3": false,
				"4": true,
				"5": true
			},
			"ev_poll": "110",
			"ev_trans": "",
			"cf_addr": {
				"1": 1111,
				"4": 4444,
				"5": 5555
			},
			"cf_tgr": {
				"cl_id": 1,
				"mode": "Continous",
				"level": 2.5,
				"edge": true
			},
			"cf_t_inc": 10
		},
		"flow_ctrl": "ACK"
	}
}
*/

//82 a9 74 72 61 6e 73 70 6f 72 74 a3 2e 2e 2e a7 70 61 79 6c 6f 61 64 82 a6 73 63 5f 63 6d 64 86 aa 63 66 5f 72 75 6e 6e 69 6e 67 85 a1 31 c2 a1 32 c3 a1 33 c2 a1 34 c3 a1 35 c3 a7 65 76 5f 70 6f 6c 6c a3 31 31 30 a8 65 76 5f 74 72 61 6e 73 a0 a7 63 66 5f 61 64 64 72 83 a1 31 cd 04 57 a1 34 cd 11 5c a1 35 cd 15 b3 a6 63 66 5f 74 67 72 84 a5 63 6c 5f 69 64 01 a4 6d 6f 64 65 a9 43 6f 6e 74 69 6e 6f 75 73 a5 6c 65 76 65 6c cb 40 04 00 00 00 00 00 00 a4 65 64 67 65 c3 a8 63 66 5f 74 5f 69 6e 63 0a a9 66 6c 6f 77 5f 63 74 72 6c a3 41 43 4b

extern "C" {
//	#include "Shared/Memory/MockMemoryAllocator.h"
    #include "msgpack_parser.h"
}

/*class msgpack_parserTest : public ::testing::Test
{
protected:
    msgpack_parserTest()
        : Test()
    {
        // TODO: Allocate as much memory you require for the unit under test
		MockMemoryAllocator_initialize(&_mockAllocator, 50);
    }

    void SetUp() override
    {
        _msgpack_parser = msgpack_parser_create(&_mockAllocator.memoryAllocator);
    }

    void TearDown() override
    {
    	MockMemoryAllocator_reset(&_mockAllocator);
    }

    virtual ~msgpack_parserTest()
    {
        MockMemoryAllocator_destroy(&_mockAllocator);
    }

    MockMemoryAllocator _mockAllocator;
    msgpack_parser_Handle _msgpack_parser;
};*/

/*TEST_F(msgpack_parserTest, msgpack_parserCreated)
{
	// test that the template was created
  //  EXPECT_NE(nullptr, _msgpack_parser);
}*/
