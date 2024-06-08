#!/bin/bash

# Function to check if checksum has changed
check_checksum() {
    # Define paths
    FILE="CMakeLists.txt"
    CHECKSUM_FILE="cmake.checksum"

    # Get current checksum of a.txt
    current_checksum=$(md5sum "$FILE" | awk '{print $1}')

    # Check if checksum.txt exists
    if [ -f "$CHECKSUM_FILE" ]; then
        # Read last checksum from checksum.txt
        last_checksum=$(cat "$CHECKSUM_FILE")
        
        # Compare current and last checksum
        if [ "$current_checksum" = "$last_checksum" ]; then
            no_changes_detected
        else
            # Update checksum.txt with current checksum
            echo "$current_checksum" > "$CHECKSUM_FILE"
            checksum_changed
        fi
    else
        # If checksum.txt doesn't exist, create it with current checksum
        echo "$current_checksum" > "$CHECKSUM_FILE"
        checksum_file_created
    fi
}

# Function called when no changes are detected
no_changes_detected() {
	cmake --build ./build
    exit 0
}

# Function called when checksum has changed
checksum_changed() {
    rm -r build
	mkdir build
	cmake -S . -B ./build
	cmake --build ./build
    exit 1
}

# Function called when checksum file is created
checksum_file_created() {
    rm -r build
	mkdir build
	cmake -S . -B ./build
	cmake --build ./build
    exit 1
}

# Call the function to check the checksum
check_checksum
