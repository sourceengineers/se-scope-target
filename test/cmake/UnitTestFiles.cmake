
set(SE_SCOPE_TEST_MOCKS
        ${SE_SCOPE_PROJECT_TEST_DIR}/ScopeMock/ScopeMock.c
        ${SE_SCOPE_PROJECT_TEST_DIR}/Observer/ObserverMock.c
        )

set(SE_SCOPE_UNIT_TESTS
        ${SE_SCOPE_PROJECT_TEST_DIR}/Control/AnnounceStorage-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Core/channel-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Core/trigger-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/main.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Integration/CommandIntegration-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Integration/FullIntegration-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Communication/FramedIO-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Serialisation/Protobuf/NanopbUnpacker-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/Serialisation/Protobuf/NanopbPacker-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/GeneralPurpose/Bytestream-test.cpp
        ${SE_SCOPE_PROJECT_TEST_DIR}/GeneralPurpose/FloatRingBuffer-test.cpp
        )