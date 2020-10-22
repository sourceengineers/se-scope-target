#!/bin/bash

# Build the scope test
make se-scope-test

# Set the name and path of the folder the test report should be put into
export GTEST_OUTPUT="xml:test-results/test-results.xml"

# Run the test
./test/se-scope-test