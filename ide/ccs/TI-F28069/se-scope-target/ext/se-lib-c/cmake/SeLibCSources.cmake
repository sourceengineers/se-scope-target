# Add sourcecode
set(SE_LIB_C_SOURCES
		${SE_LIB_C_SOURCE_DIR}/container/ByteRingBuffer.c
		${SE_LIB_C_SOURCE_DIR}/container/FloatRingBuffer.c
		${SE_LIB_C_SOURCE_DIR}/container/IntRingBuffer.c
		${SE_LIB_C_SOURCE_DIR}/logger/Logger.c
		${SE_LIB_C_SOURCE_DIR}/logger/LoggerBuilder.c
		${SE_LIB_C_SOURCE_DIR}/monitoring/MaxCheck.c
		${SE_LIB_C_SOURCE_DIR}/monitoring/MinCheck.c
		${SE_LIB_C_SOURCE_DIR}/stream/BufferedByteStream.c
		${SE_LIB_C_SOURCE_DIR}/stream/BufferedFloatStream.c
		${SE_LIB_C_SOURCE_DIR}/stream/BufferedIntStream.c
		${SE_LIB_C_SOURCE_DIR}/util/runnable/RunnableList.c
)


