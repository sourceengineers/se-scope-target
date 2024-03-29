# Add sourcecode
set(SE_SCOPE_SOURCES
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandClearParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandTIncParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandTransParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandRunningParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandPollParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandDetectParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandAnnounceParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandTriggerParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/CommandParser/CommandAddrParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandTInc.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandDetect.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandTrigger.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandAnnounce.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandTrans.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandAddr.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandPoll.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandRunning.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Commands/Command/CommandClear.c
        ${SE_SCOPE_SOURCE_DIR}/Control/AnnounceStorage.c
        ${SE_SCOPE_SOURCE_DIR}/Control/CommandPackParserDispatcher.c
        ${SE_SCOPE_SOURCE_DIR}/Control/CommandParserDispatcher.c
        ${SE_SCOPE_SOURCE_DIR}/Control/ParserDefinitions.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/CommandParser/CommandPackAnnounceParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/CommandParser/CommandPackDataParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/CommandParser/CommandPackLogParser.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/Command/CommandPackLog.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/Command/CommandPackAnnounce.c
        ${SE_SCOPE_SOURCE_DIR}/Control/PackCommands/Command/CommandPackData.c
        ${SE_SCOPE_SOURCE_DIR}/Control/Controller.c
        ${SE_SCOPE_SOURCE_DIR}/Core/Timestamper.c
        ${SE_SCOPE_SOURCE_DIR}/Core/Channel.c
        ${SE_SCOPE_SOURCE_DIR}/Core/Scope.c
        ${SE_SCOPE_SOURCE_DIR}/Core/Trigger.c
        ${SE_SCOPE_SOURCE_DIR}/Communication/Interfaces/FramedIO.c
        ${SE_SCOPE_SOURCE_DIR}/Builders/ScopeRunner.c
        ${SE_SCOPE_SOURCE_DIR}/Builders/ScopeFramedStack.c
        ${SE_SCOPE_SOURCE_DIR}/Builders/ScopeThreadRunner.c
        ${SE_SCOPE_SOURCE_DIR}/Builders/ScopeBuilder.c
        ${SE_SCOPE_SOURCE_DIR}/Serialisation/Protobuf/NanopbUnpacker.c
        ${SE_SCOPE_SOURCE_DIR}/Serialisation/Protobuf/se-scope.pb.c
        ${SE_SCOPE_SOURCE_DIR}/Serialisation/Protobuf/NanopbPacker.c
        ${SE_SCOPE_SOURCE_DIR}/Serialisation/Serializer.c
        )

set(SE_SCOPE_PUBLIC_HEADERS
        ${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope/Builders/ScopeFramedStack.h
        ${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope/Communication/ITransceiver.h
        ${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope/Control/AnnounceStorage.h
        ${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope/GeneralPurpose/DataTypes.h
        ${SE_SCOPE_PROJECT_INCLUDE_DIR_PUBLIC}/Scope/GeneralPurpose/IMutex.h
        )
